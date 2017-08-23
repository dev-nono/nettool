//============================================================================
// Name        : test_cpp.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

//#include "WaitFifo.h"
#include "WaitGpio.h"

#include "test.h"

#define MAKE_SEC(a_time) ((double) ( (double)(a_time.tv_sec) + ((double)(a_time.tv_usec / 1e6) )))
#define GET_TIME_1(IDX_1,IDX_2)   struct timeval tv_ ## IDX_1 , tv_ ## IDX_2;    gettimeofday(&tv_ ## IDX_1,0);
#define GET_TIME_2(IDX_1,IDX_2,Comment)   gettimeofday(&tv_ ## IDX_2,0);    printf("\n*********** %s diff = %f\n",Comment,MAKE_SEC(tv_ ## IDX_2) - MAKE_SEC(tv_ ## IDX_1) );



#define TEST_READ_FILE           1
#define TEST_PIPE                2
#define TEST_GET_TIME_OF_DAY     3
#define TEST_WAIT_GPIO           4
#define TEST_WAIT_FIFO           5
#define TEST_STRING              6
#define TEST_IPCALC              7
#define TEST_INET_PTON           8
#define TEST_STRING_SUBSTR       9
#define TEST_SSCANF              10
#define TEST_CALC_MASK           11
#define TEST_CHECK_STATE_MACHINE 12
#define TEST_ARGV                13
#define TEST_COUNT_DONW          14
#define TEST_TIMER               15
#define TEST_CONDITION_VARIABLE  16
#define TEST_WAIT_SIGNAL         17
#define TEST_SLEEP               18
#define TEST_MODF                19
#define TEST_LONG_DOUBLE         20
#define TEST_UTIL_TIMESPEC       21
#define TEST_GPIO_SELECT         22
#define TEST_QUEUE               23


int Test_WaitFifo()
{

   // WaitFifoChar()((char*)"./wait.fifo",'1','0');
   printf("passage_1\n");

   //WaitFifoChar(((char*)"./wait.fifo",'1','0');
   printf("passage_2\n");

   return 0;

}

int Test_WaitGpio()
{
   int ii = 0;
   char vCondition = '1';

   for( ii = 0; ii < 10; ii ++ )
   {
      WaitGpio((char*)"/tmp/waitdebug",1,vCondition);
   }

   return 0;
}
int Test_gettimeofday()
{
   {
      GET_TIME_1(1,2)
        sleep(1);
      GET_TIME_2(1,2,"Test_gettimeofday")
   }
   {
      GET_TIME_1(1,2)
        sleep(2);
      GET_TIME_2(1,2,"Test_gettimeofday")
   }
   return 0;
}

void PrintUsage()
{
   printf( "\n%d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n %d %s\n \n",
           TEST_READ_FILE ,                  "CheckReadFile(argc, argv);",
           TEST_PIPE ,                       "CheckPipe",
           TEST_GET_TIME_OF_DAY,             "Test_gettimeofday",
           TEST_WAIT_GPIO,                   "Test_WaitGpio",
           TEST_WAIT_FIFO,                   "Test_WaitFifo",
           TEST_STRING,                      "TestString",
           TEST_IPCALC,                      "ipcalcMain( argc, argv);",
           TEST_INET_PTON,                   "Net_inet_pton( argc, argv) : 8 [01] ip ",
           TEST_STRING_SUBSTR,               "StringSubstr( argc, argv);",
           TEST_SSCANF,                      "Sscanf(int argc, char **argv)", //
           TEST_CALC_MASK,                   "CalcMask(int argc, char **argv)", //
           TEST_CHECK_STATE_MACHINE,         "CheckStateMachine",
           TEST_ARGV,                        "Test_Argv",
           TEST_COUNT_DONW,                  "Countdown(int argc, char **argv)",
           TEST_TIMER,                       "timer(int argc, char **argv)",
           TEST_CONDITION_VARIABLE,          "TEST_CONDITION_VARIABLE",
           TEST_WAIT_SIGNAL,                 "TEST_WAIT_SIGNAL",
           TEST_SLEEP,                       "TEST_SLEEP",
           TEST_MODF,                        "TEST_MODF",
           TEST_LONG_DOUBLE,                 "TEST_LONG_DOUBLE",
           TEST_UTIL_TIMESPEC,               "TEST_UTIL_TIMESPEC",
           TEST_GPIO_SELECT,				      "TEST_GPIO_SELECT",
           TEST_QUEUE,                       "TEST_QUEUE"

   );


}

int main(int argc, char **argv)
{
   int Val = 0;

   if( argc >1 )
   {
      Val =  atoi(argv[1]);

      switch ( Val )
      {
         case TEST_READ_FILE:
         {
            CheckReadFile(argc, argv);
            break;
         }
         case TEST_PIPE:
         {
            //CheckPipe(argc, argv);
            break;
         }
         case TEST_GET_TIME_OF_DAY:
         {
            Test_gettimeofday();
            break;
         }
         case TEST_WAIT_GPIO:
         {
            Test_WaitGpio();
            break;
         }
         case TEST_WAIT_FIFO:
         {
            Test_WaitFifo();
            break;
         }
         case TEST_STRING:
         {
            TestString( );
            break;
         }
         case TEST_IPCALC:
         {
            ipcalcMain( argc, argv);
            break;
         }
         case TEST_INET_PTON:
         {
            Net_inet_pton( argc, argv);
            break;
         }
         case TEST_STRING_SUBSTR:
         {
            StringSubstr( argc, argv);
            break;
         }
         case TEST_SSCANF:
         {
            Sscanf( argc, argv);
            break;
         }
         case TEST_CALC_MASK:
         {
            CalcMask(argc, argv);
            break;
         }
         case TEST_CHECK_STATE_MACHINE:
         {
            CheckStateMachine();
            break;
         }
         case TEST_ARGV:
         {
            Test_Argv();
            break;
         }
         case TEST_COUNT_DONW:
         {
            CountDown(argc, argv);
            break;
         }
         case TEST_TIMER :
         {
            main_Timer(argc, argv);
            break;
         }
         case TEST_CONDITION_VARIABLE :
         {
            main_ConditionVariable();
            break;
         }
         case TEST_WAIT_SIGNAL :
         {
            main_WaitSignal();
            break;
         }
         case TEST_SLEEP :
         {
            main_Sleep();
            break;
         }
          case TEST_MODF :
         {
            Check_Modf();
            break;
         }
          case TEST_LONG_DOUBLE :
          {
             Check_LongDouble();
             break;
          }
          case TEST_UTIL_TIMESPEC :
          {
             Test_unit_Util_timespec_main();
             break;
          }
          case TEST_GPIO_SELECT:
          {
        	  Test_Gpio_Select();
        	  break;
          }
          case TEST_QUEUE:
          {
             main_queue( argc, argv);
             break;
          }
    default:
            PrintUsage();
            break;
      };
   }
   else
   {
      PrintUsage();
   }


   return 0;

}
