/*
 * tu_UTIL_Timespec.c
 *
 *  Created on: Jul 3, 2017
 *      Author: hico
 */

#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "test.h"
#include "UTIL_Timespec.h"
#include "tu_UTIL_Timespec.h"


//********************************************
//*
//********************************************
void Test_unit_Util_timespec_Add()
{
   struct timespec   vTimes_1 = {0,234000456};
   struct timespec   vTimes_2 = {1,345000678};
   struct timespec   vTime    = {1,345000678};

   printf("\nTest_unit_Util_timespec_Add\n");

   vTime = CTimespec::Add_ts(vTimes_1,vTimes_2);
   printf("\tAdd_ts 1 + 2 = %ld.%ld \n",vTime.tv_sec, vTime.tv_nsec);

   printf("\tAdd_ll 1 + 2 = %lld \n",CTimespec::Add_ll(vTimes_1,vTimes_2) );
}
//********************************************
//*
//********************************************
void Test_unit_Util_timespec_Compare()
{

   printf("\nTest_unit_Util_timespec_Compare\n");

   struct timespec   vTimes_1 = {1,2345};
   struct timespec   vTimes_2 = {1,3456};
   struct timespec   vTimes_3 = {1,2345};


   printf("\tcompare  (1 == 2) = %d \n",
          CTimespec::Compare(vTimes_1,vTimes_2) != 0 );

   printf("\tCompare  (1 == 3) = %d \n",
          CTimespec::Compare(vTimes_1,vTimes_3) == 0);

   printf("\tCompare  (2 == 3) = %d \n",
          CTimespec::Compare(vTimes_2,vTimes_3) != 0);

   printf("\tCompare  ( 1 < 2 ) = %d \n",
          CTimespec::Compare(vTimes_1,vTimes_2) == -1 );

   printf("\tCompare  ( 2 > 1 ) = %d \n",
          CTimespec::Compare(vTimes_2,vTimes_1) == 1 );


}
//********************************************
//*
//********************************************
void Test_unit_Util_timespec_getNano()
{
   uint64_t       vRetvalue = 0.0;
   struct timespec   vTimespec = {0,0};

   printf("\ntimespec_getNano : \n");

          vTimespec.tv_sec = 123;
   vTimespec.tv_nsec = 456;
   vRetvalue = CTimespec::getNano(vTimespec);
   printf("\tgetNano(%ld.%09ld) : vRetvalue = %ld \n",
          vTimespec.tv_sec, vTimespec.tv_nsec, vRetvalue);

   vTimespec.tv_nsec = 4560;
   vRetvalue = CTimespec::getNano(vTimespec);
   printf("\tgetNano(%ld.%09ld) : vRetvalue = %ld \n",
          vTimespec.tv_sec, vTimespec.tv_nsec, vRetvalue);

   vTimespec.tv_nsec = 45600;
   vRetvalue = CTimespec::getNano(vTimespec);
   printf("\tgetNano(%ld.%09ld) : vRetvalue = %ld \n",
          vTimespec.tv_sec, vTimespec.tv_nsec, vRetvalue);

   vTimespec.tv_nsec = 456000;
   vRetvalue = CTimespec::getNano(vTimespec);
   printf("\tgetNano(%ld.%09ld) : vRetvalue = %ld \n",
          vTimespec.tv_sec, vTimespec.tv_nsec, vRetvalue);

   vTimespec.tv_nsec = 456000000;
   vRetvalue = CTimespec::getNano(vTimespec);
   printf("\tgetNano(%ld.%09ld) : vRetvalue = %ld \n",
          vTimespec.tv_sec, vTimespec.tv_nsec, vRetvalue);

}
//********************************************
//*
//********************************************
void Test_unit_Util_timespec_Substract()
{
   printf("\nTest_unit_Util_timespec_Substract \n\n");


   struct timespec   vTimes_1 = {0,234000456};
   struct timespec   vTimes_2 = {1,345000678};
   struct timespec   vDiff    = {0,0};

   vDiff = CTimespec::Substract_ts(vTimes_1,vTimes_2);
   printf("\tSubstract  1 - 2 = %ld.%ld \n", vDiff.tv_sec,vDiff.tv_nsec);

   vDiff = CTimespec::Substract_ts(vTimes_2,vTimes_1);
   printf("\tSubstract  2 - 1 = %ld.%ld \n", vDiff.tv_sec,vDiff.tv_nsec);

   vDiff = CTimespec::Substract_ts(vTimes_1,vTimes_1);
   printf("\tSubstract  1 - 1 = %ld.%ld \n", vDiff.tv_sec,vDiff.tv_nsec);


   printf("\n\tSubstract  1 - 2 : ns=%lld , %Lf \n",
             (td_lli)CTimespec::Substract_ll(vTimes_1,vTimes_2),
             (long double) (
             (td_lli)CTimespec::Substract_ll(vTimes_1,vTimes_2) / 1e9)
              );

   printf("\tSubstract  2 - 1 : ns=%lld , %Lf \n",
             (td_lli)CTimespec::Substract_ll(vTimes_2,vTimes_1),
             (long double) (
             (td_lli)CTimespec::Substract_ll(vTimes_2,vTimes_1) / 1e9)
              );

   printf("\tSubstract  1 - 1 : ns=%lld , %Lf \n",
             (td_lli)CTimespec::Substract_ll(vTimes_1,vTimes_1),
             (long double) (
             (td_lli)CTimespec::Substract_ll(vTimes_1,vTimes_1) / 1e9)
              );


}
//********************************************
//*
//********************************************
void Test_unit_Util_timespec_IsEqual()
{
   printf("\nTest_unit_Util_timespec_IsEqual \n\n");


   struct timespec   vTimes_1 = {1,2345};
   struct timespec   vTimes_2 = {1,3456};
   struct timespec   vTimes_3 = {1,2345};


   printf("\tIsEqual  (1 == 3) = %d \n",
          CTimespec::IsEqual(vTimes_1,vTimes_3) != 0 );


   printf("\tIsEqual  (1 == 2) = %d \n",
          CTimespec::IsEqual(vTimes_1,vTimes_2) == 0);


   printf("\tIsEqual  (2 == 3) = %d \n",
          CTimespec::IsEqual(vTimes_2,vTimes_3) == 0);


}
//********************************************
//*
//********************************************
void Test_unit_Util_timespec_IsLessThan()
{
   struct timespec   vTimes_1 = {0,2345};
   struct timespec   vTimes_2 = {1,5678};

   printf("\nTest_unit_Util_timespec_IsLessThan \n\n");

   printf("\tIsLessThan : 1 < 2 = %d\n",
      CTimespec::IsLessThan(vTimes_1,vTimes_2) != 0 );


   printf("\tIsLessThan : 2 < 1 = %d\n",
      CTimespec::IsLessThan(vTimes_2,vTimes_1) == 0 );

   printf("\tIsLessThan : 1 < 1 = %d\n",
      CTimespec::IsLessThan(vTimes_1,vTimes_1) == 0 );


   printf("\tIsLessThanOrEqual : 1 <= 2 = %d\n",
      CTimespec::IsLessOrEqual(vTimes_1,vTimes_1) != 0 );

   printf("\tIsLessThanOrEqual : 1 <= 1 = %d\n",
      CTimespec::IsLessOrEqual(vTimes_1,vTimes_1) != 0 );

   printf("\tIsLessThanOrEqual : 2 <= 1 = %d\n",
      CTimespec::IsLessOrEqual(vTimes_2,vTimes_1) == 0 );


}
//********************************************
//*
//********************************************
void Test_unit_Util_timespec_IsGreaterThan()
{
   struct timespec   vTimes_1 = {0,2345};
   struct timespec   vTimes_2 = {1,5678};

   printf("\nTest_unit_Util_timespec_IsGreaterThan \n\n");

   printf("\tIsGreaterThan : 1 > 2 = %d\n",
      CTimespec::IsGreaterThan(vTimes_1,vTimes_2) == 0 );


   printf("\tIsGreaterThan : 2 > 1 = %d\n",
      CTimespec::IsGreaterThan(vTimes_2,vTimes_1) != 0 );

   printf("\tIsGreaterThan : 1 > 1 = %d\n",
      CTimespec::IsGreaterThan(vTimes_1,vTimes_1) == 0 );


   printf("\tIsGreaterOrEqual : 1 >= 1 = %d\n",
      CTimespec::IsGreaterOrEqual(vTimes_1,vTimes_1) != 0 );

   printf("\tIsGreaterOrEqual : 1 >= 2 = %d\n",
      CTimespec::IsGreaterOrEqual(vTimes_1,vTimes_2) == 0 );

   printf("\tIsGreaterOrEqual : 2 >= 1 = %d\n",
      CTimespec::IsGreaterOrEqual(vTimes_2,vTimes_1) != 0 );

}
//********************************************
//*
//********************************************
void Test_unit_Util_timespec_main()
{

   Test_unit_Util_timespec_Add();
   Test_unit_Util_timespec_Compare();
   Test_unit_Util_timespec_getNano();
   Test_unit_Util_timespec_Substract();
   Test_unit_Util_timespec_IsEqual();
   Test_unit_Util_timespec_IsLessThan();
   Test_unit_Util_timespec_IsGreaterThan();








}
