#include <linux/module.h>
#include <linux/pci.h>
#include <linux/errno.h>
#include <linux/init.h>

static int __init my_init(void)
{
	u16 dval;
	char byte;
	int j = 0;
	struct pci_dev *pdev = NULL;

	printk(KERN_INFO "LOADING THE PCI_DEVICE_FINDER\n");

	/* either of the following looping constructs will work */

	for_each_pci_dev(pdev) {

		/*    while ((pdev = pci_get_device
		   (PCI_ANY_ID, PCI_ANY_ID, pdev))) { */

		printk(KERN_INFO "\nFOUND PCI DEVICE # j = %d, ", j++);
		printk(KERN_INFO "READING CONFIGURATION REGISTER:\n");

		printk(KERN_INFO "Bus,Device,Function=%s", pci_name(pdev));

		pci_read_config_word(pdev, PCI_VENDOR_ID, &dval);
		printk(KERN_INFO " PCI_VENDOR_ID=%x", dval);

		pci_read_config_word(pdev, PCI_DEVICE_ID, &dval);
		printk(KERN_INFO " PCI_DEVICE_ID=%x", dval);

		pci_read_config_byte(pdev, PCI_REVISION_ID, &byte);
		printk(KERN_INFO " PCI_REVISION_ID=%d", byte);

		pci_read_config_byte(pdev, PCI_INTERRUPT_LINE, &byte);
		printk(KERN_INFO " PCI_INTERRUPT_LINE=%d", byte);

		pci_read_config_byte(pdev, PCI_LATENCY_TIMER, &byte);
		printk(KERN_INFO " PCI_LATENCY_TIMER=%d", byte);

		pci_read_config_word(pdev, PCI_COMMAND, &dval);
		printk(KERN_INFO " PCI_COMMAND=%d\n", dval);

		/* decrement the reference count and release */
		pci_dev_put(pdev);

	}
	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "UNLOADING THE PCI DEVICE FINDER\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LF331:1.6 s_20/lab2_pci.c");
MODULE_LICENSE("GPL v2");
