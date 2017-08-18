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
int get_gateway(int fd, struct sockaddr_nl *sa, int domain)
{
   char buf[BUFLEN];

   memset(buf, 0, BUFLEN);

   // assemble the message according to the netlink protocol
   struct nlmsghdr *nl;
   nl = (struct nlmsghdr*)buf;
   nl->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
   nl->nlmsg_type = RTM_GETROUTE;
   nl->nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;

   struct rtmsg *rt;
   rt = (struct rtmsg*)NLMSG_DATA(nl);
   rt->rtm_family = domain;

   // prepare struct msghdr for sending.
   struct iovec iov = { nl, nl->nlmsg_len };
   struct msghdr msg = { sa, sizeof(*sa), &iov, 1, NULL, 0, 0 };

   // send netlink message to kernel.
   int r = sendmsg(fd, &msg, 0);
   return (r < 0) ? -1 : 0;
}


static
int parse_rt_msg(struct rtmsg *rt, void *buf, size_t len)
{
   //char ifname[IF_NAMESIZE];
   printf("==================================\n");
   printf("family:\t\t%d\n", (rt->rtm_family == AF_INET) ? 4 :6);
   printf("table id:\t%d\n", rt->rtm_table);
   printf("protocol:\t%d\n", rt->rtm_protocol);
   printf("scope:\t\t%d\n", rt->rtm_scope);
   printf("type:\t\t%d\n", rt->rtm_type);
   printf("\n");

   struct rtattr *rta = NULL;
   int fa = rt->rtm_family;
   for_each_rattr(rta, buf, len) {
      if (rta->rta_type == RTA_GATEWAY) {
         printf("gateway:\t%s\n", util_ntop(fa, RTA_DATA(rta)));
      }
   }

   return 0;
}

static
uint32_t parse_nl_msg_gateway(void *buf, size_t len)
{
   struct nlmsghdr *nl = NULL;
   for_each_nlmsg(nl, buf, len) {
      if (nl->nlmsg_type == NLMSG_ERROR) {
         printf("error");
         return -1;
      }

      if (nl->nlmsg_type == RTM_NEWROUTE) {
         struct rtmsg *rt;
         rt = (struct rtmsg*)NLMSG_DATA(nl);
         parse_rt_msg(rt, RTM_RTA(rt), RTM_PAYLOAD(nl));
         continue;
      }


   }
   return nl->nlmsg_type;
}

int gateway_show(void)
{
   int fd = 0, len = 0;

   // First of all, we need to create a socket with the AF_NETLINK domain
   fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
//   check(fd);

   struct sockaddr_nl sa;
   memset(&sa, 0, sizeof(sa));
   sa.nl_family = AF_NETLINK;

   len = get_gateway(fd, &sa, AF_INET);
//   check(len);

   char buf[BUFLEN];
   uint32_t nl_msg_type;
   do {
      len = util_get_msg(fd, &sa, buf, BUFLEN);
//      check(len);

      nl_msg_type = util_parse_nl_msg_OK(buf, len);
   } while (nl_msg_type != NLMSG_DONE && nl_msg_type != NLMSG_ERROR);

   return 0;
}
