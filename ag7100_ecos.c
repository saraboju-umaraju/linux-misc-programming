// ==========================================================================
// 
// dev/ag7100ecos.c
// 
// Ethernet driver for AG7100
//
// vim: ts=4
// 
// ==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2003 Atheros Communications, Inc.
// 
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later
// version.
// 
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
// 
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
// 
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
// 
// Alternative licenses for eCos may be arranged by contacting the copyright
// holders.
// -------------------------------------------
// ####ECOSGPLCOPYRIGHTEND####
// ==========================================================================
// #####DESCRIPTIONBEGIN####
// 
// Author(s): Atheros Communications, Inc.
// Contributors: Atheros Engineering
// Date: 2003-10-22
// Purpose:
// Description: AG7100 ethernet hardware driver
// 
// 
// ####DESCRIPTIONEND####
// 
// ==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/io_eth_drivers.h>
#include <pkgconf/devs_eth_mips_mips32_ag7100.h>
#ifdef CYGPKG_NET
#include <pkgconf/net.h>
#include <cyg/kernel/kapi.h>
#endif
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_endian.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/plf_io.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/drv_api.h>
#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>
#include <cyg/io/eth/eth_drv_stats.h>
#include <string.h>

#include <sys/param.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#ifdef USE_MBUF_FOR_ETHERNET_DRIVER
#include <sys/mbuf.h>
#endif
#include <sys/socket.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <net/netisr.h>

#include "athrs_mac_def.h"

#define QCA_MISC_IRQ_ETHSW 17

#define ATHR_PRINTF(x, args...)  diag_printf("%s:%d:" x, __func__, __LINE__, ##args)
#if 0
#define ATHR_DPRINTF(x, args...) do {                                                               \
                                     if (athr_debug) {                                              \
                                         diag_printf("%s:%d:" x "\n", __func__, __LINE__, ##args);  \
                                     }                                                              \
                                 } while (0)
#else
#define ATHR_DPRINTF(x, args...)
#endif
#define ARRIVE()  ATHR_DPRINTF("begin");
#define LEAVE()  ATHR_DPRINTF("end");

int g_debug = 1;
#if 1
#define CONSOLE_PRINTF(fmt, arg...) if(g_debug == 1) {              \
    do {                                                             \
        diag_printf("%s[%4d]: "fmt"\n", __func__, __LINE__, ##arg);    \
    } while(0); \
}
#else
#define CONSOLE_PRINTF(fmt, arg...) do {                                                    \
                               diag_printf("%s[%4d]: "fmt"\n", __func__, __LINE__, ##arg);    \
                          } while(0)
#endif

#define AG7100_LINK_CHK_INTVL 0xffff

int frame_sz = 0;
static cyg_handle_t link_interrupt_handle;
static cyg_interrupt link_interrupt_object;

//#define DUMP_PACKET

void athr_dump_gmac_reg(int cmd);
void ag7100_get_macaddr(athr_gmac_t * mac, cyg_uint8 * mac_addr);
static cyg_uint32 ag7100_isr(cyg_vector_t vector, cyg_addrword_t data);

static int ag7100_if_poll_xmit(struct ifnet *ifp, int *budget);
static int ag7100_if_poll_recv(struct ifnet *ifp, int *budget);

void
ag7100_g_debug(int set)
{
    g_debug = set;
}

#ifdef USE_MBUF_FOR_ETHERNET_DRIVER

static cyg_uint8 *
ag7100_get_pkt_buf(void *m)
{
    return mtod((struct mbuf *)m, cyg_uint8 *);
}

#else

static cyg_uint8 *
ag7100_get_pkt_buf(cyg_uint8 * p)
{
    return p;
}

#endif

