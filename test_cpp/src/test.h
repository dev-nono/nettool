/*
 * test.h
 *
 *  Created on: Jan 13, 2017
 *      Author: hico
 */

#ifndef TEST_H_
#define TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "WaitFifo.h"
#include "WaitGpio.h"

#include "test.h"

//**********************
// main.c
//**********************
void SyntaxError(int argc, char **argv);

//**********************
// WaitGpio.c
//**********************
void CheckPipe(int argc, char **argv);
void CheckReadFile(int argc, char **argv);
void TestString();

//**********************
// NetLib.c
//**********************
void  Net_1();
int   Net_2(void);

//**********************
// ipcalc.c
//**********************
int ipcalcMain(int argc, char **argv);

//**********************
// Network.c
//**********************
void  Net_inet_pton(int argc, char **argv);
int   getcolon (char *addr, int pos);
//**********************
// test.cppp
//**********************
void TestString();

void StringSubstr(int argc, char **argv);
void Sscanf(int argc, char **argv);
void CalcMask(int argc, char **argv);
void CountDown(int argc, char **argv);

//************************************************************************

void CheckStateMachine();

void Test_Argv();

//**************************************
// TIMER
// ************************************
int main_Timer(int argc, char *argv[]);

int   main_ConditionVariable();
int   main_WaitSignal();

//*********************************************
// Sleep.c
//*********************************************
int main_Sleep();

//*********************************************
// test.cpp
//*********************************************
struct  timespec Check_Modf();
long double  Check_LongDouble();

//*********************************************
// test unit UTIL_Timespec
//*********************************************
void Test_unit_Util_timespec_main();


void Test_Gpio_Select();



#define WAN2_STATE_INIT          0
#define WAN2_STATE_START         1

#define WAN2_STATE_IP            3
#define WAN2_STATE_IP_ERR        4

#define WAN2_STATE_GATEWAY       5
#define WAN2_STATE_GATEWAY_ERR   6

#define WAN2_STATE_DNS_1         7
#define WAN2_STATE_DNS_1_ERR     8

#define WAN2_STATE_DNS_2         9
#define WAN2_STATE_DNS_2_ERR     10

#define WAN2_STATE_DEAMON_CLIENT       11
#define WAN2_STATE_DEAMON_CLIENT_ERR   12

#define WAN2_STATE_DEAMON_SERVER       13
#define WAN2_STATE_DEAMON_SERVER_ERR   14

#define WAN2_STATE_IFUP                15
#define WAN2_STATE_IFUP_ERR            16

#define WAN2_STATE_LINK                17
#define WAN2_STATE_LINK_ERR            18

#ifdef __cplusplus

class CUtilNet_StateMachine
{
public:

   CUtilNet_StateMachine()  {
      ResetState();
   };

   ~CUtilNet_StateMachine() {
   }
   int getState() const {
      return m_State;
   }
   void ResetState() {
      m_State = 0;}

   void setState(int a_Position , bool a_State)
   {
      int vMask = ( 1 << a_Position );

      if( a_State != 0 )
      {
         m_State |= vMask;
      }
      else
      {
         vMask = ~ vMask;
         m_State &= vMask;
      }
   }

   bool isInit(){
      return getState() & ( 1 << WAN2_STATE_INIT);
   }
   bool isStart(){
      return getState() & ( 1 << WAN2_STATE_START);
   }
   //************************************************
   bool isIp(){
         return getState() &  (1 << WAN2_STATE_IP);}
   bool isIpErr(){
         return getState() &  (1 << WAN2_STATE_IP_ERR);}
   //************************************************
   bool isIpGateway(){
         return getState() &  (1 << WAN2_STATE_GATEWAY);}
   bool isIpGatewayErr(){
         return getState() &  (1 << WAN2_STATE_GATEWAY_ERR);}
   //************************************************
   bool isIpDns1(){
         return getState() &  (1 << WAN2_STATE_DNS_1);}
   bool isIpDns1Err(){
         return getState() &  (1 << WAN2_STATE_DNS_1_ERR);}
   //************************************************
   bool isIpDns2(){
         return getState() &  (1 << WAN2_STATE_DNS_2);}
   bool isIpDns2Err(){
         return getState() &  (1 << WAN2_STATE_DNS_2_ERR);}
   //************************************************
   bool isIpDaemonClient(){
         return getState() &  (1 << WAN2_STATE_DEAMON_CLIENT);}
   bool isIpDaemonClientErr(){
         return getState() &  (1 << WAN2_STATE_DEAMON_CLIENT_ERR);}
   //************************************************
   bool isIpDaemonServer(){
         return getState() &  (1 << WAN2_STATE_DEAMON_SERVER);}
   bool isIpDaemonServerErr(){
         return getState() &  (1 << WAN2_STATE_DEAMON_SERVER_ERR);}
   //************************************************
   bool isIpIfUP(){
         return getState() &  (1 << WAN2_STATE_IFUP);}
   bool isIpIfUPErr(){
         return getState() &  (1 << WAN2_STATE_IFUP_ERR);}
   //************************************************
   bool isIpLink(){
         return getState() &  (1 << WAN2_STATE_LINK);}
   bool isIpLinkErr(){
         return getState() &  (1 << WAN2_STATE_LINK_ERR);}

private:


   int   m_State;

}; // class CWan2StateMachine
#endif

#ifdef __cplusplus
}
#endif


#endif /* TEST_H_ */
