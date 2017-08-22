/*
 * Network.c
 *
 *  Created on: Jan 31, 2017
 *      Author: hico
 */


#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>


#define NIPQUAD(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

#define NIPQUAD_FMT "%u.%u.%u.%u"

#define NIP6(addr) \
        ntohs((addr).s6_addr16[0]), \
        ntohs((addr).s6_addr16[1]), \
        ntohs((addr).s6_addr16[2]), \
        ntohs((addr).s6_addr16[3]), \
        ntohs((addr).s6_addr16[4]), \
        ntohs((addr).s6_addr16[5]), \
        ntohs((addr).s6_addr16[6]), \
        ntohs((addr).s6_addr16[7])

#define NIP6_FMT "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"

#define INFINITY_LIFE_TIME      0xFFFFFFFFU

int  Net_1()

{
      struct {
              struct nlmsghdr n;
              struct ifaddrmsg r;
//              char             buf[1024];
      } req;

      struct rtattr *rta;
      struct sockaddr_in6 *sin6p    __attribute__ ((unused));
      struct sockaddr_in *sinp      __attribute__ ((unused));
      int status;
      char buf[16384];
      struct nlmsghdr *nlmp;
      struct ifaddrmsg *rtmp;
      struct rtattr *rtatp;
      int rtattrlen;
      struct in_addr *inp           __attribute__ ((unused));
      struct in6_addr *in6p;
      struct ifa_cacheinfo *cache_info;

      int fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);

      /* We use RTM_GETADDR to fetch the ip address from the kernel interface table *
         * So what we do here is pretty simple, we populate the msg structure (req)   *
         * the size of the message buffer is specified to netlink message header, and *
         * flags values are set as NLM_F_ROOT | NLM_F_REQUEST. The request flag must  *
         * be set for all messages requesting the data from kernel. The root flag is  *
         * used to notify the kernel to return the full tabel. Another flag (not used)*
         * is NLM_F_MATCH. This is used to get only speficed entried in the table.    *
         * At the time of writing this program this flag is not implemented in kernel */

      memset(&req, 0, sizeof(req));
      req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
      req.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;
      req.n.nlmsg_type = RTM_GETADDR;


      /* AF_INET6 is used to signify the kernel to fetch only ipv6 entires.         *
        * Replacing this with AF_INET will fetch ipv4 address table.                 */

      req.r.ifa_family = AF_INET6;

      /* Fill up all the attributes for the rtnetlink header. The code is pretty easy*
        * to understand. The lenght is very important. We use 16 to signify the ipv6  *
       * address. If the user chooses to use AF_INET (ipv4) the length has to be     *
        * RTA_LENGTH(4) */

      rta = (struct rtattr *)(((char *)&req) + NLMSG_ALIGN(req.n.nlmsg_len));
      rta->rta_len = RTA_LENGTH(16);

      /* Time to send and recv the message from kernel */

      status = send(fd, &req, req.n.nlmsg_len, 0);

      if (status < 0) {
              perror("send");
              return 1;
      }

      status = recv(fd, buf, sizeof(buf), 0);

      if (status < 0) {
              perror("recv");
              return 1;
      }

      if(status == 0){
              printf("EOF\n");
              return 1;
      }

      /* Typically the message is stored in buf, so we need to parse the message to *
        * get the required data for our display. */

      for(nlmp = (struct nlmsghdr *)buf; status > sizeof(*nlmp);){
              int len = nlmp->nlmsg_len;
              int req_len = len - sizeof(*nlmp);

              if (req_len<0 || len>status) {
                      printf("error\n");
                      return -1;
              }

              if (!NLMSG_OK(nlmp, status)) {
                      printf("NLMSG not OK\n");
                      return 1;
              }

              rtmp = (struct ifaddrmsg *)NLMSG_DATA(nlmp);
              rtatp = (struct rtattr *)IFA_RTA(rtmp);

              /* Start displaying the index of the interface */

              printf("Index Of Iface= %d\n",rtmp->ifa_index);

              rtattrlen = IFA_PAYLOAD(nlmp);

              for (; RTA_OK(rtatp, rtattrlen); rtatp = RTA_NEXT(rtatp, rtattrlen)) {

               /* Here we hit the fist chunk of the message. Time to validate the    *
                 * the type. For more info on the different types see man(7) rtnetlink*
                 * The table below is taken from man pages.                           *
                 * Attributes                                                         *
                 * rta_type        value type             description                 *
                 * -------------------------------------------------------------      *
                 * IFA_UNSPEC      -                      unspecified.                *
                 * IFA_ADDRESS     raw protocol address   interface address           *
                 * IFA_LOCAL       raw protocol address   local address               *
                 * IFA_LABEL       asciiz string          name of the interface       *
                 * IFA_BROADCAST   raw protocol address   broadcast address.          *
                 * IFA_ANYCAST     raw protocol address   anycast address             *
                 * IFA_CACHEINFO   struct ifa_cacheinfo   Address information.        */

                      if(rtatp->rta_type == IFA_CACHEINFO){
                              cache_info = (struct ifa_cacheinfo *)RTA_DATA(rtatp);
                              if (cache_info->ifa_valid == INFINITY_LIFE_TIME)
                                      printf("valid_lft forever\n");
                              else
                                      printf("valid_lft %usec\n", cache_info->ifa_valid);

                              if (cache_info->ifa_prefered == INFINITY_LIFE_TIME)
                                      printf(" preferred_lft forever\n");
                              else
                                      printf(" preferred_lft %usec\n",cache_info->ifa_prefered);
                      }

                      /* NOTE: All the commented code below can be used as it is for ipv4 table */

                      if(rtatp->rta_type == IFA_ADDRESS){
                              // inp = (struct in_addr *)RTA_DATA(rtatp);
                              in6p = (struct in6_addr *)RTA_DATA(rtatp);
                              printf("addr0: " NIP6_FMT "\n",NIP6(*in6p));
                              // printf("addr0: "NIPQUAD_FMT"\n",NIPQUAD(*inp));
                      }

                      if(rtatp->rta_type == IFA_LOCAL){
                              // inp = (struct in_addr *)RTA_DATA(rtatp);
                              in6p = (struct in6_addr *)RTA_DATA(rtatp);
                              printf("addr1: " NIP6_FMT "\n",NIP6(*in6p));
                              // printf("addr1: "NIPQUAD_FMT"\n",NIPQUAD(*inp));
                      }

                      if(rtatp->rta_type == IFA_BROADCAST){
                              // inp = (struct in_addr *)RTA_DATA(rtatp);
                              in6p = (struct in6_addr *)RTA_DATA(rtatp);
                              printf("bcataddr: " NIP6_FMT "\n",NIP6(*in6p));
                              // printf("Bcast addr: "NIPQUAD_FMT"\n",NIPQUAD(*inp));
                      }

                      if(rtatp->rta_type == IFA_ANYCAST){
                             //inp = (struct in_addr *)RTA_DATA(rtatp);
                             in6p = (struct in6_addr *)RTA_DATA(rtatp);
                             printf("anycastaddr: "NIP6_FMT"\n",NIP6(*in6p));
                            // printf("anycast addr: "NIPQUAD_FMT"\n",NIPQUAD(*inp));
                      }

              }
              status -= NLMSG_ALIGN(len);
              nlmp = (struct nlmsghdr*)((char*)nlmp + NLMSG_ALIGN(len));

      }

      return 0;
}
//**********************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
//#include <net/if.h>

