/*
 * Network.c
 *
 *  Created on: Feb 6, 2017
 *      Author: hico
 */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "test.h"

#define V6TYPE_STANDARD 1
#define V6TYPE_V4INV6 2


//*************************************************
//*   argv[1] : 8
//*   argv[2] : 0 to IPv4, 1 to IPv6
//*   argv[3] : address IP
//*
//*************************************************
void Net_inet_pton(int argc, char **argv)
{
   printf("\n Net_inet_pton( argc=%d argv[1]=%s argv[2]=%s argv[3]=%s \n\n",
          argc,
          (argc > 1) ? argv[1] : "",
          (argc > 2) ? argv[2] : "",
          (argc > 3) ? argv[3] : "");
   union
   {
         struct in_addr    inet4;
         struct in6_addr   inet6;
         char              Buffer[1024];
   } vTranslatedAddress;

   int      vAddressFamily = 0;
   void*    pAddr       = 0;
   int      vRetcode    = 0;
   int   ii =0;
   memset(&vTranslatedAddress,0,sizeof(vTranslatedAddress));

   if( argc > (4-1) )
   {
      if( argv[2][0]  == '0' ) // IPV4
      {
         vAddressFamily = AF_INET;
         pAddr          = &(vTranslatedAddress.inet4);

      }
      else
      {
         vAddressFamily = AF_INET6;
         pAddr          = &(vTranslatedAddress.inet6);
      }

      // Try to convert address
      vRetcode = inet_pton(vAddressFamily, argv[3], pAddr);

      if( vRetcode == 1 )
      {
         printf("vRetcode=%d  OK \n\n",vRetcode);
         printf("sizeof(struct in6_addr) = %d \n",(int)sizeof(struct in6_addr));

         for( ii = 0; ii< sizeof(struct in6_addr); ii++ )
         {
            printf("%X",((char*)&vTranslatedAddress)[ii]);
         }

         printf("\n\n");

      }
      else
      {
         printf(" Error bad address :  vRetcode=%d errno=%d %s\n\n",vRetcode, errno, strerror(errno));

      }

      if( vRetcode == 1 )
      {
         int   vValue   = 0;
         //int   vPosBit  = 0;
         char  vChar    = 0;
         int   ii       = 0;
         int   jj       = 0;
         char  StrOutput[1024];
         char  Str[1024];
         char  vBuff[1024];
         int   vBase       = 0;
         int   vDecalage   = 0;
         int   vNbBit      = 0;

         //printf("\n\n" );
         for(ii = 0; ii < 8; ii++ ) // 8 fields
         {
            vValue = getcolon(argv[3],ii);
            sprintf(vBuff,"%02X%02X%c", ( (vValue & 0xFF00) >> 8) ,(vValue & 0x00FF) , ii<7 ?':' : '\0' );
            strcat(StrOutput,vBuff);

            sprintf(vBuff,"%02X%02X", ( (vValue & 0xFF00) >> 8) ,(vValue & 0x00FF) );
            strcat(Str,vBuff);
         }
         printf("\n\n %s\n\n" ,StrOutput);

         for( ii = 0; ii < (8*4) ; ii++)// 8 fields x 4
         {
            sprintf(vBuff,"0x%c",Str[ii]);
            sscanf(vBuff,"%hhx",&vChar);

           // boucle sur les bits(4)
            for ( jj = 0; jj < 4 ; jj++)
            {
               vDecalage   = 3 - jj;
               vBase       = ii * 4;

               if( vChar & ( 1 << vDecalage ) )
               {
                  vNbBit      = vBase + jj + 1;
               }
            }
         }// for()

         printf("\n bit = %d \n",vNbBit);
      }// if( vRetcode == 1 )
   }
   else
   {
      printf("\n Error : no input address ! \n\n");
   }


}// Net_inet_pton

int isV6TypeStandard(char *addr)
{
   char * pChar = 0;

   pChar = strchr(addr,'.');

   return (pChar == 0);
}
int getcolon (char *addr, int pos) //, int type)
{
   int x, y;
   int compressed;
   int cstart, cend;
   int max;
   char str[5];

   /*   if (type == V6TYPE_STANDARD)
      max = 7;
   if (type == V6TYPE_V4INV6)
      max = 5;
    */

   if( isV6TypeStandard(addr) )
      max = 7;
   else
      max = 5;

   compressed = 0;
   x           = 0;
   while (x < strlen (addr) - 1) {
      if (addr[x] == ':' && addr[x + 1] == ':')
         compressed++;
      x++;
   }

   if (compressed) {
      cstart = 0;
      x = 0;
      while (x < strlen (addr) - 1) {
         if (addr[x] == ':' && addr[x + 1] == ':')
            break;
         if (addr[x] == ':')
            cstart++;
         x++;
      }
      x += 2;
      cend = 0;
      while (x < strlen (addr)) {
         if (addr[x] == ':')
            cend++;
         x++;
      }
      if (addr[strlen (addr) - 1] == ':'
      && addr[strlen (addr) - 2] != ':')
         cend--;
   }

   if (!compressed) {
      x = 0;
      y = 0;
      while (x < pos) {
         if (addr[y] == ':')
            x++;
         y++;
      }

      bzero ((char *) str, 5);
      x = 0;
      while (y < strlen (addr) && addr[y] != ':') {
         str[x] = addr[y];
         x++;
         y++;
      }
   }

   if (compressed) {
      bzero ((char *) str, 5);
      if (pos <= cstart) {
         x = 0;
         y = 0;
         while (x < pos) {
            if (addr[y] == ':')
               x++;
            y++;
         }

         x = 0;
         while (y < strlen (addr) && addr[y] != ':') {
            str[x] = addr[y];
            x++;
            y++;
         }
      }

      if ((pos > cstart) && (pos < (max - cend))) {
         str[0] = '0';
      }

      if (pos >= (max - cend)) {
         y = 0;
         while (y < strlen (addr) - 1) {
            if (addr[y] == ':' && addr[y + 1] == ':')
               break;
            y++;
         }
         y += 2;

         x = max - cend;
         while (x < pos) {
            if (addr[y] == ':')
               x++;
            y++;
         }

         bzero ((char *) str, 5);
         x = 0;
         while (y < strlen (addr) && addr[y] != ':') {
            str[x] = addr[y];
            x++;
            y++;
         }
      }

   }

   if (str[0] == '\0')
      str[0] = '0';
   x = strtol (str, (char **) NULL, 16);

   return x;

} // getcolon (char *addr, int pos, int type)