void
athr_gmac_handle_tx_complete(struct eth_drv_sc *sc)
{
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    ag7100_ring_t *r = &mac->txring;
    ag7100_desc_t *ds;
    int head = r->free_head;
    int tail = r->used_head;

    // interrupt disable
    athr_gmac_intr_disable_tx(mac);
    CONSOLE_PRINTF
        ("+++athr_gmac_handle_tx_complete+++\n");
    while (tail != head) {
        ds = (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(&r->desc[tail]);
        if (athr_gmac_tx_owned_by_dma(ds)) {
            break;
        } else {
            (sc->funs->eth_drv->tx_done) (sc, mac->txkeys[tail], 0);
        }
        tail++;
        if (tail >= AG7100_TX_DESC_CNT) {
            tail = 0;
        }

        athr_gmac_intr_ack_tx(mac);
    }
    r->used_head = tail;
    diag_printf("Reap tx desc, now head=%d, tail=%d\n", r->free_head,
                r->used_head);
    // interrupt enable
}

static int
ag7100_can_send(struct eth_drv_sc *sc)
{
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    ag7100_ring_t *r = &mac->txring;
    int count;

    count = ((AG7100_TX_DESC_CNT-1) -
		((r->free_head - r->used_head) & (AG7100_TX_DESC_CNT-1)));

    diag_printf("ag7100_can_send TXRING free=%d, used=%d, avbl=%d\n",
                r->free_head, r->used_head, count);

	return count;
}

static void
ag7100_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
            int sg_len, int total, unsigned long key)
{
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    ag7100_ring_t *r = &mac->txring;
    ag7100_desc_t *ds;
    char *buf;
    int len = 0;
    int i = 0;

    ds = (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(&r->desc[r->free_head]);
    buf = ag7100_get_pkt_buf(ds->packet);

    for (i = 0; i < sg_len; i++) {
        memcpy(buf + len, (void *)sg_list[i].buf, sg_list[i].len);
        len += sg_list[i].len;
    }
    mac->txkeys[r->free_head] = key;
#ifdef DUMP_PACKET
    diag_printf("DUMP TX frame\n");
    for (i = 0; i < 50; i++) {
        diag_printf("%02x ", buf[i]);
    }
    diag_printf("\n");
#endif
    //athr_gmac_handle_tx_complete(sc);

    if (athr_gmac_tx_owned_by_dma(ds)) {
        diag_printf("tx ring index=%d is busy, return\n", r->free_head);
        return;
    }
    A_DATA_CACHE_FLUSH(buf, len);
    ds->pkt_size = len;
    HAL_REORDER_BARRIER();
    ds->more = 0;
    ds->res1 = 0;
    ds->res2 = 0;
    ds->res3 = 0;

    athr_gmac_tx_give_to_dma(ds);
    if (++(r->free_head) >= AG7100_TX_DESC_CNT) {
        r->free_head = 0;
    }
    diag_printf
        ("NULL_SEND: TXRING, free=%d, used=%d, give DS=%p to DMA, empty=%d\n",
         r->free_head, r->used_head, ds, ds->is_empty);

    athr_gmac_intr_enable_tx(mac);
    athr_gmac_tx_start(mac);
}

static void
ag7100_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, int sg_len)
{

    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    int used_idx = 0;
    ag7100_desc_t *rxdesc;
    int i = 0;
    ag7100_ring_t *rxring = &mac->rxring;

    used_idx = rxring->used_head;
    rxdesc = (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(&rxring->desc[used_idx]);
    char *buf_ptr = ag7100_get_pkt_buf(rxdesc->packet);

#ifdef DUMP_PACKET
    for (i = 0; i < 50; i++) {
        diag_printf("%02x ", buf_ptr[i]);
    }
    diag_printf("\n");
#endif

    for (i = 0; i < sg_len; i++) {
        if (sg_list[i].buf != 0) {
            memcpy((void *)sg_list[i].buf, buf_ptr, sg_list[i].len);
            buf_ptr += sg_list[i].len;
        }
    }
}

static void
ag7100_poll(struct eth_drv_sc *sc)
{
}

void
athr_handle_rx_int(athr_gmac_t * mac)
{
    ag7100_ring_t *rxring = &mac->rxring;
    ag7100_desc_t *rxdesc;
    int used_idx;
    int pkt_size;
    int i = 0;

    CONSOLE_PRINTF("In RX DSR handle\n");
    used_idx = rxring->used_head;

    while (1) {
        rxdesc = (ag7100_desc_t *)
            CYGARC_UNCACHED_ADDRESS(&rxring->desc[used_idx]);
        if (athr_gmac_rx_owned_by_dma(rxdesc)) {
            break;
        }
        athr_gmac_intr_ack_rx(mac);
        pkt_size = rxdesc->pkt_size;
        A_DATA_CACHE_INVAL(ag7100_get_pkt_buf(rxdesc->packet), pkt_size);
        pkt_size -= 4;
#ifdef DUMP_PACKET
        diag_printf("athr_handle_rx_int\n");
        for (i = 0; i < 50; i++) {
            diag_printf("%02x ", ag7100_get_pkt_buf(rxdesc->packet)[i]);
        }
        diag_printf("\n");
#endif
        (mac->sc->funs->eth_drv->recv) (mac->sc, pkt_size);
        CYG_ASSERT(m != NULL, "incorrect rxdesc");
        // rxdesc->packet = NULL;

        if (++used_idx >= AG7100_RX_DESC_CNT) {
            used_idx = 0;
        }
        diag_printf
            ("Received a frame, len=%d, now return it back to free desc\n",
             pkt_size);
        athr_gmac_rx_give_to_dma(rxdesc);
    }
    rxring->used_head = rxring->free_head = used_idx;
    CONSOLE_PRINTF("end  r->free_head:%u used_head:%d size:%u",
                   rxring->free_head, rxring->used_head, rxring->size);
    LEAVE();

    return;
}

void
athr_handle_tx_int(athr_gmac_t * mac)
{
    CONSOLE_PRINTF("In TX DSR handle\n");
    athr_gmac_handle_tx_complete(mac->sc);
}

static void
ag7100_deliver(struct eth_drv_sc *sc)
{
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;

    if (mac->isr & ATHR_GMAC_INTR_RX) {
        athr_handle_rx_int(mac);
    }
    if (mac->isr & ATHR_GMAC_INTR_TX) {
        athr_handle_tx_int(mac);
    }

    athr_gmac_int_enable(mac);
}

/*
 * This is an ethernet driver - there is no 80211 packets involved. Just
 * get straight large buffers in the driver and do not use mbuf's. 
 */
#ifdef USE_MBUF_FOR_ETHERNET_DRIVER

#define MIN_HEAD_ROOM  64

static inline void *
ag7100_alloc_pkt(void)
{
    struct mbuf *m;

    MGETHDR(m, M_DONTWAIT, MT_DATA);
    if (m == NULL)
        return NULL;

    MCLGET(m, M_DONTWAIT);
    if ((m->m_flags & M_EXT) == 0) {
        m_free(m);
        return NULL;
    }
    m->m_len = 0;

#if 1                           // reserved 64 byte for 80211
    // protocol packet encapsulation
    m->m_data += ((MIN_HEAD_ROOM + sizeof(long)) & ~(sizeof(long) - 1));
#else
    // edward
    M_ALIGN(m, MIN_HEAD_ROOM);
#endif
    m->m_nextpkt = NULL;
    m->m_next = NULL;

    return m;
}

static inline void
ag7100_free_pkt(void *m)
{
    m_freem(m);
}

#else /* USE_MBUF_FOR_ETHERNET_DRIVER */

static inline void *
ag7100_alloc_pkt(void)
{
    void *ptr;
    ptr = malloc(AG7100_RX_BUF_SIZE);

    if (ptr == NULL)
        printf("MALLOC FAILED\n");

    return ptr;
}

static inline void
ag7100_free_pkt(void *m)
{
    printk("ag7100_free_pkt\n");
    free(m);
}

#endif

static void
ag7100_rings_clean(athr_gmac_t * mac)
{
    int i;
    ag7100_desc_t *rxdesc = mac->rxdesc;
    ag7100_desc_t *rxdesc_tmp;
    struct mbuf *m;

    CONSOLE_PRINTF("begin");
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_RX_DESC, (cyg_uint32) NULL);
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC_Q0, (cyg_uint32) NULL);

    for (i = 0, rxdesc_tmp =
         (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(rxdesc);
         i < AG7100_RX_DESC_CNT; i++, rxdesc_tmp++) {

        if (rxdesc_tmp->packet) {
            m = rxdesc_tmp->packet;
            ag7100_free_pkt(m);
            rxdesc_tmp->packet = NULL;
        }
    }
#if 1
    {
        ag7100_desc_t *txdesc;
        ag7100_desc_t *txdesc_tmp;
        // remove tx ring
        /*
         * free Tx descriptors 
         */
        txdesc = mac->txdesc;
        for (i = 0, txdesc_tmp =
             (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(txdesc);
             i < (AG7100_TX_DESC_CNT); i++, txdesc_tmp++) {

            if (txdesc_tmp->packet) {
                m = txdesc_tmp->packet;
                ag7100_free_pkt(m);
                txdesc_tmp->packet = NULL;
            }
        }
    }
#endif
    CONSOLE_PRINTF("end");
}

static int
ag7100_rings_init(athr_gmac_t * mac)
{
    int i;
    struct mbuf *m;

	diag_printf ("####### RINGS_INIT ##############\n"); 
    ag7100_ring_t *txring = &mac->txring;
    ag7100_ring_t *rxring = &mac->rxring;
    ag7100_desc_t *txdesc = mac->txdesc;
    ag7100_desc_t *rxdesc = mac->rxdesc;
    ag7100_desc_t *txdesc_tmp;
    ag7100_desc_t *rxdesc_tmp;

    CONSOLE_PRINTF("begin");
    bzero(txdesc, sizeof(mac->txdesc));
    bzero(rxdesc, sizeof(mac->rxdesc));
    bzero(txring, sizeof(*txring));
    bzero(rxring, sizeof(*rxring));

    /*
     * Init Tx descriptors 
     */
    for (i = 0, txdesc_tmp =
         (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(txdesc);
         i < AG7100_TX_DESC_CNT - 1; i++, txdesc_tmp++) {
        txdesc_tmp->next_desc = virt_to_bus(txdesc_tmp + 1);
        txdesc_tmp->is_empty = 1;
        if ((m = txdesc_tmp->packet = ag7100_alloc_pkt()) == NULL) {
            diag_printf("%s:%d:no buffer i:%d\n", __func__, __LINE__, i);
            goto fail;
        }
        txdesc_tmp->pkt_start_addr =
            virt_to_bus(ag7100_get_pkt_buf(txdesc_tmp->packet));
    }
    txdesc_tmp->next_desc = virt_to_bus(txdesc);
    txdesc_tmp->is_empty = 1;
    if ((m = txdesc_tmp->packet = ag7100_alloc_pkt()) == NULL) {
        diag_printf("%s:%d:no buffer i:%d\n", __func__, __LINE__, i);
        goto fail;
    }
    txdesc_tmp->pkt_start_addr =
        virt_to_bus(ag7100_get_pkt_buf(txdesc_tmp->packet));

    /*
     * Init Rx descriptors and allocate packet buf 
     */
    for (i = 0, rxdesc_tmp =
         (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(rxdesc);
         i < AG7100_RX_DESC_CNT - 1; i++, rxdesc_tmp++) {
        rxdesc_tmp->next_desc = virt_to_bus(rxdesc_tmp + 1);
        rxdesc_tmp->is_empty = 1;
        if ((m = rxdesc_tmp->packet = ag7100_alloc_pkt()) == NULL) {
            diag_printf("%s:%d:no buffer i:%d\n", __func__, __LINE__, i);
            goto fail;
        }
        rxdesc_tmp->pkt_start_addr =
            virt_to_bus(ag7100_get_pkt_buf(rxdesc_tmp->packet));
    }
    rxdesc_tmp->next_desc = virt_to_bus(rxdesc);
    rxdesc_tmp->is_empty = 1;
    if ((m = rxdesc_tmp->packet = ag7100_alloc_pkt()) == NULL) {
        diag_printf("%s:%d:no buffer i:%d\n", __func__, __LINE__, i);
        goto fail;
    }
    rxdesc_tmp->pkt_start_addr =
        virt_to_bus(ag7100_get_pkt_buf(rxdesc_tmp->packet));

    /*
     * Setup Tx Ring 
     */
    txring->desc = (ag7100_desc_t *) txdesc;
    txring->used_head = 0;
    txring->free_head = 0;
    txring->size = 0;

    /*
     * Setup Rx Ring 
     */
    rxring->desc = rxdesc;
    rxring->used_head = 0;
    rxring->free_head = 0;
    rxring->size = ATHR_GMAC_DMA_RX_DESC;

    /*
     * TODO flush cache into memory 
     */
    A_DATA_CACHE_FLUSH((cyg_uint32) txdesc, sizeof(*txdesc));
    A_DATA_CACHE_FLUSH((cyg_uint32) rxdesc, sizeof(*rxdesc));

    /*
     * Write Rx/TX descriptor to register 
     */
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_RX_DESC, virt_to_bus(&rxring->desc[0]));
    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_TX_DESC, virt_to_bus(&txring->desc[0]));

    CONSOLE_PRINTF("end");
    return 0;

fail:
    for (i = 0, rxdesc_tmp = rxdesc; i < AG7100_RX_DESC_CNT; i++, rxdesc_tmp++) {
        if (rxdesc_tmp->packet) {
            m = rxdesc_tmp->packet;
            ag7100_free_pkt(rxdesc_tmp->packet);
            rxdesc_tmp->packet = NULL;
        }
    }

    CONSOLE_PRINTF("end -1");
    return -1;
}

char *mii_str[2][4] = {
    {"GMii", "Mii", "RGMii", "RMii"},
    {"RGMii", "RMii", "INVL1", "INVL2"}
};

void ag7100_get_stats(struct ifnet *ifp, void *ifr);

static cyg_uint32
athr_gmac_link_intr(cyg_vector_t vector, cyg_addrword_t data)
{
    athr_gmac_t *mac = (athr_gmac_t *) data;

    CONSOLE_PRINTF("link interrupt is triggered");
    mac->phy->link_isr(0);
    return CYG_ISR_HANDLED;
}

/*
 * Standard eCos initialization routine.  Call the upper-level ethernet
 * driver and let it know our MAC address.
 */
struct eth_drv_sc *SC = NULL;
static bool
ag7100_init(struct cyg_netdevtab_entry *tab)
{
    unsigned char enaddr[ETHER_ADDR_LEN];
    struct eth_drv_sc *sc = (struct eth_drv_sc *)tab->device_instance;
					   SC = (struct eth_drv_sc *)tab->device_instance;
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    // struct ifnet *ifp = &sc->sc_arpcom.ac_if;
    int unit;
    unsigned int mask;
    int i = 0;

    ARRIVE();

    CONSOLE_PRINTF("begin");
    unit = mac->mac_unit;
    athr_gmacs[unit] = mac;
    mac->sc = sc;

    mac->ops = &mac->ops_func;
    mac->phy = &mac->phy_func;

    athr_gmac_attach(mac);

    mac->ops->set_caps(mac);

    mac->mac_noacs = 1;
    mac->no_of_macs = 2;

    if (mac->mac_unit == 0) {
        // For MAC1
        mac->mii_intf = CFG_ATHR_GMAC1_MII;
        mac->mii_phy = CFG_ATHR_GMAC1_MII_PHY;
    } else {
        // For MAC0
        mac->mii_intf = CFG_ATHR_GMAC0_MII;
        mac->mii_phy = CFG_ATHR_GMAC0_MII_PHY;
    }

    athr_gmac_phy_attach(mac, mac->mac_unit);

    if (!mac->phy) {
        CONSOLE_PRINTF("MAC:%d Warning: PHY not found!!!", mac->mac_unit);
    }

    /*
     * Get ethernet's MAC address from board configuration data 
     */
    ag7100_get_macaddr(mac, enaddr);

    for (i = 0; i < ATHR_GMAC_NMACS; i++) {

        athr_gmac_reg_rmw_set(athr_gmacs[i], MAC_CONFIGURATION_1_ADDRESS,
                              (MAC_CONFIGURATION_1_SOFT_RESET_SET(1) |
                               MAC_CONFIGURATION_1_RESET_RX_MAC_SET(1) |
                               MAC_CONFIGURATION_1_RESET_TX_MAC_SET(1)));

        HAL_DELAY_US(20);

        if (i) {
            mask =
                (RST_RESET_GE1_MDIO_RESET_SET(1) |
                 RST_RESET_GE1_MAC_RESET_SET(1));
        } else {
            mask =
                (RST_RESET_GE0_MDIO_RESET_SET(1) |
                 RST_RESET_GE0_MAC_RESET_SET(1));
        }

        /*
         * put into reset, hold, pull out.
         */

        athr_reg_rmw_set(RST_RESET_ADDRESS, mask);
        HAL_DELAY_US(100 * 1000);
        athr_reg_rmw_clear(RST_RESET_ADDRESS, mask);
        /*
         * Increase the delay for MDIO and MAC reset to settle down.
         */
        HAL_DELAY_US(100 * 1000);

        athr_gmac_reg_rmw_clear(athr_gmacs[i], MAC_CONFIGURATION_1_ADDRESS,
                                MAC_CONFIGURATION_1_SOFT_RESET_SET(1));

    }

    cyg_drv_interrupt_create(mac->vector,
                             0,
                             (CYG_ADDRWORD) sc,
                             ag7100_isr,
                             eth_drv_dsr, &mac->interrupt_handle,
                             &mac->interrupt);

    cyg_drv_interrupt_create(QCA_MISC_IRQ_ETHSW,
                             0,
                             (CYG_ADDRWORD) mac,
                             athr_gmac_link_intr,
                             NULL, &link_interrupt_handle,
                             &link_interrupt_object);
    cyg_drv_interrupt_attach(link_interrupt_handle);
    cyg_drv_interrupt_unmask(QCA_MISC_IRQ_ETHSW);


	diag_printf ("#$%#%#$%#$$#%#%#$# address is %x\n", (sc->funs->eth_drv->init));
    (sc->funs->eth_drv->init) (sc, enaddr);

    LEAVE();
    CONSOLE_PRINTF("end");

    return true;
}

static void
ag7100_start(struct eth_drv_sc *sc, unsigned char *enet_addr, int flags)
{

	diag_printf ("################## AG7100_START ################\n");
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    athr_phy_ops_t *phy = mac->phy;
    unsigned int mask;
    unsigned int w1, w2,i;

	ARRIVE();
	if (mac->mac_ifup == 2) {        
#if 0
	athr_gmac_attach(mac);

	athr_gmac_phy_attach(mac, mac->mac_unit);

	diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
	diag_printf (" ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");


			athr_gmac_reg_rmw_set(athr_gmacs[i], MAC_CONFIGURATION_1_ADDRESS,
							(MAC_CONFIGURATION_1_SOFT_RESET_SET(1) |
							 MAC_CONFIGURATION_1_RESET_RX_MAC_SET(1) |
							 MAC_CONFIGURATION_1_RESET_TX_MAC_SET(1)));

			HAL_DELAY_US(20);

			if (i) {
					mask =
							(RST_RESET_GE1_MDIO_RESET_SET(1) |
							 RST_RESET_GE1_MAC_RESET_SET(1));
			} else {
					mask =
							(RST_RESET_GE0_MDIO_RESET_SET(1) |
							 RST_RESET_GE0_MAC_RESET_SET(1));
			}

			/*
			 * put into reset, hold, pull out.
			 */

			athr_reg_rmw_set(RST_RESET_ADDRESS, mask);
			HAL_DELAY_US(100 * 1000);
			athr_reg_rmw_clear(RST_RESET_ADDRESS, mask);
			/*
			 * Increase the delay for MDIO and MAC reset to settle down.
			 */
			HAL_DELAY_US(100 * 1000);

			athr_gmac_reg_rmw_clear(athr_gmacs[i], MAC_CONFIGURATION_1_ADDRESS,
							MAC_CONFIGURATION_1_SOFT_RESET_SET(1));
#endif 
	}
    if (mac->mac_ifup == 1) {
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
diag_printf (" ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\n");
        mac->phy->link_isr(0);
        return;
    }

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 

    /*
     * disable the switch ports
     */
    if (phy->reg_config) {
        /*
         * Added for Honybee 
         */
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
        if (is_s27()) {
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
            phy->reg_config(ATHR_GMAC_PORT_OFF, ENET_UNIT_LAN); /* LAN ports */
            phy->reg_config(ATHR_GMAC_PORT_OFF, ENET_UNIT_WAN); /* WAN port */
        } else {
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
            if ((mac_has_flag(mac, ATHR_S17_MAC0_SGMII))) {
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
                phy->reg_config(ATHR_GMAC_PORT_OFF,
                                ((mac->mac_unit ==
                                  0) ? ENET_UNIT_WAN : ENET_UNIT_LAN));
            } else {
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
                phy->reg_config(ATHR_GMAC_PORT_OFF,
                                ((mac->mac_unit ==
                                  1) ? ENET_UNIT_WAN : ENET_UNIT_LAN));
            }
        }                       /* end of is_s27 */
    }
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 

    if (mac->mac_unit == 1)
        mask = RST_RESET_GE1_MDIO_RESET_SET(1) | RST_RESET_GE1_MAC_RESET_SET(1);
    else
        mask = RST_RESET_GE0_MDIO_RESET_SET(1) | RST_RESET_GE0_MAC_RESET_SET(1);

    athr_reg_rmw_set(RST_RESET_ADDRESS, mask);
    HAL_DELAY_US(100 * 1000);
    set_gmac_delay();
    athr_reg_rmw_clear(RST_RESET_ADDRESS, mask);
    HAL_DELAY_US(100 * 1000);

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 

    /*
     * When the mac is taken out of reset it
     * will put gmac modules in reset.
     * clr the soft reset in mac to bring out
     * of reset.
     */

    /*
     * Attach interrupt handler 
     */

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__);  
    cyg_drv_interrupt_attach(mac->interrupt_handle);
    cyg_drv_interrupt_acknowledge(mac->vector);
    cyg_drv_interrupt_unmask(mac->vector);

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 

	if (mac->mac_ifup != 2)

    ag7100_rings_init(mac);

    athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1,
                            MAC_CONFIGURATION_1_SOFT_RESET_SET(1));

    if (mac->ops->soc_gmac_mii_setup)
        mac->ops->soc_gmac_mii_setup(mac);

    phy->init(mac);

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 

    if (mac->ops->soc_gmac_hw_setup)
        mac->ops->soc_gmac_hw_setup(mac);

    // mac->mac_speed = ATHR_PHY_SPEED_UNKNOWN;

    CONSOLE_PRINTF("Setting PHY...\n");
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 

    phy_in_reset = 1;
    phy->setup(mac);
    phy_in_reset = 0;

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
    if ((mac->mii_intf == ATHR_SGMII)
        && !mac_has_flag(mac, ATHR_SGMII_FORCED)) {
        athr_reg_wr(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_AN_ENABLE_SET(1));
    }

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
    if (mac_has_flag(mac, ETH_LINK_INTERRUPT)) {
        mac->phy->en_link_intrs(mac);
    }

    /*
     * set the mac addr
     */
diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
    addr_to_words(enet_addr, w1, w2);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR1, w1);
    athr_gmac_reg_wr(mac, ATHR_GMAC_GE_MAC_ADDR2, w2);

    athr_gmac_reg_wr(mac, ATHR_GMAC_DMA_INTR, 0);
    athr_gmac_int_enable(mac);
    athr_gmac_rx_start(mac);

    mac->mac_ifup = 1;

diag_printf (" __UMA__ %s %s %d\n",__FILE__,__func__,__LINE__); 
    athr_dump_gmac_reg(mac->mac_unit);
    LEAVE();
}

static void
athr_gmac_hw_stop(athr_gmac_t * mac)
{
    //athr_gmac_rx_stop(mac);
    //athr_gmac_tx_stop(mac);
    //athr_gmac_int_disable(mac);

    /*
     * IF SGMII link interrupt is enabled
     * disable and clear the status.
     */

    if (mac->mii_intf == ATHR_SGMII) {
        if (mac_has_flag(mac, ETH_LINK_INTERRUPT)) {
            athr_reg_wr(SGMII_INTERRUPT_ADDRESS, 0x0);
            athr_reg_wr(SGMII_INTERRUPT_MASK_ADDRESS, 0x0);
        }
    } else {
        /*
         * Honybee link interupt disable 
         */
        //mac->phy->dis_link_intrs(mac);
    }

    /*
     * During interface up on PHY reset the link status will be updated.
     * Clearing the software link status while bringing the interface down
     * will avoid the race condition during PHY RESET. 
     */
    /*
     * put everything into reset.
     * Dont Reset WAN MAC as we are using eth0 MDIO to access S26 Registers.
     */

    //athr_gmac_reg_rmw_set(mac, MAC_CONFIGURATION_1_ADDRESS,
    //                      MAC_CONFIGURATION_1_SOFT_RESET_SET(1));
	
	mac->mac_ifup = 2;
}