#define NEW_IP    "192.168.1.2"
#define BUFLEN    4096

#define for_each_nlmsg(n, buf, len)             \
   for (n = (struct nlmsghdr*)buf;              \
        NLMSG_OK(n, (uint32_t)len) && n->nlmsg_type != NLMSG_DONE;   \
        n = NLMSG_NEXT(n, len))

#define for_each_rattr(n, buf, len)             \
   for (n = (struct rtattr*)buf; RTA_OK(n, len); n = RTA_NEXT(n, len))

static inline
void check(int val)
{
   /*
    * NOTICE: we have not reclaim the resources allocated before, this is
    * due to the fact that this program aims to illustrate the facts and
    * principles, but not for production environment
    */
   if (val < 0) {
      printf("check error: %s\n", strerror(errno));
      exit(1);
   }
}

static inline
int pton(int domain, void *buf, const char *ip)
{
   if (domain == AF_INET) {
      inet_pton(AF_INET, ip, buf);
      return sizeof(struct in_addr);
   }

   if (domain == AF_INET6) {
      inet_pton(AF_INET6, ip, buf);
      return sizeof(struct in6_addr);
   }

   return -1;
}


static
int add_ip(int fd, struct sockaddr_nl *sa, int domain, const char *ip)
{
   char buf[BUFLEN];
   int ip_len = 0;

   memset(buf, 0, BUFLEN);

   // assemble the message according to the netlink protocol
   struct nlmsghdr *nl;
   nl = (struct nlmsghdr*)buf;
   nl->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
   // NOTE: the only difference between ip add and ip del operations is
   // the nlmsg_type.
   nl->nlmsg_type = RTM_NEWADDR;
   // we request kernel to send back ack for result checking
   nl->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;

   struct ifaddrmsg *ifa;
   ifa = (struct ifaddrmsg*)NLMSG_DATA(nl);
   ifa->ifa_family = domain; // we only get ipv4 address here
   ifa->ifa_prefixlen = 16;
   ifa->ifa_flags = IFA_F_PERMANENT;
   ifa->ifa_scope = 0;
   ifa->ifa_index = 2;

   // del local address
   struct rtattr *rta = (struct rtattr*)IFA_RTA(ifa);
   rta->rta_type = IFA_LOCAL;
   ip_len = pton(domain, RTA_DATA(rta), ip);
   rta->rta_len = RTA_LENGTH(ip_len);
   // update nlmsghdr length
   nl->nlmsg_len = NLMSG_ALIGN(nl->nlmsg_len) + rta->rta_len;

   // del interface address
   int l = BUFLEN - nl->nlmsg_len;
   rta = (struct rtattr*)RTA_NEXT(rta, l);
   rta->rta_type = IFA_ADDRESS;
   ip_len = pton(domain, RTA_DATA(rta), ip);
   rta->rta_len = RTA_LENGTH(ip_len);
   // update nlmsghdr length
   nl->nlmsg_len += rta->rta_len;

   // prepare struct msghdr for sending.
   struct iovec iov = { nl, nl->nlmsg_len };
   struct msghdr msg = { sa, sizeof(*sa), &iov, 1, NULL, 0, 0 };

   // send netlink message to kernel.
   int r = sendmsg(fd, &msg, 0);
   return (r < 0) ? -1 : 0;
}

