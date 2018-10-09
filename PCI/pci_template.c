/*

Template for a character device driver for a PCI device,
based on the templates from the book "Linux Treiber entwickeln" and
"Linux Device Drivers".

2 in 1 driver: PCI device driver (incl. ISR) +character device driver.

For a real driver replace _template by the name of your hardware, e. g. _foobar,
and fill in the correct vendor and device id.
Makefile and pci_template.sh have to be edited similar (only one replacment per file).

For the "kernel: pci_template: unsupported module, tainting kernel." kernel
message from loading the Module see
http://www.novell.com/coolsolutions/feature/14910.html

Rolf Freitag 2005

*/
/* This program is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; either version 2 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* This program is distributed in the hope that it will be useful,      */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         */
/* GNU General Public License for more details.                         */
/*                                                                      */
/************************************************************************/
#include <linux/fs.h>
#include <linux/module.h>                         // MOD_DEVICE_TABLE,
#include <linux/init.h>
#include <linux/pci.h>                            // pci_device_id,
#include <linux/interrupt.h>
#include <asm/uaccess.h>                          // copy_to_user,
#include <linux/version.h>                        // KERNEL_VERSION,
#include <iso646.h>
//#include <linux/config.h>
//#include <linux/init.h>

// simple version number
#define _template_SOFTWARE_VERSION_NUMBER 1.0

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)    /* not < 2.5 */
#  error "This kernel is too old: not supported by this file"
#endif

static int i_foo;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rolf Freitag, rolf dot freitag at email.de");
MODULE_DESCRIPTION("PCI  +character device driver template");
module_param(i_foo, int, 0444);                   // Module parameter, unused
MODULE_PARM_DESC(i_foo, "foo frequency");         // unused

// vendor and device id of the PCI device
#define VENDOR_ID 0x0001                          // vendor id: buggy Kolter card: official 0x1001 but real 0x0001
#define DEVICE_ID 0x0017                          // device id of the Proto-3 card

// for first and second i/o region (pci memory):
static unsigned long ioport=0L, iolen=0L, memstart=0L, memlen=0L;
static int i_template_major = 231;

// not really necessary; for future use
MODULE_DEVICE_TABLE(pci, pci_drv_ids);

static struct
pci_device_id pci_drv_ids[] __devinitdata =
{
  // { VENDOR_ID, DEVICE_ID, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0 },
  { PCI_DEVICE(VENDOR_ID, DEVICE_ID), },
  { 0, }
};

// declarations for fops, pci_driver
static int _template_open (struct inode *, struct file *);
static int _template_close (struct inode *, struct file *);
static ssize_t _template_read (struct file *, char *, size_t, loff_t *);
static ssize_t _template_write (struct file *, __user const char *, size_t, loff_t *);
static int device_init(struct pci_dev *, const struct pci_device_id *);
static void device_deinit( struct pci_dev *);
static void device_deinit( struct pci_dev *);
static int __init pci_drv_init(void);
static void __exit pci_drv_exit(void);

module_init(pci_drv_init);
module_exit(pci_drv_exit);

static struct
pci_driver pci_drv_template =
{
  .name= "pci_drv_template",
  .id_table= pci_drv_ids,
  .probe= device_init,
  .remove= device_deinit,
};

static struct file_operations _template_fops =
{
  .owner = THIS_MODULE,
  .read = _template_read,
  .write = _template_write,
  .open = _template_open,
  .release = _template_close,
};

// example for reading a PCI config byte
static unsigned char
get_revision(struct pci_dev *dev)
{
  u8 revision;

  pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
  return (revision);
}


static
irqreturn_t pci_isr( int irq, void *dev_id, struct pt_regs *regs )
{
  // nothing todo for the irq yet
  return (IRQ_HANDLED);
}