static void
ag7100_stop(struct eth_drv_sc *sc)
{
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;

	mac->mac_ifup = 0;
	
	mac->mac_ifup = 2;

    //athr_gmac_hw_stop(mac);

    //ag7100_rings_clean(mac);
}

static int
ag7100_control(struct eth_drv_sc *sc,
               unsigned long key, void *data, int data_length)
{
    struct ifnet *ifp = &sc->sc_arpcom.ac_if;
    int error = 0;
    ARRIVE();
    /*
     * TBDXXX 
     */
    switch (key) {
    case ETH_DRV_GET_IF_STATS_UD:
    case ETH_DRV_GET_IF_STATS:
        ag7100_get_stats(ifp, data);
        error = 0;
        break;
    case ETH_DRV_SET_MC_ALL:
    case ETH_DRV_SET_MC_LIST:
        error = 0;
        break;
    default:
		diag_printf (" $$$$$$$$ ERROR $$$$ %s %s %d %x n",__FILE__,__func__,__LINE__,key); 
        error = -1;
        break;
    }
    LEAVE();
    return error;
}

static ag7100_desc_t *ag7100_get_tx_ds
    (athr_gmac_t * mac, int *len, unsigned char **start) {
    ag7100_desc_t *ds;
    int len_this_ds;
    ag7100_ring_t *r = &mac->txring;
    int tx_len_per_ds = 512;    // 2048;

    /*
     * force extra pkt if remainder less than 4 bytes 
     */
    if (*len > tx_len_per_ds) {
        if (*len <= (tx_len_per_ds + 4))
            len_this_ds = tx_len_per_ds - 4;
        else
            len_this_ds = tx_len_per_ds;
    } else
        len_this_ds = *len;

    ds = (ag7100_desc_t *) CYGARC_UNCACHED_ADDRESS(&r->desc[r->free_head]);

    ds->pkt_size = len_this_ds;
    HAL_DCACHE_FLUSH((cyg_uint32) * start, len_this_ds);
    ds->pkt_start_addr = CYGARC_PHYSICAL_ADDRESS(*start);
    ds->more = 1;

    *len -= len_this_ds;
    *start += len_this_ds;

    // ag7100_ring_incr(r->ring_head);
    if (++(r->free_head) >= AG7100_TX_DESC_CNT) {
        r->free_head = 0;
    }
    return ds;
}

