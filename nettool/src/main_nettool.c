#include <asm/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

#include "liberrno.h"

#undef __USE_MISC

//#include <net/if.h> // ok
#include <linux/if.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include "libnettool.h"

#include "main_nettool.h"

static char gStringError[] = "ERROR";
static char gStringNull[] = "";

char gArray_RTM_Type [RTM_MAX][128];
char gArray_IFA_Type [__IFA_MAX][128];

struct sInfoSocket gInfoSocket;
struct sArrayFamily gArrayFamily;





void PrintUsage(int argc, char *argv[])
{
   printf("\n");
   printf(" Usage %s [ARG]\n",argv[0]);
   printf("\t all    : all test\n");
   printf("\t evloop : loop event\n");
   printf("\t lu     : link up\n");
   printf("\t ld     : link down\n");
   printf("\t li     : link info\n");
   printf("\n");
}


void net_event_show()
{
   nettool_ip_add();
   nettool_ip_show();

   //   ip_del();
   //   ip_show();
   //
   //   gateway_add();
   //   ip_show();
   //
   //   gateway_del();
   //   ip_show();
   //
   //   gateway_show();
   //   ip_show();


}
//******************************************************
//*
//******************************************************
int main (int argc, char *argv[])
{
   int vRetCode = 0;

   SetArrays();

   //printf("id=%d , %s \n ",0,ArrayFindItem(&gArrayFamily,0));
   //printf("id=%d , %s \n ",ARRAY_ADDRESS_FAMILY_MAX-3,ArrayFindItem(&gArrayFamily,ARRAY_ADDRESS_FAMILY_MAX-3));
   //printf("id=%d , %s \n ",ARRAY_ADDRESS_FAMILY_MAX-2,ArrayFindItem(&gArrayFamily,ARRAY_ADDRESS_FAMILY_MAX-2));
   //printf("id=%d , %s \n ",ARRAY_ADDRESS_FAMILY_MAX-1,ArrayFindItem(&gArrayFamily,ARRAY_ADDRESS_FAMILY_MAX-1));


   if( argc > 1 )
   {


      util_OpenSocket();


      //**********************************************
      //*         loop event
      //**********************************************
      if( strcmp("evloop",argv[1]) == 0 ) // event loop
      {
         vRetCode = LoopEvent(argc,argv);
      }
      //**********************************************
      //*         print all info
      //**********************************************
      else if( strcmp("all",argv[1]) == 0 ) //
      {
         net_event_show();
      }
      //**********************************************
      //*         LINK
      //**********************************************
      else if( argv[1][0] == 'l' ) // link
      {
         ip_link(argc,argv);
      }
      //**********************************************
      //*         ip_show
      //**********************************************
      else if( strcmp("show",argv[1]) == 0  ) //
      {
         nettool_ip_show();
      }
      else if( strcmp("add",argv[1]) == 0  ) //
      {
         nettool_ip_add();
      }
      else if( strcmp("",argv[1]) == 0  ) //
      {
         nettool_ip_add();
      }
      else
      {
         PrintUsage(argc,argv);
      }

      CloseSocket();
   }
   else
   {
      PrintUsage(argc,argv);
   }

   return vRetCode ;

}

void CloseSocket()
{
   if( gInfoSocket.Nl_socket )
   {
      close(gInfoSocket.Nl_socket);
      gInfoSocket.Nl_socket = 0;
   }

}
//****************************************
// family   : AF_NETLINK AF_ROUTE
// protocol : NETLINK_ROUTE
//*****************************************

