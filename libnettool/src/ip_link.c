#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>

#undef __USE_MISC

#include <linux/if.h>
#include <unistd.h>
#include <net/if.h>

#include <pthread.h>

#include "libnettool.h"
#include "util.h"

int         g_ip_link_Init = 0;
pthread_t   g_ip_link_thread;

#define IPLINK_MAX_ARRAY 1024

struct s_ip_link_notify
{
      char                       DeviceName[128];
      nettool_link_notify_func_t pfunction;
      int                        FlagToCheck;
};

struct s_ip_link_notify g_arrayNotification[IPLINK_MAX_ARRAY];

//*************************************************************
//*
//*************************************************************
static int ip_link_add_notification(   const char*                a_DeviceName,
                           nettool_link_notify_func_t a_pfunction,
                           int                        a_FlagToCheck)
{
   int vRetcode = 0;

   return vRetcode;

}
//*************************************************************
//*
//*************************************************************
int nettool_link_notify(   const char*                a_DeviceName,
                           nettool_link_notify_func_t a_pfunction,
                           int                        a_FlagToCheck)
{
   int vRetcode   = 0;
   int vSocket    = 0;

   if(   (a_DeviceName      == 0 )
      || (*a_DeviceName     == 0 )
      || (a_pfunction       == 0 )
      || (a_FlagToCheck     == 0 ) )
   {
      vRetcode = -1;
   }

   //**************************************************
   // create thread
   //**************************************************
   if( (vRetcode == 0 )  && ( g_ip_link_Init == 0 ) )
   {
      //vSocket = util_OpenSocket( );


      g_ip_link_Init = 1;
   }

   //**************************************************
   // add notification
   //**************************************************
   if( (vRetcode == 0 )  && ( g_ip_link_Init != 0 ) )
   {
      vRetcode = ip_link_add_notification(a_DeviceName,a_pfunction,a_FlagToCheck);

   }

   return vRetcode;

}

//*************************************************************
//*
//*************************************************************
static void
rtnl_print_link(struct nlmsghdr *h)
{
   struct ifinfomsg *iface;
   struct rtattr *attribute;
   int len;

   iface = NLMSG_DATA(h);
   len = h->nlmsg_len - NLMSG_LENGTH(sizeof(*iface));

   for (attribute = IFLA_RTA(iface); RTA_OK(attribute, len); attribute = RTA_NEXT(attribute, len))
   {
      switch(attribute->rta_type)
      {
         case IFLA_IFNAME:
            printf("rtnl_print_link : IFLA_IFNAME =  %d : %s ifi_flags =0x%X\n",
                   iface->ifi_index,
                   (char *) RTA_DATA(attribute),
                   iface->ifi_flags
            );
            break;
            //        case IFLA_LINK:
            //        {
            //           printf("rtnl_print_link : IFLA_LINK %d : %s\n",
            //
            //                  *((int*)RTA_DATA(attribute)));
            //           break;
            //        }
         default:
            break;
      }
   }
}
//*************************************************************
//*
//*************************************************************
static   int32_t  link_info_receive( int32_t             a_SocketDescriptor ,
                                     int                 a_IndexToFind,
                                     struct ifinfomsg*   a_pInfomsg)
{
   int                  vRetcode = 0;
   int                  end      = 0;
   char                 vBuffReply[4096]; /* a large buffer */
   struct iovec         iov;
   struct sockaddr_nl   kernel;

   int                  vFind    = 0;
   int                  len      = 0;
   struct nlmsghdr*     msg_ptr  = 0;    /* pointer to current part */
   struct msghdr        rtnl_reply;    /* generic msghdr structure */
   struct iovec         io_reply;
   struct ifinfomsg*    pIface   = 0;;

   memset(&kernel, 0, sizeof(kernel));

   kernel.nl_family = AF_NETLINK; // fill-in kernel address (destination)

