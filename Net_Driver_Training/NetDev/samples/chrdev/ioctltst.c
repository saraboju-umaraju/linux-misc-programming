/***********************************************************************
*                                                                      *
* SourceName         : samp4tst.c                                       *
*                                                                      *
* Description        : This is an application program  to test the     *
*                      the driver sample4.                             *
*                                                                      *
* Programmer         : ----------                                      *
* Date               : 15/07/2002                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give                            *
*                "gcc -o sampi4tst samp4tst.c" this will give the      *
*                output file 'samp4tst' which you can run.             *
*                                                                      *
* How To Load  : To run samp4tst file give "./samp4tst"                *
*                                                                      *
***********************************************************************/

/*----------------------------------------------------------------------
 ************************** Include File *******************************
 *--------------------------------------------------------------------*/

 #include <stdio.h>

/*--------------------------------------------------------------------*/

char device[]  = {"/dev/mydevice"};
struct _MyData {
    long MyInt;
    char MyString[16];
    }MyData;

main()
{
   int fd, nb;

   fd = open(device, 0666);
   if (fd <= 0)
   {
     err_sys("Can not open the device\n");
   }
   MyData.MyInt = 100;
   strcpy(MyData.MyString, "Welcome");
   ioctl(fd, 0, &MyData);
   close(fd);
   exit(0);
}

/* display err message and exit */

err_sys(char *s)
{
  printf("%s\n", s);
  exit(1);
}