int util_OpenSocket( )
{
   int vRetcode = 0;

   gInfoSocket.Nl_socket = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
   if (gInfoSocket.Nl_socket < 0)
   {
      printf ("Socket Open Error!");
      exit (1);
   }

   memset ((void *) &gInfoSocket.Sockaddr_nl, 0, sizeof (gInfoSocket.Sockaddr_nl));

   gInfoSocket.Sockaddr_nl.nl_family      = AF_NETLINK;
   gInfoSocket.Sockaddr_nl.nl_pid         = getpid ();
   gInfoSocket.Sockaddr_nl.nl_groups      = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;
   //  Sockaddr_nl.nl_groups = RTMGRP_LINK;

   vRetcode  = bind (  gInfoSocket.Nl_socket,
                       (struct sockaddr *) &gInfoSocket.Sockaddr_nl,
                       sizeof (gInfoSocket.Sockaddr_nl));

   if( vRetcode < 0 )
   {
      printf ("Socket bind failed! %d %s ",errno, strerror(errno) );
      exit (1);
   }

   return 0;
}
int LoopEvent (int argc, char *argv[])
{
   int retval;


   while (1)
   {
      FD_ZERO (&gInfoSocket.ReadFileDescriptor);
      FD_CLR (gInfoSocket.Nl_socket, &gInfoSocket.ReadFileDescriptor);
      FD_SET (gInfoSocket.Nl_socket, &gInfoSocket.ReadFileDescriptor);

      gInfoSocket.Timeval.tv_sec = 10;
      gInfoSocket.Timeval.tv_usec = 0;

      retval = select (FD_SETSIZE, &gInfoSocket.ReadFileDescriptor, NULL, NULL, &gInfoSocket.Timeval);
      if (retval == -1)
         printf ("Error select() \n");
      else if (retval)
      {
         //printf ("Event recieved >> \n");
         read_event();
      }
      //else          printf ("## Select TimedOut ## \n");
   }
   return 0;
}
//******************************************************
//*
//******************************************************
void SetArrayRTMTypes()
{

   strcpy(gArray_RTM_Type[RTM_BASE],"RTM_BASE");
   strcpy(gArray_RTM_Type[RTM_NEWLINK],"RTM_NEWLINK");

   strcpy(gArray_RTM_Type[RTM_DELLINK],"RTM_DELLINK");
   strcpy(gArray_RTM_Type[RTM_GETLINK],"RTM_GETLINK");

   strcpy(gArray_RTM_Type[RTM_SETLINK],"RTM_SETLINK");

   strcpy(gArray_RTM_Type[RTM_NEWADDR],"RTM_NEWADDR");
   strcpy(gArray_RTM_Type[RTM_DELADDR],"RTM_DELADDR");
   strcpy(gArray_RTM_Type[RTM_GETADDR],"RTM_GETADDR");

   strcpy(gArray_RTM_Type[RTM_NEWROUTE],"RTM_GETADDR");
   strcpy(gArray_RTM_Type[RTM_DELROUTE],"RTM_DELROUTE");
   strcpy(gArray_RTM_Type[RTM_GETROUTE],"RTM_GETROUTE");

   strcpy(gArray_RTM_Type[RTM_NEWNEIGH],"RTM_NEWNEIGH");
   strcpy(gArray_RTM_Type[RTM_DELNEIGH],"RTM_DELNEIGH");
   strcpy(gArray_RTM_Type[RTM_GETNEIGH],"RTM_GETNEIGH");

   strcpy(gArray_RTM_Type[RTM_NEWPREFIX],"RTM_NEWPREFIX");

   strcpy(gArray_RTM_Type[RTM_NEWNETCONF],"RTM_NEWNETCONF");
   strcpy(gArray_RTM_Type[RTM_GETNETCONF],"RTM_GETNETCONF");



   strcpy(gArray_IFA_Type[IFA_UNSPEC],    "IFA_UNSPEC");
   strcpy(gArray_IFA_Type[IFA_ADDRESS],   "IFA_ADDRESS");
   strcpy(gArray_IFA_Type[IFA_LOCAL],     "IFA_LOCAL");
   strcpy(gArray_IFA_Type[IFA_LABEL],     "IFA_LABEL");
   strcpy(gArray_IFA_Type[IFA_BROADCAST], "IFA_BROADCAST");
   strcpy(gArray_IFA_Type[IFA_ANYCAST],   "IFA_ANYCAST");
   strcpy(gArray_IFA_Type[IFA_CACHEINFO], "IFA_CACHEINFO");
   strcpy(gArray_IFA_Type[IFA_MULTICAST], "IFA_MULTICAST");
   strcpy(gArray_IFA_Type[IFA_FLAGS],     "IFA_FLAGS");


}

char* ArrayFindItem(struct sArrayFamily  *a_pArray,
                    int                   a_ID )
{
   int   ii       = 0;
   char* pItem    = gStringError;

   for( ii = 0; ii < a_pArray->Max; ii++)
   {
      if( a_pArray->Array[ii].ID == a_ID )
      {
         pItem = a_pArray->Array[ii].Name;
         break;
      }
   }

   return pItem;
}

void ArraySetItem( struct sArrayFamily  *a_pArray,
                   int                  a_Id,
                   char*                a_KeyName)
{

   strcpy(a_pArray->Array[a_pArray->Number].Name,a_KeyName);

   a_pArray->Array[a_pArray->Number].ID = a_Id;

   a_pArray->Number++;
}

