/***********************************************************************
*                                                                      *
* SourceName         : samp8read.c                                     *
*                                                                      *
* Description        : This is an application program  to test the     *
*                      the driver (read method) sample8.               *
*                                                                      *
* Programmer         : ----------                                      *
* Date               : 19/02/2007                                      *
*                                                                      *
*----------------------------------------------------------------------*
* Notes                                                                *
* How To Build : On the command prompt give                            *
*                "gcc -o samp8read samp8read.c" this will give the     *
*                output file 'samp8read' which you can run.            *
*                                                                      *
* How To Load  : To run samp8read file give "./samp8read"              *
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

