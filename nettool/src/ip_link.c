/*
 * ip_link.c
 *
 *  Created on: Aug 18, 2017
 *      Author: hico
 */
#include <string.h>


#include "libnettool.h"

#include "print_info.h"

#include "main_nettool.h"

static int ip_link_UP(char* a_DeviceName, int a_BoolUP )
{
   int vRetcode = 0;

   vRetcode = nettool_link_up(a_DeviceName,a_BoolUP);

   return vRetcode;

}

static int ip_link_Info(char* a_DeviceName)
{
   int vRetcode = 0;

   struct ifinfomsg vIfinfomsg;

   memset(&vIfinfomsg,0,sizeof(vIfinfomsg));

   vRetcode = nettool_link_info(a_DeviceName,&vIfinfomsg);

   if( vRetcode == 0 )
   {
      PrintMessageFlag(vIfinfomsg.ifi_flags);

   }

   return vRetcode;

}

//*************************************************
//*      ./app [ lu | ld | li ] eth1
//*************************************************
int ip_link(int argc, char *argv[])
{
   int vRetcode = 0;

   if ( argc == 3  )
   {

      if( strcmp(ARG_LINK_UP, argv[1]) == 0 )
      {
         vRetcode = ip_link_UP(argv[2], 1);
      }
      else if( strcmp(ARG_LINK_DOWN, argv[1]) == 0 )
      {
         vRetcode = ip_link_UP(argv[2], 0);

      }
      else if( strcmp(ARG_LINK_INFO, argv[1]) == 0 )
      {
         vRetcode = ip_link_Info(argv[2]);
      }
      else
      {
         PrintUsage(argc,argv);
      }
   }
   else
   {
      PrintUsage(argc,argv);
   }

   return vRetcode;
}