void setArrayAddressFamily()
{
   memset(&gArrayFamily,0,sizeof(gArrayFamily));

   gArrayFamily.Max = ARRAY_ADDRESS_FAMILY_MAX;

   ArraySetItem(&gArrayFamily ,AF_UNSPEC,"AF_UNSPEC");

   //Address families.
   ArraySetItem(&gArrayFamily ,AF_UNSPEC,    "AF_UNSPEC");
   ArraySetItem(&gArrayFamily ,AF_LOCAL,     "AF_LOCAL");
   ArraySetItem(&gArrayFamily ,AF_UNIX,      "AF_UNIX");
   ArraySetItem(&gArrayFamily ,AF_FILE,      "AF_FILE");
   ArraySetItem(&gArrayFamily ,AF_INET,      "AF_INET");
   ArraySetItem(&gArrayFamily ,AF_AX25,      "AF_AX25");
   ArraySetItem(&gArrayFamily ,AF_IPX,       "AF_IPX");
   ArraySetItem(&gArrayFamily ,AF_APPLETALK, "AF_APPLETALK");
   ArraySetItem(&gArrayFamily ,AF_NETROM,    "AF_NETROM");
   ArraySetItem(&gArrayFamily ,AF_BRIDGE,    "AF_BRIDGE");
   ArraySetItem(&gArrayFamily ,AF_ATMPVC,    "AF_ATMPVC");
   ArraySetItem(&gArrayFamily ,AF_X25,       "AF_X25");
   ArraySetItem(&gArrayFamily ,AF_INET6,     "AF_INET6");
   ArraySetItem(&gArrayFamily ,AF_ROSE,      "AF_ROSE");
   ArraySetItem(&gArrayFamily ,AF_DECnet,    "AF_DECnet");
   ArraySetItem(&gArrayFamily ,AF_NETBEUI,   "AF_NETBEUI");
   ArraySetItem(&gArrayFamily ,AF_SECURITY,  "AF_SECURITY");
   ArraySetItem(&gArrayFamily ,AF_KEY,       "AF_KEY");
   ArraySetItem(&gArrayFamily ,AF_NETLINK,   "AF_NETLINK");
   ArraySetItem(&gArrayFamily ,AF_ROUTE,     "AF_ROUTE");
   ArraySetItem(&gArrayFamily ,AF_PACKET,    "AF_PACKET");
   ArraySetItem(&gArrayFamily ,AF_ASH,       "AF_ASH");
   ArraySetItem(&gArrayFamily ,AF_ECONET,    "AF_ECONET");
   ArraySetItem(&gArrayFamily ,AF_ATMSVC,    "AF_ATMSVC");
   ArraySetItem(&gArrayFamily ,AF_RDS,       "AF_RDS");
   ArraySetItem(&gArrayFamily ,AF_SNA,       "AF_SNA");
   ArraySetItem(&gArrayFamily ,AF_IRDA,      "AF_IRDA");
   ArraySetItem(&gArrayFamily ,AF_PPPOX,     "AF_PPPOX");
   ArraySetItem(&gArrayFamily ,AF_WANPIPE,   "AF_WANPIPE");
   ArraySetItem(&gArrayFamily ,AF_LLC,       "AF_LLC");
   ArraySetItem(&gArrayFamily ,AF_CAN,       "AF_CAN");
   ArraySetItem(&gArrayFamily ,AF_TIPC,      "AF_TIPC");
   ArraySetItem(&gArrayFamily ,AF_BLUETOOTH, "AF_BLUETOOTH");
   ArraySetItem(&gArrayFamily ,AF_IUCV,      "AF_IUCV");
   ArraySetItem(&gArrayFamily ,AF_RXRPC,     "AF_RXRPC");
   ArraySetItem(&gArrayFamily ,AF_ISDN,      "AF_ISDN");
   ArraySetItem(&gArrayFamily ,AF_PHONET,    "AF_PHONET");
   ArraySetItem(&gArrayFamily ,AF_IEEE802154,"AF_IEEE802154");
   ArraySetItem(&gArrayFamily ,AF_CAIF,      "AF_CAIF");
   ArraySetItem(&gArrayFamily ,AF_ALG,       "AF_ALG");
   ArraySetItem(&gArrayFamily ,AF_NFC,       "AF_NFC");
   ArraySetItem(&gArrayFamily ,AF_VSOCK,     "AF_VSOCK");
   ArraySetItem(&gArrayFamily ,AF_MAX,      "AF_MAX");

}
//****************************************
//*
//****************************************
char* getRTM_Type(int a_Type)
{
   int    ii         = 0;
   char* pRetvalue   = 0;

   if( ii < RTM_MAX )
   {
      pRetvalue = gArray_RTM_Type[ii];
   }

   return pRetvalue;

}
//****************************************
//*
//****************************************
char* getIFA_Type(int a_Type)
{
   char* pRetvalue = 0;


   if( a_Type < __IFA_MAX )
   {
      pRetvalue = gArray_IFA_Type[a_Type];
   }

   return pRetvalue;
}

