/*
 * Sleep.c
 *
 *  Created on: Jun 28, 2017
 *      Author: hico
 */

#define  _GNU_SOURCE

#include <pthread.h>
#include <signal.h>

#include "time.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "Common.h"


static pthread_t  gTreadID = 0;
sigset_t          gSigsetMask;

void   Sleep_staticHandler (int a_Signal, siginfo_t *a_pSigInfo, void * a_pData)
{
   Printf("Sleep_staticHandler  a_Signal= %X-%d",a_Signal,a_Signal);
}


int Sleep_ConfigSignal(  int a_SignalToTrap )
{
   struct sigaction vAction;

   sigemptyset (&gSigsetMask);
   sigaddset (&gSigsetMask, a_SignalToTrap);

   // set signal
   pthread_sigmask(SIG_SETMASK,&gSigsetMask,NULL);

   // Block timer signal temporarily
   pthread_sigmask(SIG_BLOCK,&gSigsetMask,NULL);

   sigemptyset(&vAction.sa_mask);

   vAction.sa_sigaction = Sleep_staticHandler;
   sigaction(a_SignalToTrap, &vAction, NULL);


   Printf("*** CTimeout::ConfigSignal() : Blocking signal %X-%d",
          a_SignalToTrap,a_SignalToTrap);

   return 0;
}

static void* Sleep_funcThread(void* a_pArg)
{
   int vRetcode = 0;

   Sleep_ConfigSignal(SIGUSR1);

   Printf("Sleep_funcThread_1");
   do
   {
      Printf("Sleep_funcThread_2 WAIT()");
      errno=0;
      vRetcode = sleep(10);

      Printf("Sleep_funcThread_3 : sleep vRetcode=%d errno=%d %s",
             vRetcode,errno, strerror(errno));

   }while (1);

   return 0;

}


int main_Sleep()
{
   int vRetcode = 0;
   union sigval vValue = {0};

   vRetcode = pthread_create(&gTreadID,0,Sleep_funcThread,0);

   do
   {
      Printf("main_Sleep : Type to continue ...");
      getchar();


      vRetcode  = pthread_sigqueue(gTreadID,SIGUSR1 ,vValue);

      Printf("main_Sleep : pthread_sigqueue vRetcode=%d errno=%d %s",
             vRetcode,errno, strerror(errno));


   }   while(1);

   return vRetcode;

}
