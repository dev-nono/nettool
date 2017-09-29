/*
 * tu_UTIL_Timeout.cpp
 *
 *  Created on: Sep 22, 2017
 *      Author: hico
 */

#include <signal.h>
#include <pthread.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "UTIL_Timeout.h"
#include "UTIL_Timespec.h"

#include "../Common.h"

#include "test.h"

#define SIG_CUST_1 ( SIGRTMIN + 1 )
#define SIG_CUST_2 ( SIGRTMIN + 2 )

//********************************************************
//* data static
//********************************************************
struct sTreadData
{
   pthread_t            TreadID;
   pthread_mutex_t      mutex;
   sigset_t             SigsetMask;
   struct timespec      CountDown;
   struct timespec      DateStart;
   struct timespec      DateStop;
   int                  ID;
   siginfo_t            Siginfo;
   struct sigaction     Sigaction;
   int                  Signal;
   CTimeout             *p_CTimeout;

   union sigval          Sigval;
};
struct sTreadData g_StructData_1;
struct sTreadData g_StructData_2;
struct sTreadData g_StructData_3;
struct sTreadData g_StructData_main;

static void  staticHandlerDefault (int, siginfo_t *, void *)
{

}

//****************************************************************************
//*
//****************************************************************************
static int Wait_1(struct sTreadData * a_pStruct)
{
   int               vRetcode = 0;
   int               vErrorNo = 0;
   struct timespec   vDateStart;
   struct timespec   vDateEnd;
   struct timespec   vDateDiff;
   struct timespec   vTimeout;

   vTimeout = a_pStruct->CountDown;
   CTimeout::getDateNow(vDateStart);

   //Printf("############## Wait_1_1 : BAD signal received =%d \n",vRetcode);
   do
   {
      errno = 0;
      vRetcode  = sigtimedwait(&a_pStruct->SigsetMask,0,&vTimeout);
      //         vRetcode  = sigtimedwait(&m_SigsetMask,NULL,&vTimeout);
      vErrorNo = errno;
      CTimeout::getDateNow(vDateEnd);

      //*******************************************
      // receive signal in the set mask, signal blocked
      //*******************************************
      if( vRetcode  > 0 )
      {
         //*******************************************
         // continue wait delta time
         //*******************************************
         if( vRetcode != a_pStruct->Signal )
         {
            // calcul du temps passé
            vDateDiff = CTimespec::Substract_ts(vDateEnd,vDateStart);
            // calcul du temps restant
            vTimeout = CTimespec::Substract_ts(a_pStruct->CountDown,vDateDiff);

            Printf("############## Wait_1_2(%d)  Error  countdown=%ld.%ld : reste=%ld.%ld",
                   a_pStruct->ID,
                   a_pStruct->CountDown.tv_sec,      a_pStruct->CountDown.tv_nsec,
                   vTimeout.tv_sec,       vTimeout.tv_nsec );
         }
         else
         {
            Printf("############## Wait_1_3(%d) signal SIGALARM  received =%d",
                   a_pStruct->ID,
                   vRetcode);
            vRetcode = 0;
            break;
         }
      }
      //*******************************************
      // error exit before timeout
      // or other signal not blocked in the current thread
      //*******************************************
      else
      {
         //*******************************************
         // stoped by another signal continue to wait delta time
         //*******************************************
         if ( vErrorNo  == EINTR )
         {
            // calcul du temps passé
            vDateDiff = CTimespec::Substract_ts(vDateEnd,vDateStart);

            // calcul du temps restant
            vTimeout = CTimespec::Substract_ts(a_pStruct->CountDown,vDateDiff);

            Printf("############## Wait_1_4(%d) countdown%ld.%ld : Error signal reste=%ld.%ld",
                   a_pStruct->ID,
                   a_pStruct->CountDown.tv_sec,      a_pStruct->CountDown.tv_nsec,
                   vTimeout.tv_sec,       vTimeout.tv_nsec );
         }
         //*******************************************
         // end wait
         //*******************************************
         else if ( vErrorNo  == EAGAIN )
         {
            Printf("############## Wait_1_5(%d) : EAGAIN end timer OK ",
                   a_pStruct->ID);
            vRetcode = 0;
            break;
         }
         //*******************************************
         // error exit -1
         //*******************************************
         else
         {
            vRetcode = -1;
            break;
         }
      }
   }while( vRetcode != a_pStruct->Signal );

}
//****************************************************************************
//*
//****************************************************************************
static int Wait_2(struct sTreadData * a_pStruct)
{
   int                  vRetcode = 0;
   char                 vBuffer[1024];
   int                  vError    = 0;

   sprintf(vBuffer,"\t\t Wait_2_1(%d)_IN",a_pStruct->ID);

   memset(&a_pStruct->Siginfo,0,sizeof(a_pStruct->Siginfo));
//
   a_pStruct->Siginfo._sifields._timer.si_sigval.sival_int = a_pStruct->ID;


   CTimeout::getDateNow(a_pStruct->DateStart);

   errno       = 0;
   vRetcode    = sigtimedwait(&a_pStruct->SigsetMask,
                              &a_pStruct->Siginfo,
                              &a_pStruct->CountDown);
   vError      = errno;

   CTimeout::getDateNow(a_pStruct->DateStop);

  if( vRetcode > 0 )
  {

     if( (vRetcode == SIGALRM )
        //( sigismember(&a_pStruct->SigsetMask,SIGALRM) > 0 )
        //||  ( sigismember(&a_pStruct->SigsetMask,SIG_CUST_1) > 0 )
        //  ( sigismember(&a_pStruct->SigsetMask,SIG_CUST_2) > 0 )
     )
     {
        sprintf(vBuffer,"\t\t Wait_2_2(%d)_1 : reception  vRetcode=%d",
                a_pStruct->ID,
                vRetcode );
     }
     else

     {
        sprintf(vBuffer,"\t\t Wait_2_3(%d) :reception signal Non blocked  continue vRetcode=%d",
                a_pStruct->ID,
                vRetcode);
     }
  }
  else
  {
     if( vError == EAGAIN )
     {
        sprintf(vBuffer,"\t\t Wait_2_4(%d):END_WAIT EAGAIN  vRetcode=%d",
                a_pStruct->ID,
                vRetcode);

     }
     else if( vError == EINTR )
     {
        sprintf(vBuffer,"\t\t Wait_2_5(%d) :EINTR interupt non blocked  vRetcode=%d ",
                a_pStruct->ID,
                vRetcode);
     }
     else
     {
        sprintf(vBuffer,"\t\t Wait_2_6(%d) : vError=%d vRetcode=%d ",
                a_pStruct->ID,
                vError,
                vRetcode);
     }
  }



//   sprintf(vBuffer,"\t\t WaitSignal_Wait_2_7 : vretcode=%d %s ID=%d SIG=%d ",
//           vRetcode,
//           vRetcode > 0          ? "***********************   OK" :
//           vError  == EAGAIN   ? "EAGAIN" :
//           vError  == EINTR    ? "EINTR" :
//           vError  == EINVAL   ? "EINVAL" : "ERROR !!!",
//           a_pStruct->ID,
//           vSiginfo.si_value.sival_int
//   );set
   PrintfLn(vBuffer);

   return 0;
}
//****************************************************************************
//*
//****************************************************************************
static void* funcThread_CTimeout(void* a_pArg)
{
   struct sTreadData*   pData       = (struct sTreadData*)a_pArg;
   int                  vRetcode    = 0;
   struct sigaction     vAction;
/*
   vRetcode = sigemptyset(&vAction.sa_mask);
   vRetcode = sigaddset(&vAction.sa_mask, SIG_CUST_1);
   vRetcode = sigaddset(&vAction.sa_mask, SIG_CUST_2);

   vRetcode = pthread_sigmask(SIG_BLOCK,&vAction.sa_mask,NULL);

   //**************************************
   vAction.sa_flags = SA_SIGINFO;

   // add handler to action
   vAction.sa_sigaction = staticHandlerDefault;

   sigaction(SIG_CUST_1, &vAction, NULL);
   sigaction(SIG_CUST_2, &vAction, NULL);
*/

   do
   {
//      Printf("funcThread(%d) :: Wait CountDown.tv_sec=%d",g
//             pData->ID,
//             (int)pData->CountDown.tv_sec);

     // pData->p_CTimeout->BlockSignal();

      vRetcode = pData->p_CTimeout->Wait (pData->CountDown);

     // pData->p_CTimeout->UnBlockSignal();

   }while(1);



}

