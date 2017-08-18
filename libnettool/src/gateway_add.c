#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "util.h"

#define for_each_nlmsg(n, buf, len)             \
   for (n = (struct nlmsghdr*)buf;              \
        NLMSG_OK(n, (uint32_t)len) && n->nlmsg_type != NLMSG_DONE;   \
        n = NLMSG_NEXT(n, len))

#define for_each_rattr(n, buf, len)             \
   for (n = (struct rtattr*)buf; RTA_OK(n, len); n = RTA_NEXT(n, len))


static
int add_gateway(int fd, struct sockaddr_nl *sa, int domain, const char *ip)
{
   char buf[BUFLEN];
   int ip_len = 0;

   memset(buf, 0, BUFLEN);

   // assemble the message according to the netlink protocol
   struct nlmsghdr *nl;
   nl = (struct nlmsghdr*)buf;
   nl->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
   nl->nlmsg_type = RTM_NEWROUTE;
   // we request kernel to send back ack for result checking
   nl->nlmsg_flags = NLM_F_REQUEST | NLM_F_REPLACE | NLM_F_ACK | NLM_F_CREATE;

   struct rtmsg *rt;
   rt = (struct rtmsg*)NLMSG_DATA(nl);
   rt->rtm_family = domain;
   rt->rtm_table = RT_TABLE_MAIN;
   rt->rtm_protocol = RTPROT_STATIC;
   rt->rtm_scope = RT_SCOPE_UNIVERSE;
   rt->rtm_type = RTN_UNICAST;

   struct rtattr *rta = (struct rtattr*)RTM_RTA(rt);
   rta->rta_type = RTA_GATEWAY;
   ip_len = util_pton(domain, RTA_DATA(rta), ip);
   rta->rta_len = RTA_LENGTH(ip_len);
   nl->nlmsg_len = NLMSG_ALIGN(nl->nlmsg_len) + rta->rta_len;

   int l = BUFLEN - nl->nlmsg_len;
   rta = (struct rtattr*)RTA_NEXT(rta, l);
   rta->rta_type = RTA_OIF;
   rta->rta_len = RTA_LENGTH(sizeof(int));
   *((int*)RTA_DATA(rta)) = 2;
   nl->nlmsg_len += rta->rta_len;

   // prepare struct msghdr for sending.
   struct iovec iov = { nl, nl->nlmsg_len };
   struct msghdr msg = { sa, sizeof(*sa), &iov, 1, NULL, 0, 0 };

   // send netlink message to kernel.
   int r = sendmsg(fd, &msg, 0);
   return (r < 0) ? -1 : 0;
}



int gateway_add(void)
{
   int fd = 0, len = 0;

   // First of all, we need to create a socket with the AF_NETLINK domain
   fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
   //check(fd);

   struct sockaddr_nl sa;
   memset(&sa, 0, sizeof(sa));
   sa.nl_family = AF_NETLINK;

   add_gateway(fd, &sa, AF_INET, "172.16.0.1");

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
         printf("Success\n");
         break;
      case -EADDRNOTAVAIL:
         printf("Failed\n");
         break;
      case -EPERM:
         printf("Permission denied\n");
         break;
      default:
         printf("%s\n", strerror(err->error));
      }
   }

   return 0;
}
