/*
 * Common.c
 *
 *  Created on: Jun 28, 2017
 *      Author: hico
 */


#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>


#include "Common.h"


void Printf(const char *fmt, ...)
{
    va_list args;
    char vBuffArg[512];
    int vLenBuffArg __attribute__ ((unused)) = 0;

    struct timespec vTimespec;
    char           vBuffer[1024];
    int vNbChar = 0;

    //pthread_mutex_lock(&g_mutex);


    va_start( args, fmt );

    vLenBuffArg = vsnprintf ( vBuffArg, sizeof vBuffArg, fmt, args );
    va_end (args);


    clock_gettime(CLOCK_MONOTONIC_RAW,&vTimespec);

    vNbChar  = sprintf(vBuffer,"%ld.%ld %lu %s\n",
              (long)vTimespec.tv_sec,
              (long)vTimespec.tv_nsec,
              pthread_self(),
              vBuffArg);

       //pthread_mutex_lock(&g_mutex);

       write(1,vBuffer,vNbChar);

       //pthread_mutex_unlock(&g_mutex);


}
//****************************************************
//********************************************************
//*
//********************************************************
void PrintfLn(char* a_Message)
{
   struct timespec vTimespec;
   char           vBuffer[100];
   int vNbChar = 0;


clock_gettime(CLOCK_MONOTONIC_RAW,&vTimespec);

vNbChar  = sprintf(vBuffer,"%.10ld.%.9ld %lu %s\n",
          (long)vTimespec.tv_sec,
          (long)vTimespec.tv_nsec,
          pthread_self(),
          a_Message);

   //pthread_mutex_lock(&m_mutex);

   write(1,vBuffer,vNbChar);

   //pthread_mutex_unlock(&m_mutex);

}
