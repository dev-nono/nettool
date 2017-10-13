/*
 * Test_condition_variable.c
 *
 *  Created on: Jun 22, 2017
 *      Author: hico
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

#include "ts_util.h"
#include "Common.h"

//#define  SIG_STOP (SIGRTMIN+1)

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)



//********************************************************
// declaration
//********************************************************
//void  main_WaitSignal();
//static void* WaitSignal_funcThread(void* a_pArg);
//static int   WaitSignal_CreateThread();
//static void Printf(char* a_Message);

//********************************************************
//* data static
//********************************************************
struct sTreadData
{
   pthread_t            TreadID;
   pthread_mutex_t      mutex;
   sigset_t             SigsetMask;
   struct timespec      Timeout;
   struct timespec      DateStart;
   int                  ID;
   siginfo_t            Siginfo;
   struct sigaction     Sigaction;
};
struct sTreadData g_StructData_1;
struct sTreadData g_StructData_2;
struct sTreadData g_StructData_Timer;


void staticHandler_1 (int a_NumSig )
{
   printf("staticHandler_1 a_NumSig=%d\n",a_NumSig);
}
void staticHandler_3 (int a_NumSig , siginfo_t * a_pSigInfo, void *a_pUcontext)
{
   printf("staticHandler_3 a_NumSig=%d\n",a_NumSig);
}

void WaitSignal_Wait(struct sTreadData * a_pStruct)
{

   int                  vRetcode = 0;
   char                 vBuffer[1024];
   int                  vError    = 0;

   sprintf(vBuffer,"\t\t WaitSignal_Wait_1");

   memset(&a_pStruct->Siginfo,0,sizeof(a_pStruct->Siginfo));

   a_pStruct->Siginfo._sifields._timer.si_sigval.sival_int = a_pStruct->ID;

   errno       = 0;
   vRetcode    = sigtimedwait(&a_pStruct->SigsetMask,
                              &a_pStruct->Siginfo,
                              &a_pStruct->Timeout);
   vError      = errno;

   clock_gettime(CLOCK_MONOTONIC_RAW,&a_pStruct->DateStart);

  if( vRetcode > 0 )
  {

     if( (vRetcode == SIGALRM )
        //( sigismember(&a_pStruct->SigsetMask,SIGALRM) > 0 )
        //||  ( sigismember(&a_pStruct->SigsetMask,SIGUSR1) > 0 )
        //  ( sigismember(&a_pStruct->SigsetMask,SIGUSR2) > 0 )
     )
     {
        sprintf(vBuffer,"\t\t WaitSignal_Wait_2_1 : reception  SIGNAL=%d ID=%d ",
                vRetcode,
                a_pStruct->ID);
     }
     else
     {
        sprintf(vBuffer,"\t\t WaitSignal_Wait_2_2 :reception signal Non blocked  continue SIGNAL=%d ID=%d ",
                vRetcode,
                a_pStruct->ID);
     }
  }
  else
  {
     if( vError == EAGAIN )
     {
        sprintf(vBuffer,"\t\t WaitSignal_Wait_3 :END_WAIT EAGAIN  ID=%d SIG=%d ",
                a_pStruct->ID,
                a_pStruct->Siginfo.si_value.sival_int);

     }
     else if( vError == EINTR )
     {
        sprintf(vBuffer,"\t\t WaitSignal_Wait_4 :EINTR interupt non blocked  ID=%d SIG=%d ",
                a_pStruct->ID,
                a_pStruct->Siginfo.si_value.sival_int);

     }
     else
     {
        sprintf(vBuffer,"\t\t WaitSignal_Wait_5 : vError=%d  ID=%d SIG=%d ",
                vError,
                a_pStruct->ID,
                a_pStruct->Siginfo.si_value.sival_int);
     }

  }

//   sprintf(vBuffer,"\t\t WaitSignal_Wait_2 : vretcode=%d %s ID=%d SIG=%d ",
//           vRetcode,
//           vRetcode > 0          ? "***********************   OK" :
//           vError  == EAGAIN   ? "EAGAIN" :
//           vError  == EINTR    ? "EINTR" :
//           vError  == EINVAL   ? "EINVAL" : "ERROR !!!",
//           a_pStruct->ID,
//           vSiginfo.si_value.sival_int
//   );
   PrintfLn(vBuffer);


}
//********************************************************
//*
//********************************************************
void static_sigtimedwait_Handler
//(int a_NumSig , siginfo_t * a_pSigInfo, void *a_pUcontext)
(int a_NumSig )
{
   printf("static_sigtimedwait_Handler a_NumSig=%d \n", a_NumSig );

}
//********************************************************
//*
//********************************************************
static void    WaitSignal_Config(struct sTreadData * a_pStruct)
{
   char               vBuffer[1024];

   sigemptyset (&a_pStruct->SigsetMask);
   sigaddset(&a_pStruct->SigsetMask, SIGALRM);
//   sigaddset(&a_pStruct->SigsetMask, SIGUSR1);
//   sigaddset(&a_pStruct->SigsetMask, SIGUSR2);

   // set signal
   pthread_sigmask(SIG_SETMASK,&a_pStruct->SigsetMask,NULL);

   /* Block timer signal temporarily */
   pthread_sigmask(SIG_BLOCK,&a_pStruct->SigsetMask,NULL);

   /*
   struct sigaction    vSigaction;
   // vSigaction.sa_sigaction  = staticHandler_1; // ok

   vSigaction.sa_sigaction    = staticHandler_3; // ok
   vSigaction.sa_flags        = SA_SIGINFO;

   sigaction(SIGUSR2,&vSigaction,NULL);
*/
   sprintf(vBuffer,"WaitSignal_Config : Blocking signal %d", SIGALRM);
   PrintfLn(vBuffer);

}
//********************************************************
//*
//********************************************************
void* WaitSignal_Timer_funcThread(void* a_pArg)
{
   return 0;
}

