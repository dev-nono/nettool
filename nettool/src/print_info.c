/*
 * print_info.c
 *
 *  Created on: Aug 18, 2017
 *      Author: hico
 */

#include <net/if.h>

#include <stdio.h>

#include "print_info.h"



//******************************************************
//*
//******************************************************
void PrintMessageFlag( unsigned a_Flags )
{


//   printf ("\n\n");
   printf ("%-20s -  %d \n",    "IFF_UP",           (a_Flags & IFF_UP)           ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_RUNNING",      (a_Flags & IFF_RUNNING)      ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_BROADCAST",    (a_Flags & IFF_BROADCAST)    ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_DEBUG",        (a_Flags & IFF_DEBUG)        ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_LOOPBACK",     (a_Flags & IFF_LOOPBACK)     ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_POINTOPOINT",  (a_Flags & IFF_POINTOPOINT)  ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_NOTRAILERS",   (a_Flags & IFF_NOTRAILERS)   ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_NOARP",        (a_Flags & IFF_NOARP)        ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_PROMISC",      (a_Flags & IFF_PROMISC)      ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_ALLMULTI",     (a_Flags & IFF_ALLMULTI)     ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_MASTER",       (a_Flags & IFF_MASTER)       ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_SLAVE",        (a_Flags & IFF_SLAVE)        ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_MULTICAST",    (a_Flags & IFF_MULTICAST)    ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_PORTSEL",      (a_Flags & IFF_PORTSEL)      ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_AUTOMEDIA",    (a_Flags & IFF_AUTOMEDIA)    ? 1 : 0);
   printf ("%-20s -  %d \n",    "IFF_DYNAMIC",      (a_Flags & IFF_DYNAMIC)      ? 1 : 0);
}
