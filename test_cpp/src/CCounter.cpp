/*
 * CCounter.cpp
 *
 *  Created on: Jun 2, 2017
 *      Author: hico
 */

#include <memory.h>
#include <stdio.h>

#include <CCounter.h>


CCounter::CCounter ( )
{
   setTimeout(0);
 }

CCounter::CCounter ( unsigned int a_Timeout )
{
   setTimeout(a_Timeout);
}

CCounter::~CCounter ( )
{
   // TODO Auto-generated destructor stub
}


void CCounter::Start()
{
   memset(&m_TimeStart,0, sizeof(m_TimeStart));

   sysinfo(&m_TimeStart);

}
void CCounter::Stop()
{
   sysinfo(&m_TimeStop);

}
bool CCounter::isTimeout(unsigned int a_Decrement )
{
   int vDiff = 0;

   sysinfo(&m_TimeIntermediaire);
   vDiff = (m_TimeIntermediaire.uptime-m_TimeStart.uptime);

   printf("CCounter::isTimeout() vDiff = %d \n",vDiff);

   return ( vDiff > (int)m_Timeout ) ;
}

bool CCounter::isTimeout()
{
   int vDiff = 0;

   sysinfo(&m_TimeIntermediaire);
   vDiff = (m_TimeIntermediaire.uptime-m_TimeStart.uptime);

   printf("CCounter::isTimeout() vDiff = %d \n",vDiff);

   return ( vDiff > ((int)m_Timeout) ) ;
}

int CCounter::Initialize()
{

   memset(&m_TimeStart,0, sizeof(m_TimeStart));
   memset(&m_TimeIntermediaire,0, sizeof(m_TimeIntermediaire));
   memset(&m_TimeStop,0, sizeof(m_TimeStop));

   return 0;

}
