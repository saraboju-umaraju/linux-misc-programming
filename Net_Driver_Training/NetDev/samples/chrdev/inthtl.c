/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                                                                      *
* ModuleName         :  inthtl                                         *
* SourceName         :  inthtl.c                                       *
*                                                                      *
* Description        :  schedule a tasklet from interrupt service      *
*                                                                      *
* Programmer         :---------                                        *
* Date               : 2/8/2002                                        *
*                                                                      *
*----------------------------------------------------------------------*
* Notes:                                                               *
*      1. For compiling, Type make                                     *
*      2. For loading,   Type insmod inthtl.ko                         *
*      3. For unloading, Type rmmod inthtl                             *
*                                                                      *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*----------------------------------------------------------------------
 ************************** Define ********************************
 *--------------------------------------------------------------------*/

#define  MY_MAJOR       250
#define  LPTBase  	0x378
#define  IRQ_LINE 	7

/*----------------------------------------------------------------------
 ************************** Include File *******************************
 *--------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/interrupt.h>

/*--------------------------------------------------------------------*/

int major;

struct file_operations my_fops;

/* Set up our tasklet if we're doing that. */
void short_do_tasklet (unsigned long);
DECLARE_TASKLET (short_tasklet, short_do_tasklet, 0);
 
void short_do_tasklet (unsigned long unused)
{
    printk("Message from Tasklet\n");
}

int interrupt_handler(int i,void *v, struct pt_regs *pt_reg)
{
    outb(0x00, LPTBase);		//clear interrupting bit
    tasklet_schedule(&short_tasklet);
    printk ("In Interrupt Handler\n");
    return IRQ_HANDLED;
}

int my_init(void)
{
    int sts;

    printk("\n==== <1>Hello World ====\n\n");
    major = register_chrdev(MY_MAJOR,"mydevice",&my_fops);

    if (-EBUSY == check_region(LPTBase, 4)){
        printk ("failed to allocate i/o ports\n");
        return -1;
    }
    request_region(LPTBase, 4, "RTL-IRQ");

    sts = request_irq(IRQ_LINE,interrupt_handler,SA_INTERRUPT,"mydevice",NULL);
    if (sts < 0){
              printk("\n=== <1> PANIC !!! NOT ABLE TO GET IRQ ===\n\n");
    }
    outb(0x10, LPTBase+2); /* IRQ Enable, set bit 4 of base+2 */

    return 0;
}

void my_cleanup(void)
{
    int i;

    outb(0xff, LPTBase);		//cause interrupt
    printk ("clean up module\n");
    for (i = 0; i < 1000000; i++)
	;
    printk("\n==== <1> Goodbye ====\n\n");
    free_irq(IRQ_LINE,NULL);
    release_region(LPTBase, 4);
    unregister_chrdev(MY_MAJOR,"mydevice");
}

module_init(my_init);
module_exit(my_cleanup);

MODULE_LICENSE("GPL");