//****************************************************************************
//*
//****************************************************************************
static void* funcThread(void* a_pArg)
{
   int vRetcode  = 0;
   struct sTreadData* pData = (struct sTreadData*)a_pArg;
   //*********************************************
   vRetcode = sigemptyset (&pData->SigsetMask);

   //vRetcode = sigaddset(&pData->SigsetMask, SIGALRM);
   vRetcode = sigaddset(&pData->SigsetMask, SIG_CUST_1);
   vRetcode = sigaddset(&pData->SigsetMask, SIG_CUST_2);

//   memset(&vSigsetOld,0,sizeof(vSigsetOld));
//   // set signal
//   vRetcode = pthread_sigmask(SIG_SETMASK,&pData->SigsetMask,&vSigsetOld);

//   for(int ii=1; ii < 64; ii++ )
//   {
//      vRetcode = sigismember(&vSigsetOld,ii);
//      if ( vRetcode > 0 ) printf(" ==> signal %d present \n",ii);
//   }

   /* Block timer signal temporarily */
   vRetcode = pthread_sigmask(SIG_BLOCK,&pData->SigsetMask,NULL);

   //*********************************************
   do
   {
       vRetcode = Wait_1(pData); // local use

   }while(1);


   return (void*)vRetcode;

}
//****************************************************************************
//*
//****************************************************************************
static int   CreateThread_1()
{
   int vRetcode  = 0;

   vRetcode = pthread_create(&g_StructData_1.TreadID,NULL,funcThread,&g_StructData_1);
   vRetcode = pthread_create(&g_StructData_2.TreadID,NULL,funcThread,&g_StructData_2);
   vRetcode = pthread_create(&g_StructData_3.TreadID,NULL,funcThread,&g_StructData_3);

   return vRetcode;
}
//****************************************************************************
//*
//****************************************************************************
static int   CreateThread_2()
{
   int vRetcode  = 0;

   vRetcode = pthread_create(&g_StructData_1.TreadID,NULL,funcThread_CTimeout,&g_StructData_1);
   vRetcode = pthread_create(&g_StructData_2.TreadID,NULL,funcThread_CTimeout,&g_StructData_2);
   vRetcode = pthread_create(&g_StructData_3.TreadID,NULL,funcThread_CTimeout,&g_StructData_3);

   return vRetcode;
}
//****************************************************************************
//*
//****************************************************************************
int main_TU_Timeout(int argc, char **argv)
{
   int      vRetcode = 0;
   char     vInputBuffer[10];
   //char     vBuffer[1024];
   int      vSignal  = SIGALRM;

   sigval_t vSigval;

   struct sTreadData * pCurrentStruct = 0;


   //********************************
   // mode
   // 01       wait intern
   // 10       CTimeout send()
   // 11       CTimeout send(signal)
   int vMode = 0;

   if( argc > 2 )
   {
      vMode = atoi(argv[3-1]);
   }


   CTimeout             vCTimeout_1;
   CTimeout             vCTimeout_2;
   CTimeout             vCTimeout_3;

   memset( &g_StructData_1,0,sizeof(g_StructData_1));
   memset( &g_StructData_2,0,sizeof(g_StructData_2));
   memset( &g_StructData_3,0,sizeof(g_StructData_3));
   memset( &g_StructData_main,0,sizeof(g_StructData_main));

   g_StructData_1.p_CTimeout = & vCTimeout_1;
   g_StructData_2.p_CTimeout = & vCTimeout_2;
   g_StructData_3.p_CTimeout = & vCTimeout_3;

   g_StructData_1.CountDown.tv_sec = 5;
   g_StructData_2.CountDown.tv_sec = 10;
   g_StructData_3.CountDown.tv_sec = 15;

   g_StructData_1.ID = 1;
   g_StructData_2.ID = 2;
   g_StructData_3.ID = 3;



//   vRetcode = sigaddset(&g_StructData_main.SigsetMask, SIG_CUST_2);
//   vRetcode = pthread_sigmask(SIG_BLOCK,&g_StructData_main.SigsetMask,NULL);

   if( vMode == 0 )
   {
      g_StructData_1.Signal = SIGALRM;
      g_StructData_2.Signal = SIGALRM;
      g_StructData_3.Signal = SIGALRM;

      CreateThread_1();
   }
   else
   {
      CreateThread_2(); // class CTimeout::wait()
   }


   do{

     // PrintfLn((char*)"main_WaitSignal_1 : Apuyer sur une touche pour continuer");
      // vChar = getchar();
      memset(vInputBuffer,0,sizeof(vInputBuffer));

      fgets(vInputBuffer,4,stdin);

      if(      vInputBuffer[1] == '1' ){  vSignal  = SIG_CUST_1;}
      else if( vInputBuffer[1] == '2' ){  vSignal  = SIG_CUST_2;}
      else                             {  vSignal  = SIGALRM;}

      //******************************************************************
      //******************************************************************
      if( vInputBuffer[0] == '1' )
      {
         pCurrentStruct = &g_StructData_1;
         //sprintf(vBuffer,"WaitSignal_sigtimedwait ==> char 1"); PrintfLn(vBuffer);
      }
      //******************************************************************
      //******************************************************************
      else if( vInputBuffer[0] == '2' )
      {
         pCurrentStruct = &g_StructData_2;
      }
      //******************************************************************
      //******************************************************************
      else if( vInputBuffer[0] == '3' )
      {
         pCurrentStruct = &g_StructData_3;
      }
      //******************************************************************
      //******************************************************************
      else
      {
         pCurrentStruct = &g_StructData_1;
      }
//*****************************************************
      if( vMode == 0 )
       {
         pCurrentStruct-> Sigval.sival_int = vSignal;
         pCurrentStruct-> Sigval.sival_ptr = 0;

         vRetcode  = pthread_sigqueue( pCurrentStruct->TreadID,
                                        vSignal,
                                        pCurrentStruct->Sigval);

         if ( vRetcode != 0 )
            Printf((char*)"Send(%#.2X)_0 vRetcode=%d errno=%d %s",
                   vSignal,vRetcode,errno, strerror(errno));
       }
       else
       {
          if( vMode == 10)
          {
             vRetcode = pCurrentStruct->p_CTimeout->SendSignal();

             if ( vRetcode != 0 )
                Printf((char*)"Send(%#.2X)_10 vRetcode=%d errno=%d %s",
                   vSignal,vRetcode,errno, strerror(errno));
          }
          else // ( vMode == 11 )
          {
             pCurrentStruct-> Sigval.sival_int = vSignal;
             pCurrentStruct-> Sigval.sival_ptr = 0;

             vRetcode  = pthread_sigqueue( pCurrentStruct->p_CTimeout->getThreadId(),
                                           vSignal,
                                           pCurrentStruct->Sigval);

             if ( vRetcode != 0 )
                Printf((char*)"Send(%#.2X)_11 vRetcode=%d errno=%d %s",
                   vSignal,vRetcode,errno, strerror(errno));
          }

       }

      //PrintfLn(vBuffer);

   }while(1);

   return vRetcode ;
}
