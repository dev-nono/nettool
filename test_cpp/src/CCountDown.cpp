/*
 * CCountDown.cpp
 *
 *  Created on: Jun 7, 2017
 *      Author: hico
 */

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <time.h>

#include "CCountDown.h"

#define _dbg_printf 1

#ifdef _dbg_printf
   #define dbg_printf( a_Args... ) printf( a_Args); fflush(0);
#else
   #define dbg_printf
#endif


CCountDown::CCountDown ( ) : m_TimeStart(0), m_TimeoutMin(0)
{
//   dbg_printf("### CCountDown::CCountDown_IN)\n");

   td_SructTimespec  vSructTimespec    = {0,0};
   double            vTimeStart        = 0.0;

   clock_gettime(CLOCK_MONOTONIC_RAW,&vSructTimespec);

   vTimeStart =  (double)vSructTimespec.tv_sec + (((double)vSructTimespec.tv_nsec / 1e9));
   setTimeStart(vTimeStart);
//   dbg_printf("### CCountDown::CCountDown_OUT\n");
}
CCountDown ::CCountDown (int a_TimeoutMin )
   :m_TimeStart(0), m_TimeoutMin(a_TimeoutMin)
{
   td_SructTimespec  vSructTimespec    = {0,0};
   double            vTimeStart        = 0.0;

   clock_gettime(CLOCK_MONOTONIC_RAW,&vSructTimespec);

   vTimeStart =  (double)vSructTimespec.tv_sec + (((double)vSructTimespec.tv_nsec / 1e9));
   setTimeStart(vTimeStart);

}

CCountDown::~CCountDown ( )
{
   // TODO Auto-generated destructor stub
}


int CCountDown::Start()
{
   int vRetcode = 0;

   dbg_printf("### CCountDown::Start_IN\n");

   td_SructTimespec  vSructTimespec    = {0,0};
   double            vTimeStart        = 0.0;

   vRetcode = clock_gettime(CLOCK_MONOTONIC_RAW,&vSructTimespec);

   vTimeStart =  (double)vSructTimespec.tv_sec + (((double)vSructTimespec.tv_nsec / 1e9));
   setTimeStart(vTimeStart);

   dbg_printf("### CCountDown::Start_OUT vTimeStart=%f \n",vTimeStart);

   return vRetcode;
}
//****************************************************
//
// return curernt date invariable
//
//****************************************************
int CCountDown::getDateNow(double& a_DateNow)
{
   int               vRetcode = -1;
   td_SructTimespec  vDateNow;

dbg_printf("### CCountDown::getDateNow_IN\n");

   memset(&a_DateNow,0,sizeof(a_DateNow));

   vRetcode  = clock_gettime(CLOCK_MONOTONIC_RAW,&vDateNow);

   if( vRetcode != -1 )
   {
      a_DateNow = vDateNow.tv_sec + (((double)vDateNow.tv_nsec) / 1e9);
   }
   else
   {
      a_DateNow = -1;
   }

   dbg_printf("### CCountDown::getDateNow_OUT a_DateNow=%f\n",a_DateNow);

   return vRetcode;
}
//****************************************************
//
// return starting date invariable
//
//****************************************************
int CCountDown::getDateStart(double& a_Date) const
{
   int vRetcode = 0;

   dbg_printf("### CCountDown::getDateStart_IN\n");

   a_Date = m_TimeStart;

   dbg_printf("### CCountDown::getDateStart_OUT a_Date=%f\n",
           a_Date);

   return vRetcode;
}
//****************************************************
//
// return remaining time  in second
//
//****************************************************
int CCountDown::getTimeLeft() const
{
   int      vRetTimeRemaining = 0;

   vRetTimeRemaining = (int)llabs(getTimeLeftMicrosecond()/1e6);

   dbg_printf("### CCountDown::getTimeLeft vRetTimeRemaining=%d\n",
           vRetTimeRemaining);

   return vRetTimeRemaining;
}
//****************************************************
//
// return remaining time
//
//****************************************************
int64_t CCountDown::getTimeLeftMicrosecond() const
{
   int64_t  vRetTimeRemaining = 0;
   double   vDateNow        = 0.0;

   CCountDown::getDateNow(vDateNow);

   vRetTimeRemaining = (int64_t)(((getTimeoutDate() - vDateNow))*1e6);

   dbg_printf("### CCountDown::getTimeLeftMicrosecond vRetTimeRemaining=%lld\n",
              (int64_t)vRetTimeRemaining);

   return (int64_t)vRetTimeRemaining;
}
//****************************************************
//
// return curernt date invariable
//
//****************************************************
double  CCountDown::getTimeoutDate() const
{

   double   vDateAtTime       = 0.0;
   double   vDateStart        = 0.0;

   getDateStart(vDateStart);

   vDateAtTime = vDateStart + (getTimeoutMin()*60) ;

   dbg_printf("### CCountDown::getTimeoutDate = %f \n",vDateAtTime );

   return vDateAtTime ;
}
//****************************************************
//
// return curernt date invariable
//
//****************************************************
bool CCountDown::isTimeout()
{
   double   vDateNow        = 0.0;
   double   vDiffTime       = 0.0;

   CCountDown::getDateNow(vDateNow);

   vDiffTime = getTimeoutDate()- vDateNow;

   dbg_printf("### CCountDown::vDiffTime = %f \n",vDiffTime );

   return vDiffTime < 0;
}