//******************************************************
//*
//******************************************************
int read_event()
{
   int                  status = 0;
   int                  ret = 0;
   char                 vBuf[4096];
   struct iovec         iov = { vBuf, sizeof vBuf };
   struct sockaddr_nl   snl;
   struct msghdr        msg = { (void *) &snl, sizeof snl, &iov, 1, NULL, 0, 0 };
   struct nlmsghdr *    pNlmsghdr;
   struct ifinfomsg *   pIfinfomsg;
   char                 vStrMsg[1024];

   /*
   printf ("\nread_event: sizeof(struct nlmsghdr)== %d\n",
           (int)sizeof(struct nlmsghdr));

   printf ("read_event: sizeof(struct ifinfomsg)== %d\n",
           (int)sizeof(struct ifinfomsg));
   printf ("read_event: sizeof(struct rtattr )== %d\n",
           (int)sizeof(struct rtattr ));
    */
   status = recvmsg (gInfoSocket.Nl_socket, &msg, 0);

   printf ("read_event_1 : >>>>>>>>>>>>>>>>>>>>>>\n");
   printf ("read_event_2 : recvmsg() == %d\n", status);

   if (status < 0)
   {
      /* Socket non-blocking so bail out once we have read everything */
      if (errno == EWOULDBLOCK || errno == EAGAIN)
         return ret;

      /* Anything else is an error */
      printf ("read_event_3 : Error recvmsg: %d\n", status);
      perror ("read_event_4 : Error: ");
      return status;
   }

   if (status == 0)
   {
      printf ("read_event_5 : EOF\n");
   }

   pNlmsghdr = (struct nlmsghdr *) vBuf;

   int vPayLoad = 0;
   vPayLoad= NLMSG_PAYLOAD(pNlmsghdr,0);

   printf ("read_event_6 : nlmsg_len = %d nlmsg_flags=%#x nlmsg_pid=%d nlmsg_seq=%d nlmsg_type= %d-%s NLMSG_PAYLOAD=%d\n \n",
           (int)pNlmsghdr->nlmsg_len,
           (int)pNlmsghdr->nlmsg_flags,
           (int)pNlmsghdr->nlmsg_pid,
           (int)pNlmsghdr->nlmsg_seq,
           (int)pNlmsghdr->nlmsg_type , getRTM_Type(pNlmsghdr->nlmsg_type),
           vPayLoad);

   struct rtattr *pRta = 0;
   char* pChar=0;


   // We need to handle more than one message per 'recvmsg'
   for (    pNlmsghdr = (struct nlmsghdr *) vBuf;
            NLMSG_OK (pNlmsghdr,(unsigned int) status);
            pNlmsghdr = NLMSG_NEXT (pNlmsghdr, status))
   {
      printf ("read_event_7 : >>>>>>>>>>>>>>>>>>>>>>\n");

      //Finish reading
      if (pNlmsghdr->nlmsg_type == NLMSG_DONE)
      {
         return ret;
      }
      // Message is some kind of error
      else if (pNlmsghdr->nlmsg_type == NLMSG_ERROR)
      {
         printf ("read_event_8 : Message is an error - decode TBD\n");
         return -1;        // Error
      }
      else if (pNlmsghdr->nlmsg_type == RTM_NEWADDR )
      {
         RTM_NEWADDR_Check(pNlmsghdr);
      }
      else if (pNlmsghdr->nlmsg_type == RTM_DELADDR )
      {
         RTM_DELADDR_Check(pNlmsghdr);
      }
      else if (pNlmsghdr->nlmsg_type == RTM_GETADDR )
      {
         RTM_GETADDR_Check(pNlmsghdr);
      }
      else if (pNlmsghdr->nlmsg_type == RTM_NEWLINK )
      {
         RTM_NEWLINK_Check(pNlmsghdr);
      }
      else if (pNlmsghdr->nlmsg_type == RTM_DELLINK )
      {
         RTM_DELLINK_Check(pNlmsghdr);
      }
      else if (pNlmsghdr->nlmsg_type == RTM_GETLINK )
      {
         RTM_GETLINK_Check(pNlmsghdr);
      }
      else if (pNlmsghdr->nlmsg_type == RTM_SETLINK )
      {
         RTM_SETLINK_Check(pNlmsghdr);
      }
      else
      {
         printf("\n read_event_9 : else  %d,%s",pNlmsghdr->nlmsg_type,getRTM_Type(pNlmsghdr->nlmsg_type));
      }


      /*
      //PrintMessageType(pNlmsghdr);
      PrintMessageFlag(pNlmsghdr);
      PrintIfinfomsg(pNlmsghdr);


      for( pRta = NLMSG_DATA(pNlmsghdr);
            ((char*)pRta) < vBuf + 4096;
            )
      {
//         printf("pRta->rta_len = %d pRta->rta_type=%d \n",
//                (int)pRta->rta_len,(int)pRta->rta_type);

         if(      ( ( pRta->rta_len == 0) && (pRta->rta_type > 0 ))
               || ( ( pRta->rta_len > 0 ) && ( pRta->rta_len < 20 ) )
           )
         {
            //snprintf(vStrMsg,pRta->rta_len,"%s",                     (char*)(pRta + sizeof(struct rtattr)));

            memcpy(  vStrMsg,
                     (char*)(pRta + sizeof(struct rtattr)),
                     pRta->rta_len);

              vStrMsg[pRta->rta_len] = 0;

              printf("pRta->rta_len = %d pRta->rta_type=%d \n=>%s.\n",
                     (int)pRta->rta_len,(int)pRta->rta_type,vStrMsg);

              printf("%s\n",vStrMsg);
         }
         pChar = (char*)pRta;
         pRta =   (struct rtattr *) ( pChar + sizeof(struct rtattr)+pRta->rta_len);
      }
       */

      printf ("read_event_10 : <<<<<<<<<<<<<<<<<<<<<<<\n");
   }

   fflush(0);

   printf ("read_event_12 : <<<<<<<<<<<<<<<<<<<<\n");

   return ret;
}
//******************************************************
//*
//******************************************************
void PrintMessageType(struct nlmsghdr * a_pNlmsghdr)
{

   if(   (a_pNlmsghdr->nlmsg_type == RTM_NEWLINK)
   || (a_pNlmsghdr->nlmsg_type == RTM_DELLINK)

   || (a_pNlmsghdr->nlmsg_type == RTM_BASE)

   || (a_pNlmsghdr->nlmsg_type == RTM_GETLINK)
   || (a_pNlmsghdr->nlmsg_type == RTM_SETLINK)

   || (a_pNlmsghdr->nlmsg_type == RTM_NEWADDR)
   || (a_pNlmsghdr->nlmsg_type == RTM_DELADDR)
   || (a_pNlmsghdr->nlmsg_type == RTM_GETADDR)

   || (a_pNlmsghdr->nlmsg_type == RTM_NEWROUTE)
   || (a_pNlmsghdr->nlmsg_type == RTM_DELROUTE)
   || (a_pNlmsghdr->nlmsg_type == RTM_GETROUTE)

   || (a_pNlmsghdr->nlmsg_type == RTM_NEWNEIGH)
   || (a_pNlmsghdr->nlmsg_type == RTM_DELNEIGH)
   || (a_pNlmsghdr->nlmsg_type == RTM_GETNEIGH)

   || (a_pNlmsghdr->nlmsg_type == RTM_NEWPREFIX)

   || (a_pNlmsghdr->nlmsg_type == RTM_NEWNETCONF)
   || (a_pNlmsghdr->nlmsg_type == RTM_GETNETCONF)
   )
   {
      printf ("\nnlmsg_type %s = %d\n",
              getRTM_Type(a_pNlmsghdr->nlmsg_type),
              a_pNlmsghdr->nlmsg_type);
   }

}
//******************************************************
//*
//******************************************************
void PrintIfinfomsg(struct nlmsghdr * a_pNlmsghdr)
{
   struct ifinfomsg *  pIfinfomsg = NLMSG_DATA (a_pNlmsghdr);

   printf("\nifi_change = %12d \nifi_family = %12d \nifi_index  = %12d \nifi_type   = %12d \n\n",
          (int)pIfinfomsg->ifi_change,
          (int)pIfinfomsg->ifi_family,
          (int)pIfinfomsg->ifi_index,
          (int)pIfinfomsg->ifi_type);

}
//******************************************************
//*
//******************************************************
void RTM_NEWADDR_Check(struct nlmsghdr * a_pNlmsghdr)
{
   int                        is_del         = 0;
   char                       address[48];
   char                       ifname[IFNAMSIZ];
   struct   ifaddrmsg*        ifa;
   struct   rtattr*           rta;
   int                        ifa_len;
   struct   lan_addr_s *      lan_addr;

   address[0] = '\0';
   ifname[0] = '\0';

   printf("RTM_NEWADDR_Check\n");

   ifa = (struct ifaddrmsg *)NLMSG_DATA(a_pNlmsghdr);
   rta = (struct rtattr *)IFA_RTA(ifa);
   ifa_len = IFA_PAYLOAD(a_pNlmsghdr);
   printf( "RTM_NEWADDR_Check : %s %s index=%d fam=%d prefixlen=%d flags=%d scope=%d\n",
           "ProcessInterfaceWatchNotify", is_del ? "RTM_DELADDR" : "RTM_NEWADDR",
           ifa->ifa_index, ifa->ifa_family, ifa->ifa_prefixlen,
           ifa->ifa_flags, ifa->ifa_scope);


   for(;RTA_OK(rta, ifa_len); rta = RTA_NEXT(rta, ifa_len))
   {
      /*RTA_DATA(rta)*/
      /*rta_type : IFA_ADDRESS, IFA_LOCAL, etc. */
      char tmp[128];
      memset(tmp, 0, sizeof(tmp));

      switch(rta->rta_type)
      {
         case IFA_ADDRESS:
         case IFA_LOCAL:
         case IFA_BROADCAST:
         case IFA_ANYCAST:
            inet_ntop(ifa->ifa_family, RTA_DATA(rta), tmp, sizeof(tmp));
            //if(rta->rta_type == IFA_ADDRESS)
            strncpy(address, tmp, sizeof(address));
            break;
         case IFA_LABEL:
            strncpy(tmp, RTA_DATA(rta), sizeof(tmp));
            strncpy(ifname, tmp, sizeof(ifname));
            break;
         case IFA_CACHEINFO:
         {
            struct ifa_cacheinfo *cache_info;
            cache_info = RTA_DATA(rta);
            snprintf(tmp, sizeof(tmp), "valid=%u prefered=%u",
                     cache_info->ifa_valid, cache_info->ifa_prefered);
         }
         break;
         default:
            strncpy(tmp, "*unknown*", sizeof(tmp));
      }// switch(..)

      printf("RTM_NEWADDR_Check : Attribut rta_type=%d-%s '%s' rta_len=%d \n",
             rta->rta_type,
             getIFA_Type(rta->rta_type),
             tmp,
             rta->rta_len);
   }// for(...)
   printf("RTM_NEWADDR_Check : OUT %s: %s/%d %s\n",
          is_del ? "RTM_DELADDR" : "RTM_NEWADDR",
          address, ifa->ifa_prefixlen, ifname);



}
//******************************************************
//*
//******************************************************
void RTM_DELADDR_Check(struct nlmsghdr * a_pNlmsghdr)
{
   printf("RTM_DELADDR_Check\n");
}
//******************************************************
//*
//******************************************************
void RTM_GETADDR_Check(struct nlmsghdr * a_pNlmsghdr)
{
   printf("RTM_GETADDR_Check\n");

}
//******************************************************
//*
//******************************************************
void RTM_NEWLINK_Check(struct nlmsghdr * a_pNlmsghdr)
{

#define NEWLINK_LEN_BUFFER_ADDR 128

   static unsigned vIfi_flags = 0;

   struct ifinfomsg *pIfinfomsg = (struct ifinfomsg*) NLMSG_DATA(a_pNlmsghdr);
   struct   rtattr*           pRtAttr;
   int                        vLen;
   struct   lan_addr_s *      lan_addr;
   char                       vBuffer[NEWLINK_LEN_BUFFER_ADDR];
   char                       vBufferData[NEWLINK_LEN_BUFFER_ADDR];
   unsigned int               vMTU = 0;
   int                        vLink = 0;
   struct rtnl_link_stats     vRrtnl_link_stats;

   pRtAttr = (struct rtattr *)IFLA_RTA(pIfinfomsg);

   vLen = IFLA_PAYLOAD(a_pNlmsghdr);

#define CHECK_FLAG(a_FLAG) if( (pIfinfomsg->ifi_flags & a_FLAG ) != (vIfi_flags & a_FLAG) )

   printf("\n RTM_NEWLINK_Check \n");
   printf("\t %-15s : %10d %s\n", "ifi_family",(int)pIfinfomsg->ifi_family , ArrayFindItem(&gArrayFamily,pIfinfomsg->ifi_family) );
   printf("\t %-15s : %10d \n",   "ifi_index" ,(int)pIfinfomsg->ifi_index);
   printf("\t %-15s : %10d \n",   "ifi_type" ,(int)pIfinfomsg->ifi_type);
   printf("\t %-15s : %10d \n",   "ifi_change",(int)pIfinfomsg->ifi_change);
   printf("\t %-30s : %10d \n",   "ifi_flags" ,(int)pIfinfomsg->ifi_flags);

   CHECK_FLAG(IFF_UP)            printf(" %-20s = %d \n" , "IFF_UP",          (pIfinfomsg->ifi_flags & IFF_UP)          != 0 );
   CHECK_FLAG(IFF_BROADCAST)     printf(" %-20s = %d \n" , "IFF_BROADCAST",   (pIfinfomsg->ifi_flags & IFF_BROADCAST)   != 0 );
   CHECK_FLAG(IFF_DEBUG)         printf(" %-20s = %d \n" , "IFF_DEBUG",       (pIfinfomsg->ifi_flags & IFF_DEBUG )      != 0 );
   CHECK_FLAG(IFF_LOOPBACK)      printf(" %-20s = %d \n" , "IFF_LOOPBACK",    (pIfinfomsg->ifi_flags & IFF_LOOPBACK)    != 0 );
   CHECK_FLAG(IFF_POINTOPOINT)   printf(" %-20s = %d \n" , "IFF_POINTOPOINT", (pIfinfomsg->ifi_flags & IFF_POINTOPOINT) != 0 );
   CHECK_FLAG(IFF_RUNNING)       printf(" %-20s = %d \n" , "IFF_RUNNING",     (pIfinfomsg->ifi_flags & IFF_RUNNING)     != 0 );
   CHECK_FLAG(IFF_NOARP)         printf(" %-20s = %d \n" , "IFF_NOARP",       (pIfinfomsg->ifi_flags & IFF_NOARP)       != 0 );
   CHECK_FLAG(IFF_PROMISC)       printf(" %-20s = %d \n" , "IFF_PROMISC",     (pIfinfomsg->ifi_flags & IFF_PROMISC)     != 0 );
   CHECK_FLAG(IFF_NOTRAILERS)    printf(" %-20s = %d \n" , "IFF_NOTRAILERS",  (pIfinfomsg->ifi_flags & IFF_NOTRAILERS)  != 0 );
   CHECK_FLAG(IFF_ALLMULTI)      printf(" %-20s = %d \n" , "IFF_ALLMULTI",    (pIfinfomsg->ifi_flags & IFF_ALLMULTI)    != 0 );
   CHECK_FLAG(IFF_MASTER)        printf(" %-20s = %d \n" , "IFF_MASTER",      (pIfinfomsg->ifi_flags & IFF_MASTER)      != 0 );
   CHECK_FLAG(IFF_SLAVE)         printf(" %-20s = %d \n" , "IFF_SLAVE",       (pIfinfomsg->ifi_flags & IFF_SLAVE)       != 0 );
   CHECK_FLAG(IFF_MULTICAST)     printf(" %-20s = %d \n" , "IFF_MULTICAST",   (pIfinfomsg->ifi_flags & IFF_MULTICAST)   != 0 );
   CHECK_FLAG(IFF_PORTSEL)       printf(" %-20s = %d \n" , "IFF_PORTSEL",     (pIfinfomsg->ifi_flags & IFF_PORTSEL)     != 0 );
   CHECK_FLAG(IFF_AUTOMEDIA)     printf(" %-20s = %d \n" , "IFF_AUTOMEDIA",   (pIfinfomsg->ifi_flags & IFF_AUTOMEDIA)   != 0 );
   CHECK_FLAG(IFF_DYNAMIC)       printf(" %-20s = %d \n" , "IFF_DYNAMIC",     (pIfinfomsg->ifi_flags & IFF_DYNAMIC)     != 0 );
   CHECK_FLAG(IFF_LOWER_UP)      printf(" %-20s = %d \n" , "IFF_LOWER_UP",    (pIfinfomsg->ifi_flags & IFF_LOWER_UP)    != 0 );
   CHECK_FLAG(IFF_DORMANT)       printf(" %-20s = %d \n" , "IFF_DORMANT",     (pIfinfomsg->ifi_flags & IFF_DORMANT)     != 0 );
   CHECK_FLAG(IFF_ECHO)          printf(" %-20s = %d \n" , "IFF_ECHO",        (pIfinfomsg->ifi_flags & IFF_ECHO)         != 0 );

   vIfi_flags = pIfinfomsg->ifi_flags;

   printf("\n");


   for(;RTA_OK(pRtAttr, vLen); pRtAttr = RTA_NEXT(pRtAttr, vLen))
   {
      //RTA_DATA(rta)
      //rta_type : IFA_ADDRESS, IFA_LOCAL, etc.
      memset(vBuffer, 0, sizeof(vBuffer));
      memset(vBufferData, 0, sizeof(vBufferData));

      switch(pRtAttr->rta_type)
      {
         case IFLA_ADDRESS:
         {
            inet_ntop(pIfinfomsg->ifi_family, RTA_DATA(pRtAttr), vBufferData, sizeof(vBufferData));

            //strncpy(vBufferData, RTA_DATA(pRtAttr), sizeof(vBufferData));
            printf("%-15s = %s \n", "IFLA_ADDRESS",vBufferData);
            break;
         }
         case IFLA_BROADCAST:
         {
            inet_ntop(pIfinfomsg->ifi_family, RTA_DATA(pRtAttr), vBufferData, sizeof(vBufferData));
            //strncpy(vBufferData, RTA_DATA(pRtAttr), sizeof(vBufferData));
            printf("%-15s = %s \n", "IFLA_BROADCAST", vBufferData);
            break;
         }
         case IFLA_IFNAME:
            strncpy(vBufferData, RTA_DATA(pRtAttr), sizeof(vBufferData));
            printf("%-15s = %s \n", "IFLA_IFNAME", vBufferData);
            break;
         case IFLA_MTU:
         {
            unsigned int* pMtu = (unsigned int*)RTA_DATA(pRtAttr);
            vMTU = *pMtu;
            printf("%-15s = %u \n","IFLA_MTU", vMTU);
            break;
         }
         case IFLA_LINK:
         {
            vLink = RTA_DATA(pRtAttr);
            printf("%-15s = %d \n",",IFLA_LINK",vLink);
         }
         break;
         case IFLA_QDISC:
         {
            strncpy(vBufferData, RTA_DATA(pRtAttr), sizeof(vBufferData));
            printf("%-15s = %s \n", "IFLA_QDISC", vBufferData);

            break;
         }
         case IFLA_STATS:
         {
            //            struct ifa_cacheinfo *cache_info;
            //            cache_info = RTA_DATA(pRtAttr);
            //            snprintf(tmp, sizeof(vBuffer), "valid=%u prefered=%u",
            //                     cache_info->ifa_valid, cache_info->ifa_prefered);
            printf("%-15s =  \n","IFLA_STATS");

            break;
         }
         default:
            strncpy(vBuffer, "*unknown*", sizeof(vBuffer));
      }
   }
   printf("RTM_NEWLINK_Check\n");
}
//******************************************************
//*
//******************************************************
void RTM_DELLINK_Check(struct nlmsghdr * a_pNlmsghdr)
{
   printf("RTM_DELLINK_Check\n");
}
//******************************************************
//*
//******************************************************
void RTM_GETLINK_Check(struct nlmsghdr * a_pNlmsghdr)
{
   printf("RTM_GETLINK_Check\n");
}
//******************************************************
//*
//******************************************************
void RTM_SETLINK_Check(struct nlmsghdr * a_pNlmsghdr)
{
   printf("RTM_SETLINK_Check\n");
}


