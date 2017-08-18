/*
 * net_envent.h
 *
 *  Created on: Jun 15, 2017
 *      Author: hico
 */

#ifndef NET_ENVENT_H_
#define NET_ENVENT_H_


#include <netinet/in.h>
#include <net/if.h>
#include <sys/queue.h>
#include <linux/netlink.h>



#define ARG_LINK_UP     "lu"
#define ARG_LINK_DOWN   "ld"
#define ARG_LINK_INFO   "li"


/* structure and list for storing lan addresses
 * with ascii representation and mask */
struct lan_addr_s {
   char ifname[IFNAMSIZ];  /* example: eth0 */
   unsigned int index;     /* use if_nametoindex() */
   char str[16];  /* example: 192.168.0.1 */
   struct in_addr addr, mask; /* ip/mask */
#ifdef MULTIPLE_EXTERNAL_IP
   char ext_ip_str[16];
   struct in_addr ext_ip_addr;
#endif
   //LIST_ENTRY(lan_addr_s) list;
};
LIST_HEAD(lan_addr_list, lan_addr_s);

struct sInfoSocket
{
      fd_set               ReadFileDescriptor;
      fd_set               WriteFileDescriptor;
      struct timeval       Timeval;
      struct sockaddr_nl   Sockaddr_nl;
      int                  Nl_socket ;
};
struct sArrayItem
{
      int   ID;
      char  Name[100];
};


#define ARRAY_ADDRESS_FAMILY_MAX ( PF_MAX + 3 )

struct sArrayFamily
{
      int               Number;
      struct sArrayItem Array[ARRAY_ADDRESS_FAMILY_MAX];
      int Max;
};




void PrintUsage(int argc, char *argv[]);


int LoopEvent(int argc, char *argv[]);

void  SetArrays();
void  SetArray();
char* getRTM_Type(int a_Type);
char* getIFA_Type(int a_Type);

int   read_event ();

void PrintMessageType(struct nlmsghdr * a_a_pNlmsghdr);
void PrintIfinfomsg(struct nlmsghdr * a_pNlmsghdr);

void RTM_NEWADDR_Check(struct nlmsghdr * a_pNlmsghdr);
void RTM_DELADDR_Check(struct nlmsghdr * a_pNlmsghdr);
void RTM_GETADDR_Check(struct nlmsghdr * a_pNlmsghdr);

void RTM_NEWLINK_Check(struct nlmsghdr * a_pNlmsghdr);
void RTM_DELLINK_Check(struct nlmsghdr * a_pNlmsghdr);
void RTM_GETLINK_Check(struct nlmsghdr * a_pNlmsghdr);
void RTM_SETLINK_Check(struct nlmsghdr * a_pNlmsghdr);

int ip_add();
int ip_del();
int ip_show();
int gateway_add();
int gateway_del();
int gateway_show();

void SetArrayRTMTypes();
void setArrayAddressFamily();
char* ArrayFindItem(struct sArrayFamily  *a_pArray,
                    int                   a_ID );
void ArraySetItem( struct sArrayFamily  *a_pArray,
                   int                  a_Id,
                   char*                a_KeyName);

int ip_link(int argc, char *argv[]);

void CloseSocket();
int util_OpenSocket( );


#endif /* NET_ENVENT_H_ */
