/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                      *
* ModuleName         :  iomemory.o                                     *
* SourceName         :  iomemory.c                                     *
*                                                                      *
* Description        : Accessing Physical Memory                       *
*                                                                      *
* Programmer         :---------                                        *
* Date               : 27/4/2006                                       *
*                                                                      *
*----------------------------------------------------------------------*
* Notes:                                                               *
*                                                                      *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
/*----------------------------------------------------------------------
 ************************** Include File *******************************
 *----------------------------------------------------------------------*/
#include <linux/module.h>
#include <asm/io.h>

char *ptr;		/* virtual address for video buffer */

/*---------------------------------------------------------------------
 ************************** INIT MODULE *******************************
 *--------------------------------------------------------------------*/
unsigned char devaddr[6];

int iomemory_init (void)
{
    int i;
    
    printk ("---------Hello----------\n");
   
    ptr = ioremap(0xf0000000,4096);
  
    for ( i=0 ; i<6; i++ )
    {
        devaddr[i] = readb((ptr+i));     
 	printk("%02x ", devaddr[i]);
    }
    printk("\n");

    return 0;
}

/*---------------------------------------------------------------------
 ************************** CLEANUP MODULE ****************************
 *--------------------------------------------------------------------*/

void iomemory_exit (void)
{
    iounmap(ptr);
    printk ("---------BYE----------\n");
}

module_init(iomemory_init);
module_exit(iomemory_exit);

MODULE_LICENSE("GPL");

