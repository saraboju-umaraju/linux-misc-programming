/*
 * application program to test driver
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char device[] = {"/dev/mydevice"};
char wbuf[]={"ABCDEFGHIJKLMNOP"};
char rbuf[32];
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
     printf("Can not open the device\n");
     exit(1);
   }

   nb = write(fd, wbuf, 16);
   if (nb != 16)
   {
     printf("Write Error\n");
     exit(1);
   }
   ioctl(fd, 0, &MyData);
   printf("No. of Bytes Written = %d\n", nb);
   nb = read(fd, rbuf, 16);
   printf("No. of Bytes Read = %d\n", nb);

   MyData.MyInt = 100;
   strcpy(MyData.MyString, rbuf);
   printf ("In user %2X:%2X:%2x\n", rbuf[2], rbuf[1], rbuf[0]);
//   ioctl(fd, 0, &MyData);

   close(fd);
   exit(0);
}

