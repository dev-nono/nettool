/*
 * debug_src.c
 *
 *  Created on: Aug 18, 2017
 *      Author: hico
 */


static
int link_info_request_3( int a_Fd)
{
   typedef struct nl_req_s nl_req_t;

   struct nl_req_s {
     struct nlmsghdr hdr;
     struct rtgenmsg gen;
   };


   struct sockaddr_nl kernel;
   struct msghdr rtnl_msg;
   struct iovec io;
   nl_req_t req;


   memset(&rtnl_msg, 0, sizeof(rtnl_msg));
   memset(&kernel, 0, sizeof(kernel));
   memset(&req, 0, sizeof(req));


   kernel.nl_family = AF_NETLINK; /* fill-in kernel address (destination) */

   req.hdr.nlmsg_len       = NLMSG_LENGTH(sizeof(struct rtgenmsg));
   req.hdr.nlmsg_type      = RTM_GETLINK + RTM_BASE; //  RTM_GETLINK;
   req.hdr.nlmsg_flags     = NLM_F_REQUEST | NLM_F_DUMP | NLM_F_ACK | NLM_F_ROOT;
   req.hdr.nlmsg_seq       = 1;
   req.hdr.nlmsg_pid       = getpid();
   req.gen.rtgen_family    = AF_PACKET; /*  no preferred AF, we will get *all* interfaces */

   io.iov_base          = &req;
   io.iov_len           = req.hdr.nlmsg_len;
   rtnl_msg.msg_iov     = &io;
   rtnl_msg.msg_iovlen  = 1;
   rtnl_msg.msg_name    = &kernel;
   rtnl_msg.msg_namelen = sizeof(kernel);

   sendmsg(a_Fd, (struct msghdr *) &rtnl_msg, 0);

   return 0;
}
//*********************************************************************
//*
//*********************************************************************
static
int link_info_request_1( int            a_Fd,
                       const char*    a_DeviceName)
{

  int ssize;

   memset(&LinkReq, 0, sizeof(LinkReq));

   LinkReq.Nh.nlmsg_len          = NLMSG_LENGTH(sizeof(LinkReq.Ifinfomsg));
   LinkReq.Nh.nlmsg_type         = RTM_GETLINK + RTM_BASE; // RTM_GETLINK + RTM_BASE
   LinkReq.Nh.nlmsg_flags        = NLM_F_REQUEST | NLM_F_DUMP | NLM_F_ROOT; //NLM_F_ROOT;
   LinkReq.Nh.nlmsg_pid          = getpid();
   LinkReq.Nh.nlmsg_seq          = 1;
   LinkReq.Ifinfomsg.ifi_family  = AF_UNSPEC;

   ssize = send( a_Fd, &LinkReq, sizeof(LinkReq), 0);
   if (ssize == -1) {
      perror("link_info_request_1 : error send");
      return -1;
   }

   return 0;
}

/**
 * @brief Receive and process the rtnetlink response (containing the list
 * of interfaces).
 *
 * @param[in] sd rtnetlink socket descriptor to be used for communication.
 *
 * @return 0 on success; -1 on failure.
 */
static int32_t   link_info_receive( int32_t sd )
{
   int ii = 0;
   int jj = 0;
   struct ifinfomsg *ifinfomsg;
   struct nlmsghdr *nh;
   struct nlmsgerr *nlme;
   struct rtattr *rta;
   struct {
         struct nlmsghdr nh;
         uint8_t payload[1024];
   } resp;
   ssize_t ssize;
   int32_t len;
   int32_t attr_len;

   ssize = recv(sd, &resp, sizeof(resp), 0);
   if (ssize == -1) {
      printf("link_info_receive : error recv errno=%d %s", errno, strerror(errno));
      return -1;
   }

   nh = &(resp.nh);
   if (nh->nlmsg_type == NLMSG_ERROR) {
      nlme = NLMSG_DATA(nh);
      fprintf(stderr, "link_info_receive : Got error NLMSG_ERROR , %d %s.\n",
              nlme->error,
              nl_geterror(nlme->error));
      return -1;
   }

   len = ssize;
   printf("Available interfaces: ssize = %d\n",(int)ssize);
   for (;NLMSG_OK(&(resp.nh), len); nh = NLMSG_NEXT(nh, len) ,ii++)
   {
      printf("ii = %d\n",ii);

      ifinfomsg  = NLMSG_DATA(nh);
      rta        = IFLA_RTA(ifinfomsg);
      attr_len   = IFLA_PAYLOAD(nh);

      for (; RTA_OK(rta, attr_len); rta = RTA_NEXT(rta, attr_len),jj++)
      {
         printf("jj = %d\n",jj);

         printf("rta->rta_type  = %d \n", (int)rta->rta_type);

         if (rta->rta_type == IFLA_IFNAME)
         {
            printf("IFLA_IFNAME = %s ", (char *)RTA_DATA(rta));
         }

         if (rta->rta_type == IFLA_LINK)
         {
            int vLink = RTA_DATA(rta);
            printf("%-15s = %d \n",",IFLA_LINK",vLink);
         }
      }
      if (nh->nlmsg_type == NLMSG_DONE)
      {
         printf("ii = %d - jj = %d\n",ii , jj);
         break;
      }
   }

   printf("\n");

   return 0;
}