   while (!end)
   {
      memset(&io_reply, 0, sizeof(io_reply));
      memset(&rtnl_reply, 0, sizeof(rtnl_reply));

      iov.iov_base             = vBuffReply;
      iov.iov_len              = 4096;
      rtnl_reply.msg_iov       = &iov;
      rtnl_reply.msg_iovlen    = 1;
      rtnl_reply.msg_name      = &kernel;
      rtnl_reply.msg_namelen   = sizeof(kernel);

      len = recvmsg(a_SocketDescriptor, &rtnl_reply, 0); /* read lots of data */

      printf("link_info_receive_2 : len = %d \n",len);

      if (len)
      {
         for (    msg_ptr = (struct nlmsghdr *) vBuffReply;
         NLMSG_OK(msg_ptr, len);
         msg_ptr = NLMSG_NEXT(msg_ptr, len))
         {
            switch(msg_ptr->nlmsg_type)
            {
               case NLMSG_DONE:        /* this is the NLMSG_DONE end msg */
                  end++;
                  printf("link_info_receive_2 : NLMSG_DONE\n");
                  break;
               case RTM_NEWLINK:    /* this is the RTM_NEWLINK msg */

                  pIface = NLMSG_DATA(msg_ptr);
                  if( a_IndexToFind == pIface->ifi_index )
                  {
                     *a_pInfomsg = *pIface;
                     vFind = 1 ;
                     break;
                  }

                  printf("link_info_receive_2 : RTM_NEWLINK \n");
                  rtnl_print_link(msg_ptr);
                  break;
               case RTM_GETLINK:
                  printf("link_info_receive_2 : RTM_GETLINK \n");
                  rtnl_print_link(msg_ptr);

                  break;
               default:    /* for education only,
                          should not happen here */
                  printf("message type %d, length %d\n", msg_ptr->nlmsg_type, msg_ptr->nlmsg_len);
                  break;
            }// switch(...)
            if( vFind ) break;
         }// for(...)
      }// if( len )
      if( vFind ) break;
   }// while (!end)

   if( vFind == 0 )
   {
      vRetcode = -1;
   }

   return vRetcode;
}
//*************************************************************
//*
//*************************************************************
static
int link_info_request( int            a_Fd,
                       struct sockaddr_nl*   a_pSockaddr_nl)
{
   char vBuf[BUFLEN];
   //int vLen  = 0;

   struct nlmsghdr*  pNlmsghdr      = 0;

   struct nl_req_s {
         struct nlmsghdr hdr;
         struct rtgenmsg gen;
   };

   memset(vBuf, 0, BUFLEN);

   // assemble the message according to the netlink protocol
   pNlmsghdr               = (struct nlmsghdr*)vBuf;
   pNlmsghdr->nlmsg_len    =  NLMSG_LENGTH(sizeof(struct rtgenmsg));
   pNlmsghdr->nlmsg_type   = RTM_GETLINK  ; // RTM_GETLINK + RTM_BASE
   // we request kernel to send back ack for result checking
   pNlmsghdr->nlmsg_flags  = NLM_F_REQUEST | NLM_F_DUMP | NLM_F_ACK;// | NLM_F_ROOT ; // NLM_F_REQUEST | NLM_F_DUMP | NLM_F_ROOT | NLM_F_ACK

   pNlmsghdr->nlmsg_pid    = getpid();
   pNlmsghdr->nlmsg_seq    = 1;

   // prepare struct msghdr for sending.
   struct iovec iov     = { pNlmsghdr, pNlmsghdr->nlmsg_len };
   struct msghdr msg    = { a_pSockaddr_nl, sizeof(struct sockaddr_nl), &iov, 1, NULL, 0, 0 };

   // send netlink message to kernel.
   int r = sendmsg(a_Fd, &msg, 0);
   return (r < 0) ? -1 : 0;

}