static
int get_msg(int fd, struct sockaddr_nl *sa, void *buf, size_t len)
{
   struct iovec iov;
   struct msghdr msg;
   iov.iov_base = buf;
   iov.iov_len = len;

   memset(&msg, 0, sizeof(msg));
   msg.msg_name = sa;
   msg.msg_namelen = sizeof(*sa);
   msg.msg_iov = &iov;
   msg.msg_iovlen = 1;

   return recvmsg(fd, &msg, 0);
}

static
uint32_t parse_nl_msg(void *buf, size_t len)
{
   struct nlmsghdr *nl = NULL;
   nl = (struct nlmsghdr*)buf;
   if (!NLMSG_OK(nl, len)) return 0;
   return nl->nlmsg_type;
}

int Net_2(void)
{
   int fd = 0, len = 0;

   // First of all, we need to create a socket with the AF_NETLINK domain
   fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
   check(fd);

   struct sockaddr_nl sa;
   memset(&sa, 0, sizeof(sa));
   sa.nl_family = AF_NETLINK;

   add_ip(fd, &sa, AF_INET, NEW_IP);

   // after sending, we need to check the result
   char buf[BUFLEN];
   uint32_t nl_msg_type;
   len = get_msg(fd, &sa, buf, BUFLEN);
   check(len);

   nl_msg_type = parse_nl_msg(buf, len);
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
}// Net_2
