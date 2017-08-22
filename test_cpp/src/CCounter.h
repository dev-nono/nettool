/*
 * CCounter.h
 *
 *  Created on: Jun 2, 2017
 *      Author: hico
 */

#ifndef CCOUNTER_H_
#define CCOUNTER_H_

#include <sys/sysinfo.h>

class CCounter
{
   public:
      CCounter ( );
      CCounter ( unsigned int a_Timeout );
      virtual      ~CCounter ( );

      void Start();
      void Stop();

      bool isTimeout(unsigned int a_Decrement);
      bool isTimeout();

      unsigned int
      getTimeout ( ) const
      {
         return m_Timeout;
      }

      void
      setTimeout ( unsigned int a_Timeout )
      {
         m_Timeout = a_Timeout;
      }

   private:
      int Initialize();

      struct sysinfo m_TimeStart;
      struct sysinfo m_TimeIntermediaire;
      struct sysinfo m_TimeStop;

      unsigned int m_Timeout;

      unsigned int m_CurrentTimeout;
};

#endif /* CCOUNTER_H_ */
