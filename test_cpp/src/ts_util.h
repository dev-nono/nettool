/*
 * ts_util.h
 *
 *  Created on: Jun 26, 2017
 *      Author: hico
 */

#ifndef TS_UTIL_H_
#define TS_UTIL_H_

#ifdef __cplusplus      /* If this is a C++ compiler, use C linkage */
extern  "C"  {
#endif

#include <time.h>

   struct  timespec  ts_Add (struct  timespec  time1,struct  timespec  time2);
   int               ts_Compare (struct  timespec  time1,struct  timespec  time2);
   double            ts_Float (struct  timespec  time);
   struct  timespec  ts_Subtract (struct  timespec  time1,struct  timespec  time2);

   int               ts_IsEqual(   struct  timespec  time1,struct  timespec  time2);
   int               ts_IsLessThan(   struct  timespec  time1,struct  timespec  time2);
   int               ts_IsGreaterThan(   struct  timespec  time1,struct  timespec  time2);

#ifdef __cplusplus      /* If this is a C++ compiler, use C linkage */
}
#endif


#endif /* TS_UTIL_H_ */
