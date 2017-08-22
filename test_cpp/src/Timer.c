/*
 * Timer.c
 *
 *  Created on: Jun 21, 2017
 *      Author: hico
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#include "Common.h"


//static void Timer_PrintSigInfo(siginfo_t *si);
//static void Timer_handler(int sig, siginfo_t *si, void *uc);
//static int Timer_CreateTimer();
//static int Timer_ConfigureSignal();
//static void* Timer_funcThread(void* a_pArg);
//static int Timer_CreateThread();
//static int Timer_SetTime(int a_Timeout ,  int a_Interval);
//static int Timer_StopTimer();


#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

timer_t              vTimerid         = 0;
//struct sigevent      vSigevent        = {0,};;
sigset_t             vMask;
//struct sigaction     vSigaction        = {0,};
char*                pArgvSeconde      = 0;
char*                pArgvNanoSecond   = 0;
pthread_t            vTreadID          = 0;
int                  gExit             = 0;



//**********************************************************
//*
//**********************************************************
static void Timer_PrintSigInfo(siginfo_t *si)
{
   //timer_t *tidp;
   //int or;

   //tidp = si->si_value.sival_ptr;

   PrintfLn("PrintSigInfo");

//   Printf("    sival_ptr = %p; ", si->si_value.sival_ptr);
//   Printf("    *sival_ptr = 0x%lx\n", (long) *tidp);
//
//   or = timer_getoverrun(*tidp);
//   if (or == -1)
//      errExit("timer_getoverrun");
//   else
//      Printf("    overrun count = %d\n", or);
}
//**********************************************************
//*
//**********************************************************
static void Timer_handler(int sig, siginfo_t *si, void *uc)
{
//    Note: calling Printf() from a signal handler is not
//              strictly correct, since Printf() is not async-signal-safe;
//       #include <sys/types.h>       see signal(7)
   char vBuffer[1024];

   sprintf(vBuffer,"handler : Caught signal %d", sig);
   PrintfLn(vBuffer);

   Timer_PrintSigInfo(si);

   //SetTime(1,2);

   signal(sig, SIG_IGN);
}
//**********************************
//* return timer id
//**********************************
int Timer_CreateTimer()
{
   int vRetcode  = 0;
   char vBuffer[1024];
   struct sigevent      vSigevent;

   memset(&vSigevent,0,sizeof(vSigevent));

   PrintfLn("CreateTimer");

   //* Create the timer

   vSigevent.sigev_notify           = SIGEV_SIGNAL;
   vSigevent.sigev_signo            = SIGRTMIN;
   vSigevent.sigev_value.sival_ptr  = &vTimerid;

   if (timer_create(CLOCK_MONOTONIC, &vSigevent, &vTimerid) == -1)
      errExit("timer_create");

   sprintf(vBuffer,"CreateTimer : timer ID is 0x%lx", (long) vTimerid);
   PrintfLn(vBuffer);

   return vRetcode;
}
//**********************************
//* configure signal
//**********************************
int Timer_ConfigureSignal()
{
   int vRetcode  = 0;
   char vBuffer[1024];
   struct sigaction     vSigaction;

   memset(&vSigaction,0,sizeof(vSigaction));

   PrintfLn("ConfigureSignal");

   //* Establish handler for timer signal

   sprintf(vBuffer,"ConfigureSignal : Establishing handler for signal %d",SIGRTMIN);
   PrintfLn(vBuffer);

   memset(&vSigaction,0,sizeof(vSigaction));
   vSigaction.sa_flags        = SA_SIGINFO;
   vSigaction.sa_sigaction    = Timer_handler;

   sigemptyset(&vSigaction.sa_mask);

   if (sigaction(SIGRTMIN, &vSigaction, NULL) == -1)
      errExit("sigaction");

   //* Block timer signal temporarily
   sprintf(vBuffer,"ConfigureSignal : Blocking signal %d", SIGRTMIN);
   PrintfLn(vBuffer);

   sigemptyset(&vMask);
   sigaddset(&vMask, SIGRTMIN);

   if (sigprocmask(SIG_SETMASK, &vMask, NULL) == -1)
      errExit("sigprocmask");

   return vRetcode;
}

void* Timer_funcThread(void* a_pArg)
{
   int               vRetcode = 0;
   struct timespec   vTimeout;
   char              vBuffer[1024];


   PrintfLn("funcThread_1 ");


  Timer_ConfigureSignal();


   PrintfLn("funcThread_2");

   vTimeout.tv_sec   = 1000;
   vTimeout.tv_nsec  = 0;

   do
   {
      PrintfLn("funcThread_3");
      vRetcode = sigtimedwait(&vMask,NULL,&vTimeout);
      sprintf(vBuffer,"funcThread_4 vretcode=%d" ,vRetcode);
      PrintfLn(vBuffer);

      if( gExit )
         break;
   }
   while(1);

   PrintfLn("funcThread_5");

   return NULL;
}
//**********************************
//*
//**********************************
int Timer_CreateThread()
{
   int vRetcode  = 0;
   PrintfLn("CreateThread");

   vRetcode = pthread_create(&vTreadID,NULL,Timer_funcThread,NULL);

   return vRetcode;
}
//**********************************
//* configure and start timer
//**********************************
int Timer_SetTime(int a_Timeout ,  int a_Interval)
{
   int vRetcode  = 0;

   struct itimerspec    vItimerspec;

   PrintfLn("SetTime");

   /* Start the timer */

   vItimerspec.it_value.tv_sec      = a_Timeout;
   vItimerspec.it_value.tv_nsec     = 0;

   vItimerspec.it_interval.tv_sec   = a_Interval;
   vItimerspec.it_interval.tv_nsec  = 0;

   if (timer_settime(vTimerid, 0, &vItimerspec, NULL) == -1)
      errExit("timer_settime");


   return vRetcode;
}
//**********************************
//*
//**********************************
int Timer_StopTimer()
{
   int vRetcode  = 0;
   union sigval vSigval;

   vSigval.sival_int = 0;
   vSigval.sival_ptr = 0;

   PrintfLn("StopTimer");

   gExit = 1;

   //SetTime(0,0);
   pthread_sigqueue(vTreadID,SIGRTMIN,vSigval);

   return vRetcode;
}

