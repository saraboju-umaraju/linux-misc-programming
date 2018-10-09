/*
 * Program demonstrating modules
*/

#include <linux/module.h>

int init_module()                  //module entry point
{
  printk("Module Initialized\n");  //kernel print function
  return 0;
}

void cleanup_module()              //clean up
{
  printk("Module removed\n");
}

MODULE_LICENSE("GPL");