//********************************************************
//*
//********************************************************
void* WaitSignal_funcThread(void* a_pArg)
{
  //int                vRetcode __attribute__ ((unused)) = 0 ;
//  char               vBuffer[1024];
//  uint64_t           ii = 0;
//  double             vdDiff       = 0;
//  struct timespec    vDateAfter  = {0,0};
//  struct timespec    vDiffAfter  = {0,0};
//  struct timespec    vDateStart  = {2,0};
  struct timespec    vDateNow;
  struct sigaction;
  int NbLoop = 3;

  WaitSignal_Config(&g_StructData_1);

  PrintfLn("\t funcThread_1 g_StructData_1");
/*
  // *************************************************
  // signal 1  sigemptyset (&g_SigsetMask);
  // *************************************************
  sigaddset (&g_StructData_2.SigsetMask, SIGALRM);

  // set signal
  pthread_sigmask(SIG_SETMASK,&g_StructData_2.SigsetMask,NULL);

//  //Block timer signal temporarily
//  pthread_sigmask(SIG_BLOCK,&g_StructData_2.SigsetMask,NULL);
//
//  sigaction(SIGALRM,&g_StructData_2.Sigaction,NULL);

  PrintfLn("\t funcThread_2 g_StructData_2");
*/

  //************************************************

   PrintfLn("\t funcThread_3 ");
   do
   {
      PrintfLn("\t funcThread_4");

      g_StructData_1.Timeout.tv_sec    = 10;
      g_StructData_1.Timeout.tv_nsec   = 0;

      g_StructData_2.Timeout.tv_sec    = 4;
      g_StructData_2.Timeout.tv_nsec   = 0;

      clock_gettime(CLOCK_MONOTONIC_RAW,&vDateNow);

      WaitSignal_Wait(&g_StructData_1);

      PrintfLn("\t funcThread_5");

      //WaitSignal_Wait(&g_StructData_2);

      // traitement
//      for(ii = 0; ii < vDateNow.tv_sec*10000; ii ++)
//      {
//         vdDiff = (vdDiff *1e6/1e9) + 0xFF;
//      }
//
//      clock_gettime(CLOCK_MONOTONIC_RAW,&vDateAfter);
//
//     vDiffAfter   = ts_Subtract(vDateAfter,g_DateStart);
//     g_Timeout    = ts_Subtract(vDateStart,vDiffAfter);
//
//     sprintf(vBuffer,"funcThread_4 diff_sec=%ld diff_ns=%ld",
//             g_Timeout.tv_sec,
//             g_Timeout .tv_nsec
//             );
//
//     PrintfLn(vBuffer);

      PrintfLn("\t funcThread_6");

     // NbLoop--;
   }
   //while(1);NbLoop
   while(NbLoop);

   PrintfLn("\t funcThread_5");

//   sprintf(vBuffer,"\t funcThread_7 Blocking signal %d", SIG_STOP);
//   PrintfLn(vBuffer);

////   if (sigprocmask(SIG_UNBLOCK, &g_SigsetMask, NULL) == -1)
////      errExit("sigprocmask");


   return NULL;
}
//********************************************************
//*
//********************************************************
int   WaitSignal_CreateThread()
{

   int vRetcode  = 0;
   PrintfLn("CreateThread_1");

   vRetcode = pthread_create(&g_StructData_1.TreadID,NULL,WaitSignal_funcThread,NULL);   g_StructData_1.Siginfo._sifields._timer.si_sigval.sival_int = g_StructData_1.ID;
   g_StructData_2.Siginfo._sifields._timer.si_sigval.sival_int = g_StructData_2.ID;

   PrintfLn("CreateThread_2");

   return vRetcode;
}

