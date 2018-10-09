/*
 * code for getting the kernel routing table
 * B R Sahu
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

struct rtnl_handle
{
	int fd;
	struct sockaddr_nl  local;
	struct sockaddr_nl  peer;   
	__u32 seq;
	__u32  dump;
};

// This code may not be complete in all respects, it just shows the flow of
// control, for more detailed information refer to the code of iproute2 and
// zebra packages.

// This function is to open the netlink socket as the name suggests.
int rtnl_open(struct rtnl_handle* rth)
{
    int addr_len;
    memset(rth, 0, sizeof(rth));

    // Creating the netlink socket of family NETLINK_ROUTE

    rth->fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (rth->fd < 0) {
        perror("cannot open netlink socket");
        return -1;
    }

    memset(&rth->local, 0, sizeof(rth->local));
    rth->local.nl_family = AF_NETLINK;
    rth->local.nl_groups = 0;

    // Binding the netlink socket
    if (bind(rth->fd, (struct sockaddr*)&rth->local, sizeof(rth->local)) < 0)
    {
        perror("cannot bind netlink socket");
        return -1;
    }

    addr_len = sizeof(rth->local);
    if (getsockname(rth->fd, (struct sockaddr*)&rth->local, &addr_len) < 0)
    {
        perror("cannot getsockname");
        return -1;
    }

    if (addr_len != sizeof(rth->local)) {
        fprintf(stderr, "wrong address length %d\n", addr_len);
        return -1;
    }

    if (rth->local.nl_family != AF_NETLINK) {
        fprintf(stderr, "wrong address family %d\n", rth->local.nl_family);
        return -1;
    }
    rth->seq = time(NULL);
    return 0;
}

// This function does the actual reading and writing to the netlink socket
int rtnl_talk(struct rtnl_handle *rtnl, struct nlmsghdr *n, pid_t peer,
        unsigned groups, struct nlmsghdr *answer)
{
    int status, len;
    struct nlmsghdr *h;
    struct sockaddr_nl nladdr;

    // Forming the iovector with the netlink packet.
    struct iovec iov = { (void*)n, n->nlmsg_len };
    char   buf[8192];

    // Forming the message to be sent.
    struct msghdr msg = {
        (void*)&nladdr, sizeof(nladdr),
        &iov,   1,
        NULL,   0,
        0
    };

    // Filling up the details of the netlink socket to be contacted in the
    // kernel.
    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    nladdr.nl_pid = peer;
    nladdr.nl_groups = groups;

    n->nlmsg_seq = ++rtnl->seq;
    if (answer == NULL)
        n->nlmsg_flags |= NLM_F_DUMP;

    // Actual sending of the message, status contains success/failure
    status = sendmsg(rtnl->fd, &msg, 0);

    if (status < 0)
        return -1;

    memset(buf, 0, sizeof(buf));
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf); 
    len = recvmsg(rtnl->fd, &msg, 0);
    if (len < 0)
    {
      printf("recvmsg: error\n");
      return -1;
    }else
    {
      int i;
      unsigned char *ptr = (char *)&msg;

      printf("recvmsg: len %d\n", len );
      printf("msghdr contents:");
      for (i = 0; i < sizeof(struct msghdr); i++)
      {
	if (i % 16 == 0)
	  printf("\n");
	printf("%02x ", ptr[i]);
      }
      printf("\n");

      printf("iov %08x\n", &iov);
      printf("iov_base %08x iov_len %d\n", iov.iov_base, iov.iov_len);
      h = (struct nlmsghdr *)buf;
      printf("len %d type %04x\n", h->nlmsg_len, h->nlmsg_type);

      printf("buf contents:");
      ptr = buf;
      for (i = 0; i < len; i++)
      {
	if (i % 16 == 0)
	   printf("\n");
	printf("%02x ", ptr[i]);
      }
      printf("\n");

      {
	struct nlmsghdr *nh;
	struct rtmsg *r;
	struct rtattr *rta;
        char *ptr;
	int rtabuflen;

	for (nh = (struct nlmsghdr *) buf; NLMSG_OK (nh, len);
              nh = NLMSG_NEXT (nh, len)) {
             /* The end of multipart message. */
             if (nh->nlmsg_type == NLMSG_DONE)
                 return 0;

             if (nh->nlmsg_type == NLMSG_ERROR)
	     {
                 /* Do some error handling. */
                 printf("NLMSG_ERROR\n");
		 continue;
	     }

             if (nh->nlmsg_type == RTM_NEWROUTE)
	     {
               /* Continue with parsing payload. */

	       r = NLMSG_DATA(nh);
               printf("family %d\n", r-> rtm_family);

               rta = (struct rtattr *)(((char *) nh) + sizeof(struct nlmsghdr)
						+ sizeof(struct rtmsg));
	       printf("option len %d, type %d\n", rta->rta_len, rta->rta_type);
               ptr = (char *)RTA_DATA(rta);
	       printf("%s\n", (char *)inet_ntoa(*(long *)ptr));

	       rtabuflen = sizeof(struct rtattr);
	       rta = (struct rtattr *)RTA_NEXT(rta, rtabuflen);
	       printf("option len %d, type %d\n", rta->rta_len, rta->rta_type);
               ptr = (char *)RTA_DATA(rta);
	       printf("%s\n", (char *)inet_ntoa(*(long *)ptr));
	     }
         }
      }
    }
}

// This function forms the netlink packet to get a route to the kernel routing
// table
int route_get()
{
    struct rtnl_handle rth;

    // structure of the netlink packet.
    struct {
        struct nlmsghdr     n;
        struct rtmsg        r;
        char            buf[1024];
    } req;

    char  mxbuf[256];
    struct rtattr * mxrta = (void*)mxbuf;
    unsigned mxlock = 0;

    memset(&req, 0, sizeof(req));

    // Initialisation of a few parameters
    req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    req.n.nlmsg_flags = NLM_F_REQUEST;
    req.n.nlmsg_type = RTM_GETROUTE;
    req.r.rtm_family = AF_INET;
    req.r.rtm_table = RT_TABLE_MAIN;

    req.r.rtm_protocol = RTPROT_BOOT;
    req.r.rtm_scope = RT_SCOPE_UNIVERSE;
    req.r.rtm_type = RTN_LOCAL;

    mxrta->rta_type = RTA_METRICS;
    mxrta->rta_len = RTA_LENGTH(0);

    // opening the netlink socket to communicate with the kernel
    if (rtnl_open(&rth) < 0)
    {
        fprintf(stderr, "cannot open rtnetlink\n");
        exit(1);
    }

    // sending the packet to the kernel.
    if (rtnl_talk(&rth, &req.n, 0, 0, NULL) < 0)
        exit(2);
    printf("OK\n");
    return 0;
}

/* main function */
int main(int argc, char *argv[])
{
   route_get();
}

