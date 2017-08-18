#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "util.h"

#define NEW_IP    "192.168.1.2"

#define for_each_nlmsg(n, buf, len)             \
   for (n = (struct nlmsghdr*)buf;              \
        NLMSG_OK(n, (uint32_t)len) && n->nlmsg_type != NLMSG_DONE;   \
        n = NLMSG_NEXT(n, len))

#define for_each_rattr(n, buf, len)             \
   for (n = (struct rtattr*)buf; RTA_OK(n, len); n = RTA_NEXT(n, len))




static
int add_ip(	int 				      a_Fd,
           	struct sockaddr_nl 	*a_pSockaddr_nl,
            int 				      a_Domain,
            const char 			   *a_Ip)
{
   char vBuf[BUFLEN];
   int vIpLen = 0;

   memset(vBuf, 0, BUFLEN);

   // assemble the message according to the netlink protocol
   struct nlmsghdr *pNlmsghdr;
   pNlmsghdr 				   = (struct nlmsghdr*)vBuf;
   pNlmsghdr->nlmsg_len 	= NLMSG_LENGTH(sizeof(struct ifaddrmsg));
   // NOTE: the only difference between ip add and ip del operations is
   // the nlmsg_type.
   pNlmsghdr->nlmsg_type 	= RTM_NEWADDR;
   // we request kernel to send back ack for result checking
   pNlmsghdr->nlmsg_flags 	= NLM_F_REQUEST | NLM_F_ACK;

   struct ifaddrmsg *pIfAddrMsg;
   pIfAddrMsg = (struct ifaddrmsg*)NLMSG_DATA(pNlmsghdr);
   pIfAddrMsg->ifa_family 		= a_Domain; // we only get ipv4 address here
   pIfAddrMsg->ifa_prefixlen 	= 16;
   pIfAddrMsg->ifa_flags 		= IFA_F_PERMANENT;
   pIfAddrMsg->ifa_scope 		= 0;
   pIfAddrMsg->ifa_index 		= 3;

   // add local address
   struct rtattr *pRtAttr 	= (struct rtattr*)IFA_RTA(pIfAddrMsg);
   pRtAttr->rta_type 		= IFA_LOCAL;
   vIpLen 					   = util_pton(a_Domain, RTA_DATA(pRtAttr), a_Ip);
   pRtAttr->rta_len 		   = RTA_LENGTH(vIpLen);
   // update nlmsghdr length
   pNlmsghdr->nlmsg_len 	= NLMSG_ALIGN(pNlmsghdr->nlmsg_len) + pRtAttr->rta_len;

   // del interface address
   int vLen 			      = BUFLEN - pNlmsghdr->nlmsg_len;
   pRtAttr 				      = (struct rtattr*)RTA_NEXT(pRtAttr, vLen);
   pRtAttr->rta_type       = IFA_ADDRESS;
   vIpLen 				      = util_pton(a_Domain, RTA_DATA(pRtAttr), a_Ip);
   pRtAttr->rta_len 	      = RTA_LENGTH(vIpLen);
   // update nlmsghdr length
   pNlmsghdr->nlmsg_len    += pRtAttr->rta_len;

   // prepare struct msghdr for sending.
   struct iovec iov 	   = { pNlmsghdr, pNlmsghdr->nlmsg_len };
   struct msghdr msg 	= { a_pSockaddr_nl, sizeof(*a_pSockaddr_nl), &iov, 1, NULL, 0, 0 };

   // send netlink message to kernel.
   int r = sendmsg(a_Fd, &msg, 0);
   return (r < 0) ? -1 : 0;
}


int nettool_ip_add(void)
{
   int fd = 0, len = 0;

   // First of all, we need to create a socket with the AF_NETLINK domain
   fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
//   check(fd);

   struct sockaddr_nl sa;
   memset(&sa, 0, sizeof(sa));
   sa.nl_family = AF_NETLINK;

   add_ip(fd, &sa, AF_INET, NEW_IP);

   // after sending, we need to check the result
   char buf[BUFLEN];
   uint32_t nl_msg_type;
   len = util_get_msg(fd, &sa, buf, BUFLEN);
//   check(len);

   nl_msg_type = util_parse_nl_msg_OK(buf, len);
   if (nl_msg_type == NLMSG_ERROR) {
      struct nlmsgerr *err = (struct nlmsgerr*)NLMSG_DATA(buf);
      switch (err->error) {
      case 0:
         printf("ip_add : Success\n");
         break;
      case -EADDRNOTAVAIL:
         printf("ip_add : Failed\n");
         break;
      case -EPERM:
         printf("ip_add : Permission denied\n");
         break;
      default:
         printf("ip_add : errno=%d %s\n", errno, strerror(err->error));
      }
   }

   return 0;
}
