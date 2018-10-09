#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>

int init_module( void )
{
	struct pci_dev *dev = NULL;
	for_each_pci_dev(dev){
		if(dev && dev->driver)
			printk("driver %s \n",dev->driver->name);
		if(dev && dev->bus)
			printk("bus name %s ; bus number %d\n",dev->bus->name,dev->bus->number);
		if(dev && dev->slot)
			printk("slot name %s \n",dev->slot->kobj.name);
		if(dev)
			printk("vendor %x \n",dev->vendor);
	}
	return 0;
}


void cleanup_module( void )
{
	return;
}