static void ag7100_deal_tx(struct eth_drv_sc *sc, athr_gmac_t * mac);

static int
ag7100_if_poll_xmit(struct ifnet *ifp, int *budget)
{
}

static int
ag7100_deal_rx(struct eth_drv_sc *sc, athr_gmac_t * mac, int *quota)
{
}

static void
ag7100_deal_tx(struct eth_drv_sc *sc, athr_gmac_t * mac)
{
}

static int
ag7100_if_poll_recv(struct ifnet *ifp, int *budget)
{
    struct eth_drv_sc *sc = ifp->if_softc;
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    int ret = 0;
    cyg_uint32 isr;

    ARRIVE();
    isr = athr_gmac_get_isr(mac);
    isr &= 0x7fffffff;

    CONSOLE_PRINTF("begin unit:%u mac->intr:0x%x isr:0x%x",
                   mac->mac_unit, mac->interrupt_handle, isr);

    if (isr & (ATHR_GMAC_INTR_RX | ATHR_GMAC_INTR_RX_OVF)) {
        ret = ag7100_deal_rx(sc, mac, budget);
    }

    if (isr & ATHR_GMAC_INTR_TX) {
        ag7100_deal_tx(sc, mac);
    }

    if (isr & ATHR_GMAC_INTR_TX_URN) {  /* TODO */
        // ag7100_intr_ack_txurn(mac);
    }

    if (isr & ATHR_GMAC_INTR_RX_BUS_ERROR) {
        // ag7100_intr_ack_rxbe(mac);
        // ag7100_reg_wr(mac, AG7100_DMA_RX_CTRL, AG7100_RXE);
        diag_printf("%s: rx error\n", __func__);
    }

    if (isr & ATHR_GMAC_INTR_TX_BUS_ERROR) {
        // ag7100_intr_ack_txbe(mac);
        // ag7100_reg_wr(mac, AG7100_DMA_TX_CTRL, AG7100_TXE);
        diag_printf("%s: tx error\n", __func__);
    }

    cyg_interrupt_unmask(mac->vector);

    CONSOLE_PRINTF("end");
    LEAVE();

    return ret;
}