// Initialising of the module with output about the irq, I/O region and memory region.
static
int device_init(struct pci_dev *dev, const struct pci_device_id *id)
{
  int i_result;

  i_result = get_revision (dev);                  // example usage to avoid the "defined but not used" message
  // change the device name default (<pci bus number>.<pci slot number>. ...) to the module name
  printk(KERN_ERR "pci_template: Old dev->dev.kobj.name: \"%s\"\n", dev->dev.kobj.name);
  strncpy(dev->dev.kobj.name,"pci_drv_template",KOBJ_NAME_LEN);
  // check irq number (don't request irq 0) before requesting
  if ( (dev->irq) and (request_irq(dev->irq,pci_isr,SA_INTERRUPT|SA_SHIRQ,"pci_drv_template",dev)) )
  {
    printk(KERN_ERR "pci_template: IRQ %d not free.\n", dev->irq );
    return -EIO;
  }
  if (dev->irq)
    printk(KERN_ERR "pci_template: IRQ %d.\n", dev->irq);
  else
    printk(KERN_ERR "pci_template: No irq required/requested.\n");
  // get the first i/o region
  ioport = pci_resource_start( dev, 0 );
  iolen = pci_resource_len( dev, 0 );
  // chech iolen before requesting the I/O region
  if (iolen and ( NULL == request_region( ioport, iolen, dev->dev.kobj.name ) ) )
  {
    printk(KERN_ERR "I/O address conflict for device \"%s\", ioport=%lu=0x%x, iolen=%lu=0x%x\n",
      dev->dev.kobj.name, ioport, (unsigned int)ioport, iolen, (unsigned int)iolen);
    goto cleanup_irq;
  }
  if (iolen)
    printk(KERN_CRIT "pci_template: request_region( ioport=%lu=0x%x, iolen=%lu=0x%x, dev->dev.kobj.name=%s) successfull.\n",
      ioport, (unsigned int)ioport, iolen, (unsigned int)iolen, dev->dev.kobj.name);
  else
    printk(KERN_CRIT "pci_template: request_region( ioport=%lu=0x%x, iolen=%lu=0x%x, dev->dev.kobj.name=%s) not necessary.\n",
      ioport, (unsigned int)ioport, iolen, (unsigned int)iolen, dev->dev.kobj.name);
  memstart = pci_resource_start( dev, 1 );
  memlen = pci_resource_len( dev, 1 );
  printk(KERN_CRIT "pci_template: memstart=%lu=0x%x, memlen=%lu=0x%x\n", memstart, (unsigned int)memstart, memlen, (unsigned int)memlen);
  // check memlen before requesting the memory region
  if (memlen and ( NULL == request_mem_region( memstart, memlen, dev->dev.kobj.name ) ))
  {
    printk(KERN_ERR "Memory address conflict for device \"%s\", request_mem_region( memstart=%lu=0x%x, memlen=%lu=0x%x, dev->dev.kobj.name=%s) failed.\n",
      dev->dev.kobj.name, memstart, (unsigned int)memstart, memlen, (unsigned int)memlen, dev->dev.kobj.name);
    goto cleanup_ports;
  }
  if (memlen)
    printk(KERN_ERR "pci_template: request_mem_region( memstart=%lu=0x%x, memlen=%lu=0x%x, dev->dev.kobj.name=%s) successfull.\n",
      memstart, (unsigned int)memstart, memlen, (unsigned int)memlen, dev->dev.kobj.name);
  else
    printk(KERN_ERR "pci_template: request_mem_region( memstart=%lu=0x%x, memlen=%lu=0x%x, dev->dev.kobj.name=%s) not necessary.\n",
      memstart, (unsigned int)memstart, memlen, (unsigned int)memlen, dev->dev.kobj.name);
  pci_enable_device( dev );
  return (0);
  cleanup_ports:
  if (iolen)
    release_region( ioport, iolen );
  cleanup_irq:
  if (dev->irq)
    free_irq( dev->irq, dev );
  return (-EIO);
}


static void
device_deinit( struct pci_dev *pdev )
{
                                                  // device driver part
  unregister_chrdev (i_template_major, "pci_chrdev_template");
  if (pdev->irq)
    free_irq( pdev->irq, pdev );
  if( iolen )
    release_region( ioport, iolen );
  if( memlen )
    release_mem_region( memstart, memlen );
  return;
}


// device driver init
static
int __init pci_drv_init(void)
{
  int i_result=0;

  printk (KERN_CRIT "pci_chrdev_template: initialising\n");
  if ((i_result = register_chrdev (i_template_major, "pci_chrdev_template", &_template_fops)) < 0)
  {
    // dynamic major number allocation
    i_result = register_chrdev (0, "pci_chrdev_template", &_template_fops);
    i_template_major = i_result;
    if (i_result < 0)
    {
      printk (KERN_CRIT "pci_chrdev_template: Cannot register device.\n");
      return (i_result);
    }
  }
  if( 0 == pci_module_init(&pci_drv_template) )
    return 0;
  unregister_chrdev( i_template_major,"pci_chrdev_template");
  return (-EIO);
}


static
void __exit pci_drv_exit(void)
{
  pci_unregister_driver( &pci_drv_template );
  unregister_chrdev(i_template_major,"pci_chrdev_template");
  return;
}


static int
_template_open (struct inode *inode, struct file *filp)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0))
  MOD_INC_USE_COUNT;
#endif
  return (0);
}


static int
_template_close (struct inode *inode, struct file *filp)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0))
  MOD_DEC_USE_COUNT;
#endif
  return (0);
}


static ssize_t
_template_read (struct file * filp, char *buff, size_t count, loff_t * ppos)
{
  u16 u16_template_dummy = inw(ioport);               // read a word from the card
  // write to user space
  unsigned long ul = copy_to_user (buff, &u16_template_dummy, sizeof(u16_template_dummy));

  return ( ((unsigned long)sizeof(u16_template_dummy)) - ul);
}


static ssize_t
_template_write (struct file * filp, __user const char *buff, size_t count, loff_t * ppos)
{
  u16 u16_template_dummy = 0;
  // read from user space
  unsigned long ul = copy_from_user (&u16_template_dummy, buff, sizeof(u16_template_dummy));

  outw(u16_template_dummy, ioport);                   // write the word to the port
  return ( ((unsigned long)sizeof(u16_template_dummy)) - ul);
}