void SetArrays()
{
   SetArrayRTMTypes();
   setArrayAddressFamily();

}

void IpLink(int a_UP )
{
   int vRetcode = 0;

   struct {
         struct nlmsghdr  Nh;
         struct ifinfomsg Ifinfomsg;
         char             Attrbuf[20];// 512
   } req;

   struct rtattr  *pRtAttr = 0;
   int   vStateLink =  a_UP ? 1 : 0;


   memset(&req, 0, sizeof(req));

   req.Nh.nlmsg_len        = NLMSG_LENGTH(sizeof(struct ifinfomsg));
   req.Nh.nlmsg_flags      =  NLM_F_REQUEST; // NLM_F_REPLACE; NLM_F_REQUEST
   req.Nh.nlmsg_type       = RTM_NEWLINK; // RTM_NEWLINK;

   req.Ifinfomsg.ifi_family = AF_UNSPEC;
   req.Ifinfomsg.ifi_index  = 5; // eth1
   req.Ifinfomsg.ifi_change = 0xffffffff; /* ??? */
   req.Ifinfomsg.ifi_flags |= IFF_UP;

   pRtAttr = (struct rtattr *)(((char *) &req) +
   NLMSG_ALIGN(req.Nh.nlmsg_len));

   pRtAttr->rta_type    = IFLA_LINK;
   pRtAttr->rta_len     = RTA_LENGTH(sizeof(int));
   req.Nh.nlmsg_len     = NLMSG_ALIGN(req.Nh.nlmsg_len) +
   RTA_LENGTH(sizeof(vStateLink));

   memcpy(RTA_DATA(pRtAttr), &vStateLink, sizeof(vStateLink));

   // send(gInfoSocket.Nl_socket, &req, req.Nh.nlmsg_len,0);

   // prepare struct msghdr for sending.
   struct iovec iov  = { &req.Nh, req.Nh.nlmsg_len };
   struct msghdr msg    = {   &gInfoSocket.Sockaddr_nl,
   sizeof(gInfoSocket.Sockaddr_nl),
   &iov,
   1,
   NULL, 0, 0 };

   // send netlink message to kernel.
   vRetcode = sendmsg(gInfoSocket.Nl_socket, &msg, 0);

   printf("IpLink(%d) vRetcode  = %d \n",a_UP, vRetcode);

}