//********************************************************
//*
//********************************************************
void WaitSignal_sigtimedwait()
{
   char  vBuffer[1024];
   char  vInputBuffer[1024];
   int   vRetcode = 0;
   pthread_mutex_init(&g_StructData_1.mutex, NULL);
   //char  vChar[10] = 0;

//   union sigval vValue = {0};

   (void)vRetcode;

   memset(&g_StructData_1,0,sizeof(g_StructData_1));
   memset(&g_StructData_2,0,sizeof(g_StructData_2));


   g_StructData_1.ID = 77;
   g_StructData_2.ID = 88;
   g_StructData_1.Siginfo._sifields._timer.si_sigval.sival_int = g_StructData_1.ID;
   g_StructData_2.Siginfo._sifields._timer.si_sigval.sival_int = g_StructData_2.ID;

//   //***************************************************************
//   sigset_t             SigsetMask;
//
//   sigemptyset (&SigsetMask);
//   sigaddset(&SigsetMask, SIGUSR2);
//
//   // set signal
//   pthread_sigmask(SIG_SETMASK,&SigsetMask,NULL);
//
//   /* Block timer signal temporarily */
//   pthread_sigmask(SIG_BLOCK,&SigsetMask,NULL);
//
////   struct sigaction    vSigaction;
////   vSigaction.sa_sigaction  = staticHandler_1;
////   sigaction(SIGUSR2,&vSigaction,NULL);


   //   WaitSignal_Config();
   // create tread
   WaitSignal_CreateThread();

   //WaitSignal_Config(&g_StructData_1);


   do{

      PrintfLn("main_WaitSignal_1 : Apuyer sur une touche pour continuer");
      // vChar = getchar();
      fgets(vInputBuffer,3,stdin);

      //int raise(int sig);
//      vRetcode = raise(SIG_STOP);
//      sprintf(vBuffer,"main_WaitSignal : raise = %d %s",
//              vRetcode,
//              vRetcode == 0      ?  "NO_ERROR":
//              vRetcode == EAGAIN ? "EAGAIN" :
//              vRetcode == EINVAL ? "EINVAL" :
//              vRetcode == ENOSYS ? "ENOSYS" :
//              vRetcode == ESRCH ? "ESRCH" : "ERROR !!!");

      if( vInputBuffer[0] == '1' )
      {
         sprintf(vBuffer,"WaitSignal_sigtimedwait ==> char 1");
         PrintfLn(vBuffer);
         vRetcode  = pthread_sigqueue( g_StructData_1.TreadID,
                                       SIGALRM,
                                       g_StructData_1.Siginfo._sifields._timer.si_sigval);
      }
      else if( vInputBuffer[0] == '2' )
      {
         sprintf(vBuffer,"WaitSignal_sigtimedwait ==> char 2");
         PrintfLn(vBuffer);
         vRetcode  = pthread_sigqueue( g_StructData_1.TreadID,
                                       SIGUSR1,
                                       g_StructData_2.Siginfo._sifields._timer.si_sigval);

      }
      else
      {
         sprintf(vBuffer,"WaitSignal_sigtimedwait ==> char 3");
         PrintfLn(vBuffer);
         vRetcode  = pthread_sigqueue( g_StructData_1.TreadID,
                                       SIGUSR2,
                                       g_StructData_2.Siginfo._sifields._timer.si_sigval);

      }

//      sprintf(vBuffer,"main_WaitSignal_2 : pthread_sigqueue = %d %s",
//              vRetcode,
//              vRetcode == 0      ?  "NO_ERROR":
//              vRetcode == EAGAIN ? "EAGAIN" :
//              vRetcode == EINVAL ? "EINVAL" :
//              vRetcode == ENOSYS ? "ENOSYS" :
//              vRetcode == ESRCH ? "ESRCH" : "ERROR !!!");




      PrintfLn(vBuffer);

      //int sigqueue(pid_t pid, int sig, const union sigval valeur);
      //int sigqueue(pid_t pid, int sig, const union sigval valeur);



   }while(1);

}
/*static void Timer_handler(int sig, siginfo_t *si, void *uc)
{
   printf("Timer_handler : Caught signal %d\n", sig);

   signal(sig, SIG_IGN);
}*/
//********************************************************
//*
//********************************************************
void WaitSignal_Timer()
{
//   int vRetcode  = 0;
//
//   memset(&g_StructData_Timer,0,sizeof(g_StructData_Timer));
//
//   PrintfLn("WaitSignal_Timer_1");
//
//   vRetcode = pthread_create(&g_StructData_Timer.TreadID,NULL,WaitSignal_Timer_funcThread,NULL);   g_StructData_1.Siginfo._sifields._timer.si_sigval.sival_int = g_StructData_1.ID;
//
//   PrintfLn("WaitSignal_Timer_2");

}

