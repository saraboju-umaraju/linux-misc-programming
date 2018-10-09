/*
 * find and extract PCI configuration for NIC
 * The design is built around struct pci_driver defined in <linux/pci.h>
*/

#include <linux/module.h>
#include <linux/pci.h>

/* PCI configuration space */

#define VENDOR_ID               0x8086 
#define DEVICE_ID               0x100f

unsigned long IOBase, IOBaseEnd;
unsigned long ulMemoryBase, ulMemoryBaseEnd;
unsigned long ulResFlags;

/* read the configuration space and extract BARs */
int get_card_details(struct pci_dev *pdev)
{
    unsigned int ulIOBase;
    unsigned char cIntLine;

    printk("\n*** PCI Device Registers ***\n");

    ulMemoryBase = pci_resource_start( (struct pci_dev *)pdev, 0);

    ulMemoryBaseEnd = pci_resource_end( (struct pci_dev *)pdev, 0);

    ulResFlags = pci_resource_flags( (struct pci_dev *)pdev, 0);

    if (ulResFlags & IORESOURCE_MEM)
      printk("BAR0 Memory Mapped Base Address is: %08lx End %08lx\n",
			ulMemoryBase, ulMemoryBaseEnd);

    IOBase = pci_resource_start( (struct pci_dev *)pdev, 2);

    IOBaseEnd = pci_resource_end( (struct pci_dev *)pdev, 2);

    ulResFlags = pci_resource_flags( (struct pci_dev *)pdev, 2);

    if (ulResFlags & IORESOURCE_IO)
      printk("BAR1 I/O Mapped Base Address is: %08lx End %08lx\n",
			IOBase, IOBaseEnd);

    pci_read_config_byte( (struct pci_dev *)pdev,
				PCI_INTERRUPT_LINE, &cIntLine);
    printk("IRQ Used is: %02x \n", cIntLine);
    return 0;
}

int rtl8139_init(struct pci_dev *pdev, const struct pci_device_id *ent)
{
    printk("PCI probe\n");
    get_card_details(pdev);
    return 0;
}

void rtl8139_exit(struct pci_dev *pdev)
{
    printk("PCI remove\n");
}

#define DRV_NAME "hppm"

static struct pci_device_id rtl8139_pci_tbl[] = {
	{VENDOR_ID, DEVICE_ID, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{0}
};

struct pci_driver hppm_driver = {
	name: DRV_NAME,
	id_table: rtl8139_pci_tbl,
	probe: rtl8139_init,
	remove: rtl8139_exit,
};

/* initialization */

int hppm_init(void)
{
  printk("Module Initialized\n");

  return pci_register_driver(&hppm_driver);
}

/* cleanup */

void hppm_exit(void)
{
  printk("Module removed\n");
  pci_unregister_driver(&hppm_driver);
}

module_init(hppm_init);
module_exit(hppm_exit);

MODULE_LICENSE("GPL");