//*********************************************************************
//*
//*********************************************************************
static
int link_up(	int 				      a_Fd,
            	struct sockaddr_nl 	*a_pSockaddr_nl,
            	int 				      a_Domain,
            	int                  a_UP,
            	int                  a_IndexDevice)
{
   char vBuf[BUFLEN];
   //int vLen  = 0;
   int               vLinkAttribut  = 0;
   struct rtattr*    pRtAttr        = 0;
   struct nlmsghdr*  pNlmsghdr      = 0;
   struct ifinfomsg* pIfinfomsg     = 0;

   memset(vBuf, 0, BUFLEN);

   // assemble the message according to the netlink protocol
   pNlmsghdr 				   = (struct nlmsghdr*)vBuf;
   pNlmsghdr->nlmsg_len 	= NLMSG_LENGTH(sizeof(struct ifinfomsg));
   pNlmsghdr->nlmsg_type 	= RTM_NEWLINK;;
   // we request kernel to send back ack for result checking
   pNlmsghdr->nlmsg_flags 	= NLM_F_REQUEST | NLM_F_ACK;

   pIfinfomsg = (struct ifinfomsg*)NLMSG_DATA(pNlmsghdr);

   pIfinfomsg->ifi_family = AF_UNSPEC;
   pIfinfomsg->ifi_index  = a_IndexDevice;
   pIfinfomsg->ifi_change = 0xffffffff; /* ??? */

   //   action up => state = link down  sans cable
   //   pIfinfomsg->ifi_flags  = 0;
   //   pIfinfomsg->ifi_flags  = IFF_LOWER_UP;                       // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_RUNNING ;                       // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_RUNNING | IFF_LOWER_UP;         // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP;                             // eth1: <NO-CARRIER,BROADCAST,UP> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_RUNNING ;              // eth1: <NO-CARRIER,BROADCAST,UP> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_LOWER_UP;              // eth1: <NO-CARRIER,BROADCAST,UP> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_RUNNING | IFF_LOWER_UP;// eth1: <NO-CARRIER,BROADCAST,UP> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000

   //   action = up => state = link down  avec cable
   //   pIfinfomsg->ifi_flags  = 0;
   //   pIfinfomsg->ifi_flags  = IFF_LOWER_UP;                       // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_RUNNING ;                       // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_RUNNING | IFF_LOWER_UP;         // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP;                             // eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_RUNNING ;              // eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_LOWER_UP;              // eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_RUNNING | IFF_LOWER_UP;// eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000


   //   action = down => state = link up  avec cable
   //   pIfinfomsg->ifi_flags  = 0;
   //   pIfinfomsg->ifi_flags  = IFF_LOWER_UP;                       // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_RUNNING ;                       // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_RUNNING | IFF_LOWER_UP;         // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP;                             // eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_RUNNING ;              // eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_LOWER_UP;              // eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP group default qlen 1000
   //   pIfinfomsg->ifi_flags  = IFF_UP | IFF_RUNNING | IFF_LOWER_UP;// eth1: <BROADCAST,UP,LOWER_UP> mtu 777 qdisc pfifo_fast state UP group default qlen 1000

   if( a_UP )
   {
      pIfinfomsg->ifi_flags  = IFF_UP | IFF_RUNNING;
   }
   else
   {
      pIfinfomsg->ifi_flags  = 0;                       // eth1: <BROADCAST> mtu 777 qdisc pfifo_fast state DOWN group default qlen 1000
   }
   vLinkAttribut           = a_UP ? 1 : 0 ;


   pRtAttr                 = (struct rtattr*)IFLA_RTA(pIfinfomsg);
   pRtAttr->rta_type       =  IFLA_LINK ;
   memcpy(RTA_DATA(pRtAttr),&vLinkAttribut,sizeof(vLinkAttribut));

   pRtAttr->rta_len        = RTA_LENGTH(sizeof(vLinkAttribut));
   // update nlmsghdr length
   pNlmsghdr->nlmsg_len    = NLMSG_ALIGN(pNlmsghdr->nlmsg_len) + pRtAttr->rta_len;


   // prepare struct msghdr for sending.
   struct iovec iov 	   = { pNlmsghdr, pNlmsghdr->nlmsg_len };
   struct msghdr msg 	= { a_pSockaddr_nl, sizeof(*a_pSockaddr_nl), &iov, 1, NULL, 0, 0 };

   // send netlink message to kernel.
   int r = sendmsg(a_Fd, &msg, 0);
   return (r < 0) ? -1 : 0;
}
//*********************************************************************
//*
//*********************************************************************
int nettool_link_up(const char* a_DeviceName, int a_UP  )
{
   int                  vSocketFD   = 0;
   int                  vLen        = 0;
   struct sockaddr_nl   vSockaddr_nl;
   char                 vBuf[BUFLEN];
   uint32_t             vNl_msg_type;
   struct nlmsgerr      *pErrorStruct = 0;
   int                  vIndexDevice = 0;

   vIndexDevice = if_nametoindex(a_DeviceName);

   if( vIndexDevice == 0 )
   {
      fprintf(stderr,"\nError device %s  not exits ! \n\n",a_DeviceName);
   }
   else
   {
      // First of all, we need to create a socket with the AF_NETLINK domain
      vSocketFD = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
      //check(fd);

      memset(&vSockaddr_nl, 0, sizeof(vSockaddr_nl));
      vSockaddr_nl.nl_family = AF_NETLINK;
      link_up(vSocketFD, &vSockaddr_nl, AF_INET, a_UP,vIndexDevice);

      // after sending, we need to check the result
      vLen = util_get_msg(vSocketFD, &vSockaddr_nl, vBuf, BUFLEN);
      //check(vLen);

      vNl_msg_type = util_parse_nl_msg_OK(vBuf, vLen);

      if (vNl_msg_type == NLMSG_ERROR)
      {
         pErrorStruct = (struct nlmsgerr*)NLMSG_DATA(vBuf);
         switch (pErrorStruct->error) {
            case 0:
               printf("nettool_link_up : Success\n");
               break;
            case -EADDRNOTAVAIL:
               printf("nettool_link_up : Failed\n");
               break;
            case -EPERM:
               printf("nettool_link_up : Permission denied\n");
               break;
            default:
               printf("nettool_link_up : errno=%d %s\n", errno, strerror(pErrorStruct->error));
         }
      }
      if( vSocketFD)
      {
         close(vSocketFD);
      }
   }

   return 0;
}
//*********************************************************************
//*
//*********************************************************************
int nettool_link_info(const char* a_DeviceName,struct ifinfomsg* a_Ifinfomsg)
{
   int                  vRetcode    = 0;
   int                  vSocketFD   = 0;
   int                  vLen        = 0;
   struct sockaddr_nl   vSockaddr_nl;
   char                 vBuf[BUFLEN];
   uint32_t             vNl_msg_type;
   struct nlmsgerr      *pErrorStruct = 0;
   int                  vIndexDevice = 0;

   vIndexDevice = if_nametoindex(a_DeviceName);

   if( vIndexDevice == 0 )
   {
      fprintf(stderr,"\nError device %s  not exits ! \n\n",a_DeviceName);
   }
   else
   {
      // First of all, we need to create a socket with the AF_NETLINK domain
      vSocketFD = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE); //SOCK_RAW SOCK_DGRAM
      //check(fd);

      memset(&vSockaddr_nl, 0, sizeof(vSockaddr_nl));
      vSockaddr_nl.nl_family = AF_NETLINK;

      vRetcode = link_info_request(vSocketFD,&vSockaddr_nl);

      // after sending, we need to check the result
      //      vLen = get_msg(vSocketFD, &vSockaddr_nl, vBuf, BUFLEN);

      vRetcode =  link_info_receive(vSocketFD,vIndexDevice,a_Ifinfomsg);

      vNl_msg_type = util_parse_nl_msg_OK(vBuf, vLen);
      if (vNl_msg_type == NLMSG_ERROR) {
         pErrorStruct = (struct nlmsgerr*)NLMSG_DATA(vBuf);
         switch (pErrorStruct->error) {
            case 0:
               printf("nettool_link_up : Success\n");
               break;
            case -EADDRNOTAVAIL:
               printf("nettool_link_up : Failed\n");
               break;
            case -EPERM:
               printf("nettool_link_up : Permission denied\n");
               break;
            default:
               printf("nettool_link_up : errno=%d %s\n", errno, strerror(pErrorStruct->error));
         }
      }
      if( vSocketFD)
      {
         close(vSocketFD);
      }
   }

   return 0;

}



