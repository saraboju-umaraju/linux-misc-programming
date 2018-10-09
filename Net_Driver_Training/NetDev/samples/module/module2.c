/***********************************************************************
*                                                                      *
* SourceName         : module2.c                                       *
* ModuleName         : module2.o                                       *
*                                                                      *
* Description        : This sample program shows how to extract        *
*                      command line arguments.                         *
*                                                                      *
*                                                                      *
* Programmer         : ----------                                      *
* Date               : 15/07/2002                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give "gcc -c module2.c",this    *
*                will compile and give 'module2.o' file which you can  *
*                load.                                                 *
* How To Load  : To load the module2.o file give                       *
*                "insmod  module2.o ival=200 sval="Welcome",this will  *
*                load the module to the kernel.                        *
*                                                                      *
***********************************************************************/

/*----------------------------------------------------------------------
 ************************** Define File ********************************
 *--------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 ************************** Include File *******************************
 *--------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

/*--------------------------------------------------------------------*/

int ival=0;
char *sval;

module_param(ival, int, S_IRUGO);
module_param(sval, charp, S_IRUGO);

int my_init(void)                                //module entry point 
{
  printk("#=== Module Initialized ===#\n");      //kernel print function
  printk("ival = %d \nval = %s\n", ival, sval);//kernel print function
  return 0;  
}

void my_exit(void)                            //cleanup function
{
  printk("#=== Module removed ===#\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