static int
ag7100_int_vector(struct eth_drv_sc *sc)
{
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;
    return mac->vector;
}

static void
print_status_cause_register(void)
{
    cyg_uint32 status;
    cyg_uint32 cause;

    asm volatile ("mfc0   %0,$12\n" "nop; nop; nop\n":"=r" (status)
                  :);

    asm volatile ("mfc0   %0,$13\n" "nop; nop; nop\n":"=r" (cause)
                  :);

    diag_printf("%s:%d: status:0x%x cause:0x%x\n", __func__, __LINE__,
                status, cause);
}

#define __read_32bit_c0_register(source, sel)                                                   \
({ unsigned int __res;                                                                                                     \
                if (sel == 0)                                                                                                          \
                                __asm__ __volatile__(                                                                  \
                                                "mfc0\t%0, " #source "\n\t"                                        \
                                                : "=r" (__res));                                                   \
                else                                                                                                                        \
                                __asm__ __volatile__(                                                                  \
                                                ".set\tmips32\n\t"                                                          \
                                                "mfc0\t%0, " #source ", " #sel "\n\t"                        \
                                                ".set\tmips0\n\t"                                                            \
                                                : "=r" (__res));                                                   \
                __res;                                                                                                                   \
})

#define __write_32bit_c0_register(register, sel, value)                                   \
do {                                                                                                                                        \
                if (sel == 0)                                                                                                          \
                                __asm__ __volatile__(                                                                  \
                                                "mtc0\t%z0, " #register "\n\t"                                    \
                                                : : "Jr" ((unsigned int)(value)));                   \
                else                                                                                                                        \
                                __asm__ __volatile__(                                                                  \
                                                ".set\tmips32\n\t"                                                          \
                                                "mtc0\t%z0, " #register ", " #sel "\n\t"   \
                                                ".set\tmips0"                                                                     \
                                                : : "Jr" ((unsigned int)(value)));                   \
} while (0)

