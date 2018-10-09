/*
 * find and extract PCI configuration and read MAC address for NIC
 * based on intel 82540em - pci based driver
 * B R Sahu, 09/05/11
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/io.h>
//#include <asm/io.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>     /* For the statistics structure. */

#define DRV_NAME "i82540net"

/* PCI configuration space */

#define VENDOR_ID               0x10ec 
#define DEVICE_ID               0x8168

/* Device 82540 Registers */
#define RAL             0x5400          /* Receive Address Low Register */
#define RAH             0x5404          /* Receive Address High Register */


#define IOADDR		0x00		/* I/O mapped: Address Register */
#define IODATA		0x04		/* I/O mapped: Data Register */

static struct net_device_stats *pcnet32_get_stats(struct net_device *);
unsigned char dev_addr[8];
struct net_device *dev;
struct net_device_stats stats = {0};


unsigned long IOBase, IOBaseEnd;
unsigned long ulMemoryBase, ulMemoryBaseEnd;
unsigned long ulResFlags;

	
/* Read MAC address through I/O Registers */
void ReadMACAddrIORegisters(void)
{
  unsigned int status;
  int i;

  printk("\n*** MAC Address thru I/O Mapped Registers ***\n");

  outl(RAL, (long *)(IOBase + IOADDR));
  status = inl((long *)(IOBase + IODATA));
  printk("status = %08x\n", status);
  *(long *)dev_addr = status;
  outl(RAH, (long *)(IOBase + IOADDR));
  status = inl((long *)(IOBase + IODATA));
  printk("status = %08x\n", status);
  *(long *)(dev_addr+4) = status;
  for (i = 0; i < 6; i++)
    printk("%02x ", dev_addr[i]);
 
}

/* Read MAC address thru Memory Mapped I/O Registers */
void ReadMACAddrMemRegisters(void)
{
  unsigned int status;
  unsigned long MemBase;
  int i;

  printk("\n*** MAC Address thru Mem Mapped Registers ***\n");

  MemBase = (unsigned long)ioremap(ulMemoryBase, 0x20000);

  /* read MAC address */
  status = readl((long *)(MemBase + RAL));
  printk("status = %08x\n", status);
  *(long *)dev_addr = status;
  status = readl((long *)(MemBase + RAH));
  printk("status = %08x\n", status);
  *(long *)(dev_addr+4) = status;
  for (i = 0; i < 6; i++)
    printk("%02x ", dev_addr[i]);

  iounmap((void *)MemBase);

}

/* read the configuration space and extract BARs */
int get_card_details(struct pci_dev *pdev)
{
	unsigned char cIntLine;
	int i = 0;

	printk("\n*** PCI Device Registers ***\n");

	ulMemoryBase = pci_resource_start( (struct pci_dev *)pdev, 0);
	ulMemoryBaseEnd = pci_resource_end( (struct pci_dev *)pdev, 0);

	ulResFlags = pci_resource_flags( (struct pci_dev *)pdev, 0);

	if (ulResFlags & IORESOURCE_MEM) 
		printk("BAR%d Memory Mapped Base Address is: %08lx End %08lx\n", i, ulMemoryBase, ulMemoryBaseEnd);



  IOBase = pci_resource_start( (struct pci_dev *)pdev, 2);

  IOBaseEnd = pci_resource_end( (struct pci_dev *)pdev, 2);

  ulResFlags = pci_resource_flags( (struct pci_dev *)pdev, 2);

  if (ulResFlags & IORESOURCE_IO)
      printk("BAR2 I/O Mapped Base Address is: %08lx End %08lx\n",
			IOBase, IOBaseEnd);

  pci_read_config_byte( (struct pci_dev *)pdev,
				PCI_INTERRUPT_LINE, &cIntLine);
  printk("IRQ Used is: %02x \n", cIntLine);

  return 0;
}

int pcnet32_open(struct net_device *dev)
{
  printk("open\n");
  netif_start_queue(dev);
  return 0;
}

int pcnet32_close(struct net_device *dev)
{
  printk("stop\n");
  netif_stop_queue(dev);
  return 0;
}

int pcnet32_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
  int i;

  printk("xmit\n");
  printk("skb->len=%d\n", skb->len);
  printk("skb->data= ");
  for (i = 0; i < 16; i++)
    printk("%02x ", skb->data[i]);
  printk("\n");
  netif_stop_queue(dev);
  return 0;
}

static struct net_device_stats *pcnet32_get_stats(struct net_device *dev)
{
  printk("stats\n");
//  struct net_device_stats *stats = &dev->stats;
	
  return &stats;
//  return &dev->stats;
}

int pcnet32_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
  printk("ioctl\n");
  return -EOPNOTSUPP;
}

static const struct net_device_ops pcnet32_netdev_ops = {
        .ndo_open               = pcnet32_open,
        .ndo_stop               = pcnet32_close,
        .ndo_get_stats          = pcnet32_get_stats,
        .ndo_start_xmit         = pcnet32_start_xmit,
        .ndo_do_ioctl           = pcnet32_ioctl,
};

int pcnet32_probe1(struct pci_dev *pdev)
{
    int i;
    int rc;

    dev = alloc_etherdev(256);
    memcpy(dev->dev_addr, dev_addr, 6);
    dev->base_addr = IOBase;
    dev->irq = pdev->irq;

    /* fill net device structure and register */
    dev->netdev_ops = &pcnet32_netdev_ops;

    printk("IObase = %04x\n", (unsigned short)dev->base_addr);
    for(i = 0; i < 6; i++)
      printk("MAC Addr[%d] Value = %02x\n", i, dev->dev_addr[i]);
    rc = register_netdev(dev);
    printk("rc= %d name = %s\n", rc, dev->name);

    return 0;
}

int pcnet_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
    printk("PCI probe\n");
    pci_enable_device(pdev);

    get_card_details(pdev);

    ReadMACAddrIORegisters();

//    ReadMACAddrMemRegisters();

    pcnet32_probe1(pdev);

    return 0;
}

void pcnet_remove(struct pci_dev *pdev)
{
    printk("PCI remove\n");

    unregister_netdev(dev);
    free_netdev(dev);
    pci_disable_device(pdev);

}

static struct pci_device_id pcnet_pci_tbl[] = {
	{VENDOR_ID, DEVICE_ID, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{0}
};

struct pci_driver pcnet_driver = {
	name: DRV_NAME,
	id_table: pcnet_pci_tbl,
	probe: pcnet32_probe1,
	remove: pcnet_remove,
};

/* initialization */

int pcnet_init(void)
{
  printk("Module Initialized\n");

  return pci_register_driver(&pcnet_driver);
}

/* cleanup */

void pcnet_exit(void)
{
  printk("Module removed\n");
  pci_unregister_driver(&pcnet_driver);
}

module_init(pcnet_init);
module_exit(pcnet_exit);

MODULE_LICENSE("GPL");

