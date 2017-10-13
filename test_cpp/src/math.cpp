/*
 * math.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: hico
 */


#include <time.h>
#include <stdint.h>
#include <stdio.h>

#include "test.h"

static int Math_Div(uint32_t a_Value)
{
   ldiv_t             vDiv        = { 0,0 };
   struct timespec   vTimeout    = { 0,0 };

   vDiv = ldiv(a_Value,1000);

   vTimeout.tv_sec   = vDiv.quot;
   vTimeout.tv_nsec  = vDiv.rem*1e6; // ms => ns

   printf("Math_Div(%10d) vTimeout.tv_sec=%10d  vTimeout.tv_nsec=%10d \n",
          a_Value,
          (int)vTimeout.tv_sec,
          (int)vTimeout.tv_nsec);

   return 0;
}


int main_Math(int argc, char **argv)
{

   Math_Div(1234);
   Math_Div(12345);
   Math_Div(123456);
   Math_Div(1234567);
   Math_Div(12345678);
   Math_Div(123456789);
   Math_Div(1234567890);
   Math_Div(1234567891);
   Math_Div(12345678911);
   Math_Div(123456789112);

   return 0;
}
