/***********************************************************************
*                                                                      *
* SourceName         : impsym.c                                        *
* ModuleName         : impsym.o                                        *
*                                                                      *
* Description        : This sample program shows how to implement      *
*                      a simple module with module init & clean up.    *
*                                                                      *
* Programmer         : B R Sahu                                        *
* Date               : 06/01/2005                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give "gcc -c impsym.c",this     *
*                will compile and give 'impsym.o' file which you can   *
*                load.                                                 *
* How To Load  : To load the module.o file give "insmod impsym.o",     *
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
extern char MyString[];

int impsym_init(void)                  /*------ module entry point -------*/ 
{
  printk("Import Symbol Module Initialized\n");  /*- kernel print function -*/
  printk("%s", MyString);
  return 0;  
}

void impsym_exit(void)              /*------ cleanup module -----------*/
{
  printk("Module removed\n");
}

module_init(impsym_init);
module_exit(impsym_exit);

MODULE_LICENSE("GPL");

