/*
 * CCountDown.h
 *
 *  Created on: Jun 7, 2017
 *      Author: hico
 */

#ifndef LDT_NETWORK_PLC_G3_PLAN_PLCFIRMWARE_CCOUNTDOWN_H_
#define LDT_NETWORK_PLC_G3_PLAN_PLCFIRMWARE_CCOUNTDOWN_H_

#include <stdint.h>
#include <time.h>
#include <stdio.h>
/*
#include <boost/serialization/access.hpp>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/version.hpp>
*/

typedef struct timespec td_SructTimespec;

class CCountDown
{
public:
      CCountDown ( );
      CCountDown (int a_TimeoutMin );
      virtual
      ~CCountDown ( );

      int         Start();
      static int  getDateNow(double& a_DateNow);
      int         getDateStart(double& a_Date)  const;
      bool        isTimeout();

      int         getTimeLeft()                 const ;// return time in second
      int64_t     getTimeLeftMicrosecond()      const ; // return time in microsecond
      double      getTimeoutDate()              const;

      int getTimeoutMin ( ) const
      {
         printf("### CCountDown::getTimeoutMin = %d\n",m_TimeoutMin);
         return m_TimeoutMin;
      }
      
      void
      setTimeoutMin ( int timeout )
      {
         m_TimeoutMin = timeout;
      }
      
      double
      getTimeStart ( ) const
      {
         return m_TimeStart;
      }
      
      uint32_t
      getTimeRemaining ( ) const
      {
         return m_TimeRemaining;
      }
      
      void
      setTimeRemaining ( uint32_t a_TimeRemaining )
      {
         m_TimeRemaining = a_TimeRemaining;
      }

      //******************************************
      //*
      //******************************************
      template<class Archive>
      void serialize(Archive & ar, const unsigned int)
      {
            //ar & m_TimeStart;
            ar & m_TimeoutMin;
            ar & m_TimeRemaining;
      }

   private:

      void setTimeStart ( double timeStart )      {
         m_TimeStart = timeStart;      }

      double   m_TimeStart;
      int      m_TimeoutMin;
      uint32_t m_TimeRemaining;


};

#endif /* LDT_NETWORK_PLC_G3_PLAN_PLCFIRMWARE_CCOUNTDOWN_H_ */
