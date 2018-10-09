#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/pci.h>

static const struct pci_device_id our_pci_table[] = {
    { PCI_DEVICE(PCI_VENDOR_ID_INTEL, DT3155_DEVICE_ID) },
    { 0, /* zero marks the end */ },
};


static struct pci_driver ourfb_driver = {

	.name = "ourfb",
	.id_table = our_pci_table,
	.probe = our_pci_init,
	.remove = our_pci_remove,
}

static int __init ourfb_init(void)
{
		return pci_register_driver(&ourfb_driver);
}

static void __exit ourfb_exit(void)
{
		pci_unregister_driver(&ourfb_driver);
}

module_init(ourfb_init);
module_exit(ourfb_exit);

MODULE_LICENSE("GPL");
