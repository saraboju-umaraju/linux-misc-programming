/***********************************************************************
*                                                                      *
* SourceName         : expsym.c                                        *
* ModuleName         : expsym.o                                        *
*                                                                      *
* Description        : This sample program shows how to implement      *
*                      a simple module with module init & clean up.    *
*                                                                      *
* Programmer         : B R Sahu                                        *
* Date               : 15/07/2006                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give "gcc -c expsym.c",this will*
*                compile and give 'module.o' file which you can load.  *
*                                                                      *
* How To Load  : To load the expsym.o file give "insmod expsym.o",     *
*                this will load the module to the kernel.              *
*                                                                      *
***********************************************************************/

/*----------------------------------------------------------------------
 ************************** Define File ********************************
 *--------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 ************************** Include File *******************************
 *--------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/init.h>

/*--------------------------------------------------------------------*/
char MyString[16];
char MyInteger;

EXPORT_SYMBOL(MyString);
EXPORT_SYMBOL(MyInteger);

int expsym_init(void)                  /*------ module entry point -------*/ 
{
  printk("Module Initialized\n");  /*------ kernel print function ----*/
  memcpy(MyString, "Hello World\n", 13);
  return 0;  
}

void expsym_exit(void)              /*------ cleanup module -----------*/
{
  printk("Module removed\n");
}

module_init(expsym_init);
module_exit(expsym_exit);

MODULE_LICENSE("GPL");

