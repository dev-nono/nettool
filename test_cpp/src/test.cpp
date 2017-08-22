/*
 * test_cpp.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: hico
 */
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <math.h>


#include "CCounter.h"

#include "test.h"

void TestString()
{
	std::string Str = " ";


	printf("Str.size = 0x%X - 0x%X  \n Str.data()= %p 0x%X \n Str.c_str() 0x%X %p \n %s %S \n",
			Str.size() , (unsigned int)Str.length(),
			Str.data(), Str.data(),
			Str.c_str(), Str.c_str(),
			*Str.data(), *Str.c_str() 	);
}
//********************************************
//* argv[1] = 9
//* argv[2] = str
//* argv[3] = motif to find
//********************************************
void StringSubstr(int argc, char **argv)
{

   if( argc > (3-1) )
   {
      std::string str1 = argv[2];
      std::string str2;
      int         vPos = str1.find( argv[3]);

      printf( "argv[1]=%s \n argv[2]=%s\n argv[3]=%s\n\n" , argv[1], argv[2], argv[3]);

      str2 = str1.substr( vPos+1, 3) ;

      printf(" StringSubstr: vPos=%d str2= %s str2.size=%lu \n" , vPos , str2.c_str() , str2.size());

   }
   else
   {
      printf(" \n\n StringSubstr:  Syntax Error,  bad number arguments \n\n");
   }

}

//********************************************
//* argv[1] = 10
//* argv[2] = str
//********************************************
void Sscanf(int argc, char **argv)
{
   if( argc > (2-1) )
   {
      int   vNbItem = 0;
      char  vArray[8][5];

      memset(vArray,0,sizeof(vArray));


      vNbItem = sscanf(argv[2],
//                       //"%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:",
//                       //"%[^':']:%[^':']:%[^':']:",
//                       vArray[0],vArray[1],vArray[2],vArray[3],
//                       vArray[4],vArray[5],vArray[6],vArray[7]);
                       "%[^':']%[^':']%[^':']",
                       vArray[0],vArray[1],vArray[2]);


      printf(" Sscanf : vNbIte=%d %s:%s:%s:%s:%s:%s:%s:%s\n",
             vNbItem,
             vArray[0],vArray[1],vArray[2],vArray[3],
             vArray[4],vArray[5],vArray[6],vArray[7]);
   }
   else
   {
      printf(" \n\n Sscanf:  Syntax Error,  bad number arguments \n\n");
   }
}
//********************************************
//* argv[1] = 11
//* argv[2] = str
//********************************************
void CalcMask(int argc, char **argv)
{
   int ii = 0;
   std::size_t vPos = 0;

   if( argc > (2-1) )
   {
      std::string vSrc = argv[1];

      int bDouble __attribute__ ((unused)) = 0;

      vPos = vSrc.find("::",0);
      bDouble =  (vPos == std::string::npos);

      for( ii = 0; ii < 8; ii++ )
      {
         //str
      }
   }
}

void CountDown_1(int argc, char **argv)
{
   if( argc == 3 )
   {
      printf("\nCountDown :  timeout=%s \n\n",argv[2]);

      CCounter vCounter(atoi(argv[2]));
      vCounter.Start();

      do
      {
         sleep(1);
         if( vCounter.isTimeout() )
         {
            break;
         }

      }while(1);

      printf("\nTimeOut !!!!!!!!!!\n");
   }
   else
   {
      printf("\n\nCountDown : Error argc=%d \n\n",argc);
   }

}
void CountDown_2()
{
   struct timespec vTimeSpecStart   = {0,0};
   struct timespec vTimeSpecNow     = {0,0};
   int            vDiff             = 0;
   //int            vTimeout          = 100;

   clock_gettime(CLOCK_MONOTONIC_RAW,&vTimeSpecStart);

   printf("\nCountDown_2 : start = %d \n\n",(int)vTimeSpecStart.tv_sec);

   do
   {
      sleep(1);
      clock_gettime(CLOCK_MONOTONIC_RAW,&vTimeSpecNow);

      vDiff = (vTimeSpecNow.tv_sec - vTimeSpecStart.tv_sec );

      printf("CountDown_2 : now = %d diff = %d \n",
             (int)vTimeSpecNow.tv_sec,
             (int)(vTimeSpecNow.tv_sec - vTimeSpecStart.tv_sec));

      if( vDiff >= 100)
      {
         break;
      }
   }while(1);


}
void CountDown(int argc, char **argv)
{
   //CountDown_1(argc,argv);

   CountDown_2();


}
//**************************************************************
//**************************************************************
void CheckStateMachine()
{
//   CUtilNet_StateMachine vState;
//
//   vState.ResetState();
//
//
//   vState.setIp(1);
//   printf ("vState.getSatte 0x%x \n",vState.getState());
//   vState.setIp(0);
//   printf ("vState.getSatte 0x%x \n",vState.getState());
//
//





}

void Print_Argv(char* a_Argv[])
{
   //char     vBuffer[128];
   int      ii = 0;

   do
   {
      printf( "%d=%s\n" ,ii, a_Argv[ii]);
      ii++;
   }while( a_Argv[ii]);



}
void Test_Argv()
{
   char* vArgv[] = {    (char*)"arg_0",
                        (char*)"arg_1",
                        (char*)"arg_2",
                        (char*)"arg_3",
                        (char*)NULL
                  };

   Print_Argv(vArgv);

}

struct  timespec Check_Modf()
{
      struct  timespec  vTimespec;
      long double            vMillisec   = 1007.0;
      long double            vIntegral   = 0;
      long double            vFraction   = 0;

      printf("Check_Modf : \n\n");

      vFraction = modfl( (vMillisec/1000)  , &vIntegral);

      vTimespec.tv_sec     = (__time_t)vIntegral;
      vTimespec.tv_nsec    = (__syscall_slong_t)(vFraction * 1e9 );

      printf("Check_Modf : %Lf  %ld.%ld \n",
             vMillisec,
             vTimespec.tv_sec,
             vTimespec.tv_nsec);

      return vTimespec;
}

long double  Check_LongDouble()
{
   struct  timespec  vTime       = {123,456};
   long double       vRetvalue   = 0.0;
   long double       vIntegral   = 0;
   long double       vFraction   = 0;

   printf("Check_LongDouble \n\n");

   vIntegral = (long double)vTime.tv_sec;

   vFraction = ((long double)vTime.tv_nsec);
   printf("Check_LongDouble %Lf %Lf-%Lf \n",      vRetvalue,      vIntegral,      vFraction );

   vFraction /= 1e3;
   printf("Check_LongDouble %Lf %Lf-%Lf \n",      vRetvalue,      vIntegral,      vFraction );

   vFraction /= 1e3;
   printf("Check_LongDouble %Lf %Lf-%Lf \n",      vRetvalue,      vIntegral,      vFraction );

   vFraction /= 1e3;
   printf("Check_LongDouble %Lf %Lf-%Lf \n",      vRetvalue,      vIntegral,      vFraction );

   vRetvalue = vIntegral + vFraction;

   printf("Check_LongDouble %Lf %Lf-%Lf \n",
      vRetvalue,
      vIntegral,
      vFraction );

   return vRetvalue  ;




}

