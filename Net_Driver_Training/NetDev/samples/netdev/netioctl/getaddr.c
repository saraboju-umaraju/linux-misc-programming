/*
  Linux socket ioctl demonstration program, 
*/

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <string.h>
#include <arpa/inet.h>

extern int errno;

/* define global variables */

int s; 			/* socket */
struct ifreq ifr;
char *myip;
char *ifname = "eth0";
unsigned char *myhwaddr;

int main(int argc, char *argv[], char *env)
{
    int i, rc;

    /* initialize */

    printf("Socket IOCTL Demo Program\n");

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	 printf("socket error: %s\n", strerror(errno));
         exit(1);
    }

    /* get the interface IP address */

    strcpy(ifr.ifr_name, ifname);
    ifr.ifr_addr.sa_family = AF_INET;

    if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
    {
       printf("SIOCGIFADDR: %s\n", strerror(errno));
       close(s);
       exit(1);
    }
    myip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    printf("%s\n", myip);

    /* get MAC address */
    if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0)
    {
       printf("SIOCGIFHWADDR: %s\n", strerror(errno));
       close(s);
       exit(1);
    }
    myhwaddr = ifr.ifr_hwaddr.sa_data;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *myhwaddr, *(myhwaddr+1), *(myhwaddr+2), *(myhwaddr+3),
                                              *(myhwaddr+4), *(myhwaddr+5));

    /* finalize */
    close(s);
}