//********************************************************
//*
//********************************************************
void* WaitSignal_funcThread_Sleep(void* a_pArg)
{
   struct sTreadData*   pThis = (struct sTreadData*)a_pArg;
   int                  vRetcode    = 0;
   int                  vErrorCode  = 0;
   //struct sigaction vAction;
   struct timespec      tsTimeoutRest;

   //*************************************************
   // signal 1  sigemptyset (&g_SigsetMask);
   //*************************************************
   sigaddset (&pThis->SigsetMask, SIGALRM);

   //if(pThis->ID == 1  )
      sigaddset (&pThis->SigsetMask, SIGUSR1);
   //else
      sigaddset (&pThis->SigsetMask, SIGUSR2);

   // set signal
   pthread_sigmask(SIG_SETMASK,&pThis->SigsetMask,NULL);
   /* Block timer signal temporarily */
   pthread_sigmask(SIG_BLOCK,&pThis->SigsetMask,NULL);

//   // reset struct
//   sigemptyset(&vAction.sa_mask);
//
//   // add handler to action
//   vAction.sa_sigaction    = staticHandler;
//   vAction.sa_mask         = pThis->SigsetMask;
//   vAction.sa_flags        = SIG_DFL;
//
//   sigaction(SIGALRM, &vAction, NULL);
//   sigaction(SIGUSR1, &vAction, NULL);
//   sigaction(SIGUSR2, &vAction, NULL);
//

    do
    {
       memset(&tsTimeoutRest,0,sizeof(tsTimeoutRest));

       errno = 0;
//       vRetcode = clock_nanosleep( CLOCK_MONOTONIC,
//                                   TIMER_ABSTIME,
//                                   &tsTimeout,
//                                   &tsTimeoutRest);
       vRetcode   = sleep(pThis->Timeout.tv_sec);
       vErrorCode = errno;

       if( vRetcode == 0 ) // end timeout
       {
          printf("Sleep_%d END \n",pThis->ID);
       }
       else
       {
          if( vErrorCode == EINTR )
          {

             printf("Sleep_%d EINTR  interuption \n",pThis->ID);

          }
          else
          {
             printf("Sleep_%d Error \n",pThis->ID);

          }

       }

    }while(1);

    return 0;
}
//********************************************************
//*
//********************************************************
int WaitSignal_Sleep()
{
   int            vRetcode = 0;
   int            vRetErrorcode = 0;
   char           vChar    = 0;
   union sigval   vSgval_1;
   union sigval   vSgval_2;
   //create thread

   memset(&g_StructData_1,0,sizeof(g_StructData_1));
   memset(&g_StructData_2,0,sizeof(g_StructData_2));


   g_StructData_1.ID = 1;
   g_StructData_2.ID = 2;

   g_StructData_1.Timeout.tv_sec = 4;
   g_StructData_2.Timeout.tv_sec = 10;

   vRetcode = pthread_create( &g_StructData_1.TreadID,NULL,
                              WaitSignal_funcThread_Sleep,
                              &g_StructData_1);

   vRetcode = pthread_create( &g_StructData_2.TreadID,NULL,
                              WaitSignal_funcThread_Sleep,
                              &g_StructData_2);

//   //*************************************************
//   // signal 1  sigemptyset (&g_SigsetMask);
//   //*************************************************
//   sigaddset (&g_StructData_1.SigsetMask, SIGALRM);
//   // set signalvRetcode
//   pthread_sigmask(SIG_SETMASK,&g_StructData_1.SigsetMask,NULL);
//   /* Block timer signal temporarily */
//   pthread_sigmask(SIG_BLOCK,&g_StructData_1.SigsetMask,NULL);
//
//
//   //*************************************************
//   // signal 1  sigemptyset (&g_SigsetMask);
//   //*************************************************
//   sigaddset (&g_StructData_2.SigsetMask, SIGALRM);
//   // set signal
//   pthread_sigmask(SIG_SETMASK,&g_StructData_2.SigsetMask,NULL);
//   /* Block timer signal temporarily */
//   pthread_sigmask(SIG_BLOCK,&g_StructData_2.SigsetMask,NULL);


   while(1)
   {
      vChar = getchar();

      vSgval_1.sival_int = 0;
      vSgval_2.sival_int = 0;

      if( vChar == '1') //  T1 SIGALARM
      {
         errno = 0;
         vRetcode  = pthread_sigqueue( g_StructData_1.TreadID,
                                       SIGALRM,
                                       vSgval_1);
         vRetErrorcode = errno;
         //if (vRetcode != 0 )
            printf("WaitSignal_Sleep : char = %c vRetcode=%d errno=%d \n",vChar,vRetcode,vRetErrorcode);
      }
      else if( vChar == '2') //  T1 SIGUSR1
      {
         errno = 0;
         vRetcode  = pthread_sigqueue( g_StructData_1.TreadID,
                                       SIGUSR1,
                                       vSgval_1);
         vRetErrorcode = errno;
         if (vRetcode != 0 )printf("WaitSignal_Sleep : char = %c vRetcode=%d errno=%d \n",vChar,vRetcode,vRetErrorcode);

      }
      else if( vChar == '5') //  T2 SIGALARM
      {
         errno = 0;
         vRetcode  = pthread_sigqueue( g_StructData_2.TreadID,
                                       SIGALRM,
                                       vSgval_2);
         vRetErrorcode = errno;
         if (vRetcode != 0 )printf("WaitSignal_Sleep : char = %c vRetcode=%d errno=%d \n",vChar,vRetcode,vRetErrorcode);

      }
      else if( vChar == '6') //  T2 SIGUSR1
      {
         errno = 0;
         vRetcode  = pthread_sigqueue( g_StructData_2.TreadID,
                                       SIGUSR2,
                                       vSgval_2);
         vRetErrorcode = errno;
         if (vRetcode != 0 )printf("WaitSignal_Sleep : char = %c vRetcode=%d errno=%d \n",vChar,vRetcode,vRetErrorcode);

      }
   }
return 0;

}

//********************************************************
//*
//********************************************************
void main_WaitSignal()
{

   //WaitSignal_Timer();
   WaitSignal_sigtimedwait();
   //WaitSignal_Sleep();

}
