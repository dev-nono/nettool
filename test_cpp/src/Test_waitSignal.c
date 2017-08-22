/*
 * Test_condition_variable.c
 *
 *  Created on: Jun 22, 2017
 *      Author: hico
 */

#define _GNU_SOURCE

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

#include "ts_util.h"
#include "Common.h"

#define  SIG_STOP (SIGRTMIN+1)

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
static pthread_t           g_TreadID          = 0;
static pthread_mutex_t     g_mutex;
sigset_t                   g_SigsetMask;
struct timespec            g_Timeout = {0,0};
struct timespec            g_DateStart;


void WaitSignal_Wait(struct timespec a_Timeout)
{
//   //struct timespec      g_DateStart;
//   double          vdDateBefore = 0.0;
//   double          vdDateAfter  = 0.0;
////   double             vdDiff       = 0;
//   double          vdDelta      = 0;
//
//   double          vdInteger         = 0;
//   double          vdFraction        = 0;
// //  struct timespec    vFrequense = {2,0};
//   double          vDoubleFrequence = 2.0;
//   struct timespec      vTimeDiff = {0,0};

   int                  vRetcode = 0;
   char                 vBuffer[1024];


   sprintf(vBuffer,"WaitSignal_Wait_1");

   vRetcode  = sigtimedwait(&g_SigsetMask,NULL,&a_Timeout);

   clock_gettime(CLOCK_MONOTONIC_RAW,&g_DateStart);

   sprintf(vBuffer,"WaitSignal_Wait_2 : vretcode=%d %s",
           vRetcode,
           vRetcode > 0          ? "***********************   OK" :
           errno  == EAGAIN   ? "EAGAIN" :
           errno  == EINTR    ? "EINTR" :
           errno  == EINVAL   ? "EINVAL" : "ERROR !!!"
           );
   PrintfLn(vBuffer);

   // calcul du nouveu timeout


//   if(   ( a_pDateAfter->tv_sec < vDateNow.tv_sec)
//   {
//      vTimeout.tv_sec  = a_pDateAfter->tv_sec - vDateNow.tv_sec;
//      vTimeout.tv_nsec = a_pDateAfter->tv_nsec - vDateNow.tv_nsec;
//   }
//   else
//   {
//      vTimeout.tv_sec  = a_pDateAfter->tv_sec - vDateNow.tv_sec;
//      vTimeout.tv_nsec = a_pDateAfter->tv_nsec - vDateNow.tv_nsec;
//   }
//
//   sprintf(vBuffer,"WaitSignal_Wait_3 vTimeout.tv_sec= %ld  vTimeout.tv_sec = %ld",
//           vTimeout.tv_sec,vTimeout.tv_nsec);
//   Printf(vBuffer);

   //vdDateAfter    = tsFloat(vDateNow);
  // vdDateBefore   = tsFloat(*a_pDateAfter);

//   vTimeDiff = ts_Subtract(g_DateStart,*a_pDateAfter);
//
//      sprintf(vBuffer,"WaitSignal_Wait_3 diff_sec=%ld diff_ns=%ld  isGreater=%d",
//              vTimeDiff.tv_sec,
//              vTimeDiff .tv_nsec,
//              ts_IsGreaterThan(g_DateStart,*a_pDateAfter)
//               );
//      Printf(vBuffer);


//   vdDateBefore =  (((double)vDateNow.tv_sec)+( ((double)vDateNow.tv_nsec) /1e9) ) ;
//   vdDateAfter  =  (((double)a_pDateAfter->tv_sec)+( ((double)a_pDateAfter->tv_nsec) /1e9) ) ;
//   if( vdDateAfter < vdDateBefore )
//   {
//      vdDelta = 0;
//   }
//   else
//   {
//      //vDoubleDiff = vDoubleDateAfter - vDoubleDateBefore);
//      vdDelta = vdDateBefore + vDoubleFrequence - vdDateAfter;
//   }
//
//   vdFraction = modfl(vdDelta,&vdInteger);
//   vTimeout.tv_sec = (__time_t)vdInteger;
//   vTimeout.tv_nsec   = (__syscall_slong_t) (vdFraction*1e9);
//
//
//   sprintf(vBuffer,"WaitSignal_Wait_3 vdDateBefore = %Lf sec= %ld ns= %ld",
//           vdDateBefore,vDateNow.tv_sec, vDateNow.tv_nsec );
//   Printf(vBuffer);
//
//   sprintf(vBuffer,"WaitSignal_Wait_4 vdDateAfter  = %Lf sec= %ld ns= %ld",
//           vdDateAfter, a_pDateAfter->tv_sec,a_pDateAfter->tv_nsec);
//   Printf(vBuffer);
//
//   sprintf(vBuffer,"WaitSignal_Wait_5 Diff         = %Lf sec= %ld ns= %ld",
//              vdDelta,vTimeout.tv_sec,vTimeout.tv_nsec);
//   Printf(vBuffer);
//
}
static void    WaitSignal_Config()
{
   char               vBuffer[1024];

   sigemptyset (&g_SigsetMask);
   sigaddset (&g_SigsetMask, SIG_STOP);

   // set signal
   pthread_sigmask(SIG_SETMASK,&g_SigsetMask,NULL);

   /* Block timer signal temporarily */
   pthread_sigmask(SIG_BLOCK,&g_SigsetMask,NULL);

   sprintf(vBuffer,"WaitSignal_Config : Blocking signal %d", SIG_STOP);
   PrintfLn(vBuffer);

}
//********************************************************
//*
//********************************************************
void* WaitSignal_funcThread(void* a_pArg)
{
  //int                vRetcode __attribute__ ((unused)) = 0 ;
  char               vBuffer[1024];
  struct timespec    vDateNow;
  uint64_t           ii = 0;
  double             vdDiff       = 0;
  struct timespec    vDateAfter  = {0,0};
  struct timespec    vDiffAfter  = {0,0};
  struct timespec    vDateStart  = {2,0};

  int NbLoop = 3;

  WaitSignal_Config();

  g_Timeout.tv_sec = 2;

   PrintfLn("funcThread_1 ");
   do
   {
      PrintfLn("funcThread_2");

      clock_gettime(CLOCK_MONOTONIC_RAW,&vDateNow);

      WaitSignal_Wait(g_Timeout);

      PrintfLn("funcThread_3");

      // traitement
      for(ii = 0; ii < vDateNow.tv_sec*10000; ii ++)
      {
         vdDiff = (vdDiff *1e6/1e9) + 0xFF;
      }

      clock_gettime(CLOCK_MONOTONIC_RAW,&vDateAfter);

     vDiffAfter   = ts_Subtract(vDateAfter,g_DateStart);
     g_Timeout    = ts_Subtract(vDateStart,vDiffAfter);

     sprintf(vBuffer,"funcThread_4 diff_sec=%ld diff_ns=%ld",
             g_Timeout.tv_sec,
             g_Timeout .tv_nsec
             );

     PrintfLn(vBuffer);

      PrintfLn("funcThread_4");

     // NbLoop--;
   }
   //while(1);NbLoop
   while(NbLoop);

   PrintfLn("funcThread_5");

   sprintf(vBuffer,"unfuncThread_6 Blocking signal %d", SIG_STOP);
   PrintfLn(vBuffer);
   if (sigprocmask(SIG_UNBLOCK, &g_SigsetMask, NULL) == -1)
      errExit("sigprocmask");


   return NULL;
}
//********************************************************
//*
//********************************************************
int   WaitSignal_CreateThread()
{
   int vRetcode  = 0;
   PrintfLn("CreateThread");

   vRetcode = pthread_create(&g_TreadID,NULL,WaitSignal_funcThread,NULL);

   return vRetcode;
}
//********************************************************
//*
//********************************************************
void main_WaitSignal()
{
   char vBuffer[1024];
   int vRetcode = 0;
   pthread_mutex_init(&g_mutex, NULL);

   union sigval vValue = {0};

//   WaitSignal_Config();
   // create tread
   WaitSignal_CreateThread();

   do{

      PrintfLn("main_WaitSignal : Apuyer sur une touche pour continuer");
      getchar();

      //int raise(int sig);
//      vRetcode = raise(SIG_STOP);
//      sprintf(vBuffer,"main_WaitSignal : raise = %d %s",
//              vRetcode,
//              vRetcode == 0      ?  "NO_ERROR":
//              vRetcode == EAGAIN ? "EAGAIN" :
//              vRetcode == EINVAL ? "EINVAL" :
//              vRetcode == ENOSYS ? "ENOSYS" :
//              vRetcode == ESRCH ? "ESRCH" : "ERROR !!!");

      vRetcode  = pthread_sigqueue(g_TreadID,SIG_STOP,vValue);

      sprintf(vBuffer,"main_WaitSignal : pthread_sigqueue = %d %s",
              vRetcode,
              vRetcode == 0      ?  "NO_ERROR":
              vRetcode == EAGAIN ? "EAGAIN" :
              vRetcode == EINVAL ? "EINVAL" :
              vRetcode == ENOSYS ? "ENOSYS" :
              vRetcode == ESRCH ? "ESRCH" : "ERROR !!!");




      PrintfLn(vBuffer);

      //int sigqueue(pid_t pid, int sig, const union sigval valeur);
      //int sigqueue(pid_t pid, int sig, const union sigval valeur);



   }while(1);


}
