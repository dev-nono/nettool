/*
 * util.h
 *
 *  Created on: Aug 11, 2017
 *      Author: hico
 */

#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <sys/select.h>
#include <time.h>


#include "liberrno.h"

#define BUFLEN    4096


#define for_each_nlmsg(n, buf, len)             \
   for (n = (struct nlmsghdr*)buf;              \
        NLMSG_OK(n, (uint32_t)len) && n->nlmsg_type != NLMSG_DONE;   \
        n = NLMSG_NEXT(n, len))

#define for_each_rattr(n, buf, len)             \
   for (n = (struct rtattr*)buf; RTA_OK(n, len); n = RTA_NEXT(n, len))


int      util_get_msg(int fd, struct sockaddr_nl *sa, void *buf, size_t len);
uint32_t util_parse_nl_msg_OK(void *buf, size_t len);
int      util_pton(int domain, void *buf, const char *ip);
char*    util_ntop(int domain, void *buf);


int RequestListDevice(  int                  a_Fd,
                        struct sockaddr_nl   *a_pSockaddr_nl);

#endif /* SRC_UTIL_H_ */
