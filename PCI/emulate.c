#define MY_PCI_DOMAIN_NUM 0x0001
#define MY_PCI_BUS_NUM 0x00

#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>

int pci_read(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *val){
  return 0;
}
int pci_write(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val){
  return 0;
}

struct pci_ops my_pci_ops = {};

int pci_bus_match(struct device *dev, struct device_driver *drv){
  return 0;
}

struct bus_type pci_bus_type = {
       name:	"pci",
       match:	pci_bus_match,
};

extern struct bus_type pci_bus_type;

MODULE_LICENSE("Dual BSD/GPL");

struct pci_bus my_pci_bus = {};
struct pci_sysdata my_pci_sd;
// 
// memset (&my_pci_sd, 0 sizeof (my_pci_sd));
// my_pci_sd.domain = MY_PCI_DOMAIN_NUM;
// my_pci_sd.node = -1;
// 
int init_module(void){
    struct pci_bus* my_pci_bus;
    
    printk(KERN_ALERT "Hello, world\n");
    memset (&my_pci_ops, 0, sizeof (my_pci_ops));
    my_pci_ops.read = pci_read;
    my_pci_ops.write = pci_write;
    
    my_pci_bus = pci_scan_bus(MY_PCI_BUS_NUM, &my_pci_ops, NULL);
    if(my_pci_bus){
      printk(KERN_INFO "Successfully created MY PCI bus");
    }
    return 0;
}

