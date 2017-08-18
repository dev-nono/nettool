/*
 * errno.h
 *
 *  Created on: Aug 11, 2017
 *      Author: hico
 */

#ifndef SRC_LIBERRNO_H_
#define SRC_LIBERRNO_H_



#define NLE_SUCCESS             0
#define NLE_FAILURE             1
#define NLE_INTR                2
#define NLE_BAD_SOCK            3
#define NLE_AGAIN               4
#define NLE_NOMEM               5
#define NLE_EXIST               6
#define NLE_INVAL               7
#define NLE_RANGE               8
#define NLE_MSGSIZE             9
#define NLE_OPNOTSUPP           10
#define NLE_AF_NOSUPPORT        11
#define NLE_OBJ_NOTFOUND        12
#define NLE_NOATTR              13
#define NLE_MISSING_ATTR        14
#define NLE_AF_MISMATCH         15
#define NLE_SEQ_MISMATCH        16
#define NLE_MSG_OVERFLOW        17
#define NLE_MSG_TRUNC           18
#define NLE_NOADDR              19
#define NLE_SRCRT_NOSUPPORT     20
#define NLE_MSG_TOOSHORT        21
#define NLE_MSGTYPE_NOSUPPORT   22
#define NLE_OBJ_MISMATCH        23
#define NLE_NOCACHE             24
#define NLE_BUSY                25
#define NLE_PROTO_MISMATCH      26
#define NLE_NOACCESS            27
#define NLE_PERM                28
#define NLE_PKTLOC_FILE         29
#define NLE_PARSE_ERR           30
#define NLE_NODEV               31
#define NLE_IMMUTABLE           32
#define NLE_DUMP_INTR           33

#define NLE_MAX                 NLE_DUMP_INTR

const char *     nl_geterror(int);
void             nl_perror(int, const char *);
int              nl_syserr2nlerr(int);


#endif

/* SRC_LIBERRNO_H_ */
