/*
 * ts_util.c
 *
 *  Created on: Jun 26, 2017
 *      Author: hico
 */

#include  "ts_util.h"         /* "timespec" manipulation functions. */

/*!*****************************************************************************
Procedure:    tsAdd ()
Purpose:       Function tsAdd() returns the sum of two TIMESPECs.
Invocation:    result = tsAdd (time1, time2) ;

    where

        <time1>      - I
            is the first operand, a time represented by a POSIX TIMESPEC
            structure.
        <time2>      - I
            is the second operand, a time represented by a POSIX TIMESPEC
            structure.
        <result>  - O
            returns, in a POSIX TIMESPEC structure, the sum of TIME1 and
            TIME2.

*******************************************************************************/
struct  timespec  ts_Add (
        struct  timespec  time1,
        struct  timespec  time2)

{    /* Local variables. */
    struct  timespec  result ;
/* Add the two times together. */

    result.tv_sec = time1.tv_sec + time2.tv_sec ;
    result.tv_nsec = time1.tv_nsec + time2.tv_nsec ;
    if (result.tv_nsec >= 1000000000L) {     /* Carry? */
        result.tv_sec++ ;  result.tv_nsec = result.tv_nsec - 1000000000L ;
    }

    return (result) ;
}
/*!*****************************************************************************

*!*****************************************************************************/
int ts_IsGreaterThan(   struct  timespec  time1,
                        struct  timespec  time2)
{
      return ts_Compare(time1,time2)  == 1;
}
/*!*****************************************************************************

*!*****************************************************************************/
int ts_IsLessThan(   struct  timespec  time1,
                        struct  timespec  time2)
{
      return ts_Compare(time1,time2)  == -1;
}
/*!*****************************************************************************

*!*****************************************************************************/
int ts_IsEqual(   struct  timespec  time1,
                  struct  timespec  time2)
{
      return ts_Compare(time1,time2)  == 0;
}

/*!*****************************************************************************
Procedure:     tsCompare ()
Purpose:       Function tsCompare() compares two TIMESPECs.
Invocation:    comparison = tsCompare (time1, time2) ;
    where
        <time1>      - I
            is a time represented by a POSIX TIMESPEC structure.
        <time2>      - I
            is another time represented by a POSIX TIMESPEC structure.
        <comparison> - O
            returns an integer indicating how the times compare:
                    -1  if TIME1 < TIME2
                     0  if TIME1 = TIME2
                    +1  if TIME1 > TIME2

*******************************************************************************/
int  ts_Compare (
        struct  timespec  time1,
        struct  timespec  time2)
{

    if (time1.tv_sec < time2.tv_sec)
        return (-1) ;            /* Less than. */
    else if (time1.tv_sec > time2.tv_sec)
        return (1) ;          /* Greater than. */
    else if (time1.tv_nsec < time2.tv_nsec)
        return (-1) ;            /* Less than. */
    else if (time1.tv_nsec > time2.tv_nsec)
        return (1) ;          /* Greater than. */
    else
        return (0) ;          /* Equal. */

}
/*!*****************************************************************************
Procedure:   tsFloat ()
Purpose:
    Function tsFloat() converts a TIMESPEC structure (seconds and nanoseconds)
    into the equivalent, floating-point number of seconds.

Invocation: realTime = tsFloat (time) ;

    where
        <time>    - I
            is the TIMESPEC structure that is to be converted to floating point.
        <realTime>   - O
            returns the floating-point representation in seconds of the time.

*******************************************************************************/
double  ts_Float (struct  timespec  time)
{
    return ((double) time.tv_sec + (time.tv_nsec / 1000000000.0)) ;
}
/*!*****************************************************************************
Procedure:  tsSubtract ()
Purpose:    Function tsSubtract() subtracts one TIMESPEC from another TIMESPEC.
Invocation: result = tsSubtract (time1, time2) ;

    where
        <time1>      - I
            is the minuend, a time represented by a POSIX TIMESPEC structure.
        <time2>      - I
            is the subtrahend, a time represented by a POSIX TIMESPEC structure.
        <result>  - O
            returns, in a POSIX TIMESPEC structure, TIME1 minus TIME2.  If
            TIME2 is greater than TIME1, then a time of zero is returned.

*******************************************************************************/
struct  timespec  ts_Subtract (
        struct  timespec  time1,
        struct  timespec  time2)

{    /* Local variables. */
    struct  timespec  result ;

/* Subtract the second time from the first. */

    if ((time1.tv_sec < time2.tv_sec) ||
        ((time1.tv_sec == time2.tv_sec) &&
         (time1.tv_nsec <= time2.tv_nsec))) {      /* TIME1 <= TIME2? */
        result.tv_sec = result.tv_nsec = 0 ;
    } else {                  /* TIME1 > TIME2 */
        result.tv_sec = time1.tv_sec - time2.tv_sec ;
        if (time1.tv_nsec < time2.tv_nsec) {
            result.tv_nsec = time1.tv_nsec + 1000000000L - time2.tv_nsec ;
            result.tv_sec-- ;          /* Borrow a second. */
        } else {
            result.tv_nsec = time1.tv_nsec - time2.tv_nsec ;
        }
    }

    return (result) ;

}
