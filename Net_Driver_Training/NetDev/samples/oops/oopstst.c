/***********************************************************************
*                                                                      *
* SourceName         : oopstst.c                                       *
*                                                                      *
* Description        : This is an application program  to test the     *
*                      the driver oops.                                *
*                                                                      *
* Programmer         : ----------                                      *
* Date               : 15/07/2002                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give                            *
*                "gcc -o oopstst oopstst.c" this will give the         *
*                output file 'oopstst' which you can run .             *
*                                                                      *
* How To Load  : To run samp4tst file give "./oopstst"                 *
*                                                                      *
***********************************************************************/

/*----------------------------------------------------------------------
 ************************** Include File *******************************
 *--------------------------------------------------------------------*/

 #include <stdio.h>

/*--------------------------------------------------------------------*/

char device[]  = {"/dev/mydevice"};
char wbuff[32] = {"ABCDEFGHIJKLMNOP"};
char rbuff[32];

main()
{
   int fd, nb;

   fd = open(device, 0666);
   if (fd <= 0)
   {
     err_sys("Can not open the device\n");
   }
   nb = write(fd, wbuff, 16);
   if (nb != 16)
     err_sys("Write Error\n");
   printf("No. of Bytes Written = %d\n", nb);
   nb = read(fd, rbuff, 16);
   printf("No. of Bytes Read = %d\n", nb);
   write(1, rbuff, nb);
   printf("\n");
   close(fd);
   exit(0);
}

/* display err message and exit */

err_sys(char *s)
{
  printf("%s\n", s);
  exit(1);
}

