/*
 * util.c
 *
 *  Created on: Aug 11, 2017
 *      Author: hico
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
//#include <socket.h>


#include "util.h"



int util_get_msg(int fd, struct sockaddr_nl *sa, void *buf, size_t len)
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

uint32_t util_parse_nl_msg_OK(void *buf, size_t len)
{
   struct nlmsghdr *nl = NULL;
   nl = (struct nlmsghdr*)buf;
   if (!NLMSG_OK(nl, len)) return 0;
   return nl->nlmsg_type;
}


int util_pton(int domain, void *buf, const char *ip)
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

char *util_ntop(int domain, void *buf)
{
   /*
    * this function is not thread safe
    */
   static char ip[INET6_ADDRSTRLEN];
   inet_ntop(domain, buf, ip, INET6_ADDRSTRLEN);
   return ip;
}

//****************************************
// family   : AF_NETLINK AF_ROUTE
// protocol : NETLINK_ROUTE
//*****************************************

int util_OpenSocket( )
{
   int                  Nl_socket   = 0;
   int                  vRetcode    = 0;
   struct sockaddr_nl   Sockaddr_nl;


   Nl_socket = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
   if (Nl_socket > 0)
   {
      memset ((void *) &Sockaddr_nl, 0, sizeof (Sockaddr_nl));

      Sockaddr_nl.nl_family      = AF_NETLINK;
      Sockaddr_nl.nl_pid         = getpid ();
      Sockaddr_nl.nl_groups      = 0;
      //gInfoSocket.Sockaddr_nl.nl_groups      = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;
      //  Sockaddr_nl.nl_groups = RTMGRP_LINK;

      vRetcode  = bind (  Nl_socket,
                  (struct sockaddr *) &Sockaddr_nl,
                  sizeof (Sockaddr_nl));

      if( vRetcode  != 0 )
      {
         Nl_socket = vRetcode;
      }
   }

   return Nl_socket;
}
