
/***********************************************************************
*                                                                      *
* SourceName         : module1.c                                       *
* ModuleName         : module1.o                                       *
*                                                                      *
* Description        : This sample program shows how explicit init is  *
*                      implemented in the kernel module.               *        
*                                                                      *
* Programmer         : ----------                                      *
* Date               : 15/07/2002                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give "gcc -c module1.c",this    *
*                will compile and give 'module1.o' file which you can  *
*                load.                                                 *
* How To Load  : To load the module1.o file give "insmod module1.o",   *
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

int my_init(void) 		/*---------- init module -------------*/
{
  printk("<1>Hello World\n"); 
  return 0;
}

void my_cleanup(void) 		/*---------- cleanup module ----------*/
{
  printk("<1> Goodbye\n");
}

module_init(my_init);
module_exit(my_cleanup);

MODULE_LICENSE("GPL");