//**********************************
//*
//**********************************
int main_Timer(int argc, char *argv[])
{
   int vRetcode  = 0;
   char vBuffer[1024];

   int ii = 0;

   if (argc != 3+1 ) {
      fprintf(stderr, "main_Timer : Usage: %s <sleep-secs> <freq-nanosecs>\n",
              argv[0]);
      exit(EXIT_FAILURE);
   }

   pArgvSeconde      = argv[2];
   pArgvNanoSecond   = argv[3];;


   Timer_CreateTimer();

   Timer_SetTime(1,2);


   Timer_CreateThread();


   //*******************************
   // wait 3 second
   //*******************************
   for( ii = 0; ii < 10; ii++)
   {
      sprintf(vBuffer,"count %d",ii);
      PrintfLn(vBuffer);
      sleep(1);

   }

   Timer_StopTimer();

   vRetcode =  pthread_join(vTreadID, NULL);


   /* Sleep for a while; meanwhile, the timer may expire
              multiple times */

   //Printf("Sleeping for %d seconds\n", atoi(pArgvNanoSecond));
  //sleep(atoi(pArgvNanoSecond));

   /* Unlock the timer signal, so that timer notification
              can be delivered */

   sprintf(vBuffer,"main_Timer : Unblocking signal %d", SIGRTMIN);
   PrintfLn(vBuffer);
   if (sigprocmask(SIG_UNBLOCK, &vMask, NULL) == -1)
      errExit("sigprocmask");

   //exit(EXIT_SUCCESS);
   return vRetcode;
}
