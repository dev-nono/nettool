/*
 * WaitGpio.c
 *
 *  Created on: Jan 3, 2017
 *      Author: hico
 */

#include "stdio.h"
#include "errno.h"
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include "WaitGpio.h"


int ReadGpio(const char* a_Filename,char a_ResetValue)
{
   char StrRead[1];
   char StrWrite[1];
   FILE * FileDesc   = 0;
   //   int   vRet        = 0;

   StrRead[0]     = '0';
   StrWrite[0]    = a_ResetValue;

   FileDesc = fopen(a_Filename,"r+");

   if( FileDesc )
   {
      fread( StrRead ,1,1,FileDesc);
      printf("ReadGpio ==> StrRead[0] = %d \n",StrRead[0]);

      fseek(FileDesc,0,SEEK_SET);
      fwrite(StrWrite,1,1,FileDesc);
      fclose(FileDesc);
   }
   else
   {
      printf("ReadGpio ==> open error %d \n",errno);

      FileDesc = fopen(a_Filename,"w");
      fwrite(StrWrite,1,1,FileDesc);
      fclose(FileDesc);

   }

   return (int)StrRead[0];
}
//**********************************************
//*
//*
//**********************************************
/*int WaitGpio(char* a_Filename,int a_Timeout,char a_WaitCondition )
{
   int   vValue        = 0;

   //vValue = WaitGpio(a_Filename,-1,a_WaitCondition )

   return vValue;;
}*/

//**********************************************
//*
//* a_ResetValue 0 et > 0 fait set reset value
//*               -1 < 0  : pas de reset value
//**********************************************
int WaitGpio(char* a_Filename,char a_WaitCondition,char a_ResetValue )
{
   int   vValue        = 0;

   // open fifo
   // WaitGpio_Open()

   do
   {
      // WaitGpio_Rea
      //vValue = ReadGpio(a_Filename, a_ResetValue);

      if( vValue == -1 )
      {
         break;
      }

   } while( vValue!= (int)a_WaitCondition );

   // close FIFO
   // WaitGpio_Close()

   return 0;
}


int Check_sscanf()
{
   /*    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int nb = 0;
    char buffer[20]= {10,11,12,13,0};

    uint16_t  ua = 0;
    uint16_t  ub = 0;

    unsigned short int   uia = 0;
    unsigned short int   uib = 0;

    nb = sscanf(buffer,"%c%c%c%c",&a,&b,&c,&d);
    printf("\n nb=%d %02X_%02X_%02X_%02X\n",nb,a,b,c,d);

    sscanf(buffer,"%hu%hu",&ua,&ub);
    printf("\n%hu %hu\n ",ua,ub);

    nb = sscanf(buffer,"%u%u",&ua,&ub);
    printf("\n nb =%d %hu %hu\n ",nb,ua,ub);

    nb = sscanf(buffer,"%uh%uh",&ua,&ub);
    printf("\n nb =%d %d %d\n ",nb,ua,ub);

//    nb = sscanf(buffer,"%" SCNu16 "%" SCNu16 "\n" ,&ua,&ub);
//    printf("\n nb =%d %d %d\n ",nb,ua,ub);

    nb = sscanf(buffer,"%u%u",&uia,&uib);
    printf("\n nb =%d %d %d\n ",nb,uia,uib);

    nb = sscanf(buffer,"%h%h",&uia,&uib);
    printf("\n nb =%d %d %d\n ",nb,uia,uib);

    nb = sscanf(buffer,"%hd",&uia);
    printf("\n nb =%d %d %d\n ",nb,uia,uib);


    */

   return 0;
}
#define BUF_LEN (10 * (sizeof(struct inotify_event) + 255 + 1))

void CheckReadFile(int argc, char **argv)
{
   int   FD             = -1;
   int   res            = 0;
   int   LenToRead      = 0;
   char  ReadBuffer[BUF_LEN];
   //int   timeout_msecs  = -1;
   int   inotifyFd      = 0;
   int   FD_Watch __attribute__ ((unused)) = 0;
   struct inotify_event *pInotifyEvent = 0;
   char FILE_TO_OPEN[] = "./inputfile";
   int numRead = 0;
   char *p = 0;
   //  char buf[BUF_LEN] __attribute__ ((aligned(8)));

   FD = open(FILE_TO_OPEN, O_TRUNC | O_RDONLY);//  O_TRUNC O_RDONLY)

   inotifyFd = inotify_init();                 /* Create inotify instance */

   FD_Watch  = inotify_add_watch(inotifyFd, FILE_TO_OPEN , IN_ALL_EVENTS);

   do
   {
      printf("passage_1 res=%d errno=%d %s  size=%d\n",res,errno , strerror(errno) , (int)sizeof(struct inotify_event));

      memset(ReadBuffer,0,sizeof(ReadBuffer));

      for (;;) //* Read events forever
      {
         numRead = read(inotifyFd, ReadBuffer, BUF_LEN);

         if (numRead == 0)
         {
            printf("read() from inotify fd returned 0!\n");
         }

         if (numRead == -1)
         {
            printf("read() from inotify fd returned -1!\n");
         }

         //printf("Read %d bytes from inotify fd \n", (long) numRead);

         /* Process all of the events in buffer returned by read() */
         for (p = ReadBuffer; p < ReadBuffer + numRead; )
         {
            pInotifyEvent = (struct inotify_event *) p;

            if ( pInotifyEvent->mask & IN_MODIFY )
            {
               LenToRead = 0;
               res  = ioctl (FD,FIONREAD,&LenToRead);
               //printf("ioctl res = %d LenToRead=%d\n",res,LenToRead);

               if ( LenToRead > 0 )
               {
                  res  = read(FD,ReadBuffer,LenToRead);
                  ftruncate(FD,0);
               }
               ReadBuffer[res-1] = 0;
               printf("len = %d ReadBuffer = %s. \n", res , ReadBuffer);
            }
            //printf("mask = %d 0x%X \n",pInotifyEvent->mask,pInotifyEvent->mask);
            // IN_MODIFY = 2 , IN_CLOSE_WRITE = 8 ,
            p += sizeof(struct inotify_event) + pInotifyEvent->len;
         }
      }

   }while(1);

}
