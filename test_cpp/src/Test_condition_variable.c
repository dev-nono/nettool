/*
 * Test_condition_variable.c
 *
 *  Created on: Jun 22, 2017
 *      Author: hico
 */


#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#include "Common.h"

int  main_ConditionVariable();
static void* ConditionVariable_funcThread(void* a_pArg);
static void  ConditionVariable_wait(int timeoutMs);
static int   ConditionVariable_CreateThread();


static pthread_mutex_t   m_mutex;
static pthread_cond_t    mCondition;
static pthread_t         vTreadID          = 0;


int   ConditionVariable_CreateThread()
{
   int vRetcode  = 0;
   PrintfLn("CreateThread");

   vRetcode = pthread_create(&vTreadID,NULL,ConditionVariable_funcThread,NULL);

   return vRetcode;
}


void* ConditionVariable_funcThread(void* a_pArg)
{

   PrintfLn("funcThread_1 ");
   do
   {
      PrintfLn("funcThread_2");
      ConditionVariable_wait( 20 * 1000);
      PrintfLn("funcThread_3");
   }
   while(1);

   PrintfLn("funcThread_5");

   return NULL;
}

void ConditionVariable_wait(int timeoutMs)
{
   struct timespec absTimeToWait;
   struct timeval  now;
   int status __attribute__ ((unused)) = 0;
   gettimeofday(&now, NULL);

   absTimeToWait.tv_sec  = now.tv_sec + (timeoutMs/1000);
   absTimeToWait.tv_nsec = now.tv_usec*1000L + 1000000L*(timeoutMs%1000);

   if (absTimeToWait.tv_nsec > 1000000000) {
      ++absTimeToWait.tv_sec;
      absTimeToWait.tv_nsec -= 1000000000;
   }

   pthread_mutex_lock(&m_mutex);

   PrintfLn("ConditionVariable_wait_1");

   status = pthread_cond_timedwait(&mCondition, &m_mutex, &absTimeToWait);

   PrintfLn("ConditionVariable_wait_2");


   pthread_mutex_unlock(&m_mutex);

}

int  main_ConditionVariable()
{
   int ii = 0;
   char vBuffer[1024];

   //init mutex
   pthread_mutex_init(&m_mutex, NULL);

   // create tread

   ConditionVariable_CreateThread();

   usleep(100*100);

   for(ii = 0; ii < 60 ; ii++)
   {
      sprintf(vBuffer,"ConditionVariable_main : ii = %d",ii);
      PrintfLn(vBuffer);
      sleep(1);
   }


   PrintfLn("ConditionVariable_main_1 : ");
   pthread_cond_signal(&mCondition);
   PrintfLn("ConditionVariable_main_2 : ");


//   int status = pthread_mutex_lock(&m_mutex);
//   int status = pthread_mutex_unlock(&m_mutex);

   return 0;

}
