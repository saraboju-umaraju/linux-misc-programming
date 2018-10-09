/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                                                                      *
* ModuleName         : serint                                          *
* SourceName         : serint.c                                        *
*                                                                      *
* Description        : serial tx interrrupt handler                    *
*                                                                      *
* Programmer         : B R Sahu                                        *
* Date               : 2/1/2012                                        *
*                                                                      *
*----------------------------------------------------------------------*
* Notes:                                                               *
*      1. For compiling, Type make                                     *
*      2. For loading,   Type insmod serint.ko                         *
*      3. For unloading, Type rmmod serint                             *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define UART_BASE	0x3f8
#define IRQ_LINE 	4

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/serial_reg.h>
#include <asm/io.h>

irqreturn_t interrupt_handler(int i, void *v, struct pt_regs *pt_reg)
{
    printk(KERN_INFO "In Interrupt Handler\n");
    return IRQ_HANDLED;
}

int my_init(void)
{
    int sts;

    printk(KERN_INFO "\nSerial Tx ISR\n");

//    sts = request_irq(IRQ_LINE,interrupt_handler,IRQF_DISABLED,"mydevice",NULL);
    sts = request_irq(IRQ_LINE,interrupt_handler,0,"mydevice",NULL);
    if (sts < 0){
        printk(KERN_ERR "Serial: can not register IRQ %d\n", IRQ_LINE);
        return -EIO;
    }
    outb(UART_IER_THRI, UART_BASE+UART_IER);
    outb(UART_MCR_OUT2, UART_BASE+UART_MCR);

    outb('A', UART_BASE+UART_TX);

    return 0;
}

void my_cleanup(void)
{
    printk(KERN_INFO "\nclean up module\n");
    free_irq(IRQ_LINE, NULL);
}

module_init(my_init);
module_exit(my_cleanup);

MODULE_LICENSE("GPL");

