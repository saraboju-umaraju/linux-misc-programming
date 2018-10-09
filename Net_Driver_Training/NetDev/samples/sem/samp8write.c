/***********************************************************************
*                                                                      *
* SourceName         : samp8write.c                                    *
*                                                                      *
* Description        : This is an application program  to test the     *
*                      the driver (write method) sample8.              *
*                                                                      *
* Programmer         : ----------                                      *
* Date               : 19/03/2007                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give                            *
*                "gcc -o samp8write samp8write.c" this will give the   *
*                output file 'samp8write' which you can run.           *
*                                                                      *
* How To Load  : To run samp8write file give "./samp8write"            *
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
   sleep(100);
   if (nb != 16)
     err_sys("Write Error\n");
   printf("No. of Bytes Written = %d\n", nb);
   close(fd);
   exit(0);
}

/* display err message and exit */

err_sys(char *s)
{
  printf("%s\n", s);
  exit(1);
}