static void
status_mask(int vector)
{
    unsigned int cp0_reg;
    cp0_reg = __read_32bit_c0_register($12, 0);
    cp0_reg &= ~(1 << (vector + 10));
    __write_32bit_c0_register($12, 0, cp0_reg);
}

static cyg_uint32
ag7100_isr(cyg_vector_t vector, cyg_addrword_t data)
{
    unsigned long isr;
    cyg_uint32 retval = 0;
    struct eth_drv_sc *sc = (struct eth_drv_sc *)data;
    athr_gmac_t *mac = (athr_gmac_t *) sc->driver_private;

    // get isr must be before the int disable, when disalbe intr, it will
    // also clear the isr
    mac->isr = athr_gmac_get_isr(mac);
    mac->isr &= 0x7fffffff;

    athr_gmac_int_disable(mac);

    if (mac->isr & ATHR_GMAC_INTR_RX) {
        CONSOLE_PRINTF("In RX interrupt");
        retval |= CYG_ISR_HANDLED | CYG_ISR_CALL_DSR;
    } else if (mac->isr & ATHR_GMAC_INTR_TX) {
        retval |= CYG_ISR_HANDLED | CYG_ISR_CALL_DSR;
        CONSOLE_PRINTF("In TX interrupt");
    } else {
        retval |= CYG_ISR_HANDLED;
        CONSOLE_PRINTF("Other interrupt %lx\n", isr);
    }

    cyg_interrupt_acknowledge(vector);
    return retval;
}

