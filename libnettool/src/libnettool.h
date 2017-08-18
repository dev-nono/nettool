/*
 * libnettool.h
 *
 *  Created on: Jul 21, 2017
 *      Author: hico
 */

#ifndef SRC_LIBNETTOOL_H_
#define SRC_LIBNETTOOL_H_

#include <linux/rtnetlink.h>


int nettool_ip_add();
int nettool_ip_del();
int nettool_ip_show();
int nettool_gateway_add();
int nettool_gateway_del();
int nettool_gateway_show();

//*************************************************************
//             Link
//*************************************************************
typedef void (*nettool_link_notify_func_t)(int* a_pFlagSignaled);

int nettool_link_up(const char* a_DeviceName, int a_UP );
int nettool_link_info(const char* a_DeviceName, struct ifinfomsg* a_Ifinfomsg);
int nettool_link_notify(   const char*                a_DeviceName,
                           nettool_link_notify_func_t a_pfunction,
                           int                        a_FlagToCheck);



#define LINK_IDX(a_LinkInfo) (a_LinkInfo->ifi_index)

#define LINK_ISUP(a_LinkInfo)            ((a_LinkInfo->ifi_flags & IFF_UP)            != 0 )
#define LINK_ISRUNNING(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_RUNNING)       != 0 )
#define LINK_ISBROADCAST(a_LinkInfo)     ((a_LinkInfo->ifi_flags & IFF_BROADCAST)     != 0 )
#define LINK_ISDEBUG(a_LinkInfo)         ((a_LinkInfo->ifi_flags & IFF_DEBUG)         != 0 )
#define LINK_ISLOOPBACK(a_LinkInfo)      ((a_LinkInfo->ifi_flags & IFF_LOOPBACK)      != 0 )
#define LINK_ISPOINTOPOINT(a_LinkInfo)   ((a_LinkInfo->ifi_flags & IFF_POINTOPOINT)   != 0 )
#define LINK_ISNOTRAILERS(a_LinkInfo)    ((a_LinkInfo->ifi_flags & IFF_NOTRAILERS)    != 0 )
#define LINK_ISNOARP(a_LinkInfo)         ((a_LinkInfo->ifi_flags & IFF_NOARP)         != 0 )
#define LINK_ISPROMISC(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_PROMISC)       != 0 )
#define LINK_ISALLMULTI(a_LinkInfo)      ((a_LinkInfo->ifi_flags & IFF_ALLMULTI)      != 0 )
#define LINK_ISMASTER(a_LinkInfo)        ((a_LinkInfo->ifi_flags & IFF_MASTER)        != 0 )
#define LINK_ISSLAVE(a_LinkInfo)         ((a_LinkInfo->ifi_flags & IFF_SLAVE)         != 0 )
#define LINK_ISMULTICAST(a_LinkInfo)     ((a_LinkInfo->ifi_flags & IFF_MULTICAST)     != 0 )
#define LINK_ISPORTSEL(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_PORTSEL)       != 0 )
#define LINK_ISAUTOMEDIA(a_LinkInfo)     ((a_LinkInfo->ifi_flags & IFF_AUTOMEDIA)     != 0 )
#define LINK_ISDYNAMIC(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_DYNAMIC)       != 0 )
#define LINK_ISRUNNING(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_RUNNING)       != 0 )
#define LINK_ISRUNNING(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_RUNNING)       != 0 )
#define LINK_ISRUNNING(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_RUNNING)       != 0 )
#define LINK_ISRUNNING(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_RUNNING)       != 0 )
#define LINK_ISRUNNING(a_LinkInfo)       ((a_LinkInfo->ifi_flags & IFF_RUNNING)       != 0 )



#endif /* SRC_LIBNETTOOL_H_ */