void
ag7100_get_macaddr(athr_gmac_t * mac, cyg_uint8 * mac_addr)
{
    mac_addr[0] = 0x00;
    mac_addr[1] = 0x03;
    mac_addr[2] = 0x7F;
    mac_addr[3] = 0xFF;
    mac_addr[4] = 0xFF;
    mac_addr[5] = 0xFF - mac->mac_unit;
}

void
ag7100_get_stats(struct ifnet *ifp, void *ifr)
{
    return;
}

unsigned int
athr_gmac_get_link_st(athr_gmac_t * mac, int *link, int *fdx,
                      athr_phy_speed_t * speed, int phyUnit)
{
    athr_phy_ops_t *phy = mac->phy;

    if (phy) {
        *fdx = phy->is_fdx(mac->mac_unit, phyUnit);
        *speed = phy->speed(mac->mac_unit, phyUnit);
        *link = phy->is_up(mac->mac_unit);
    } else {
        diag_printf("MAC:%d Warning: Phy not found!!!\n", mac->mac_unit);
    }

    return 0;
}

void
athr_gmac_fast_reset(athr_gmac_t * mac, athr_gmac_desc_t * ds, int ac)
{
}

uint16_t
athr_gmac_mii_read(int unit, uint32_t phy_addr, uint8_t reg)
{
    athr_gmac_t *mac = athr_gmac_unit2mac(unit);
    uint16_t addr = (phy_addr << ATHR_GMAC_ADDR_SHIFT) | reg, val = 0;
    volatile int rddata;
    uint16_t ii = 0x1000;

    /*
     * Check for previous transactions are complete. Added to avoid
     * race condition while running at higher frequencies.
     */
    do {
        udelay(5);
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    } while (rddata && --ii);

    if (ii == 0)
        diag_printf("ERROR:%s:%d transaction failed\n", __func__, __LINE__);

    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_ADDRESS, addr);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, ATHR_GMAC_MGMT_CMD_READ);

    do {
        udelay(5);
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    } while (rddata && --ii);

    if (ii == 0)
        diag_printf("ERROR:%s:%d transaction failed\n", __func__, __LINE__);

    val = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_STATUS);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);

    return val;
}

void
athr_gmac_mii_write(int unit, uint32_t phy_addr, uint8_t reg, uint16_t data)
{
    athr_gmac_t *mac = athr_gmac_unit2mac(unit);
    uint16_t addr = (phy_addr << ATHR_GMAC_ADDR_SHIFT) | reg;
    volatile int rddata;
    uint16_t ii = 0x1000;

    /*
     * Check for previous transactions are complete. Added to avoid
     * race condition while running at higher frequencies.
     */
    do {
        udelay(5);
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    } while (rddata && --ii);

    if (ii == 0)
        printk("ERROR:%s:%d transaction failed\n", __func__, __LINE__);

    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CMD, 0x0);

    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_ADDRESS, addr);
    athr_gmac_reg_wr(mac, ATHR_GMAC_MII_MGMT_CTRL, data);

    do {
        rddata = athr_gmac_reg_rd(mac, ATHR_GMAC_MII_MGMT_IND) & 0x1;
    } while (rddata && --ii);

    if (ii == 0)
        diag_printf("ERROR:%s:%d transaction failed\n", __func__, __LINE__);
}

/************ Shell command ********************/
void
ag7100_poll_link(void)
{
    athr_gmac_t *mac = athr_gmac_unit2mac(0);
    mac->phy->link_isr(0);
}

void
athr_dump_gmac_reg(int cmd)
{
    int i = 0;
    athr_gmac_t *mac = athr_gmac_unit2mac(cmd);
    diag_printf("Dumping mac%d GMAC reg\n", mac->mac_unit);
    for (i = 0; i < 0x140; i += 4) {
        diag_printf("reg 0x%x: 0x%x\n", i, athr_gmac_reg_rd(mac, i));
    }

    for (i = 0x1d8; i > 0x180; i -= 4)
        diag_printf("reg 0x%x: 0x%x\n", i, athr_gmac_reg_rd(mac, i));

}

/*
 * Ethernet Unit 0 
 */
static athr_gmac_t ag7100_priv_data0 = {
    .mac_unit = 0,
    .mac_base = ATH_GE0_BASE,
    .vector = 2,
};

ETH_DRV_SC(ag7100_sc0,
           &ag7100_priv_data0,
           "eth0",
           ag7100_start,
           ag7100_stop,
           ag7100_control,
           ag7100_can_send,
           ag7100_send,
           ag7100_recv, ag7100_deliver, ag7100_poll, ag7100_int_vector);

NETDEVTAB_ENTRY(ag7100_netdev0, "ag7100_eth0", ag7100_init, &ag7100_sc0);

/*
 * Ethernet Unit 1 
 */
static athr_gmac_t ag7100_priv_data1 = {
    .mac_unit = 1,
    .mac_base = ATH_GE1_BASE,
    .vector = 3,
};

ETH_DRV_SC(ag7100_sc1,
           &ag7100_priv_data1,
           "eth1",
           ag7100_start,
           ag7100_stop,
           ag7100_control,
           ag7100_can_send,
           ag7100_send,
           ag7100_recv, ag7100_deliver, ag7100_poll, ag7100_int_vector);

NETDEVTAB_ENTRY(ag7100_netdev1, "ag7100_eth1", ag7100_init, &ag7100_sc1);
