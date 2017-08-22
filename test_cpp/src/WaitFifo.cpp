/*
 * test.c
 *
 *  Created on: Jan 13, 2017
 *      Author: hico
 */


#include <stdio.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>

//#define __USE_GNU
#include <poll.h>
#include <errno.h>
//#include <sys/inotify.h>


int WaitFifoString(char* a_FifoName ,char* a_WaitCondition )
{
   int vRes = 0;

   return vRes;
}

int WaitFifoChar(char* a_FifoName ,char a_WaitCondition )
{
   int      vFileDescriptor      = 0;
   int      vRes                 = 0;
   char     ReadBuffer[1024+1];
   int      LenToRead            = 0;
   struct pollfd  vPollFd[1];
   int      vTimeOut             = -1;

   memset(ReadBuffer,0,sizeof(ReadBuffer));

   unlink(a_FifoName);
   /* create the FIFO (named pipe) */
   vRes = mkfifo(a_FifoName, 0666 ); //   S_IRUSR | S_IWUSR 0666 S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

   if( vRes != 0 )
   {
      printf("*** Error mkfifo(%s) res= %d  errno=%d %s\n",a_FifoName,vRes,errno,strerror(errno));
   }

   if( vRes == 0 )
   {
      vFileDescriptor = open( a_FifoName, O_RDWR );//O_RDWR O_RDONLY O_WRONLY
      //printf("vFileDescriptor = %d \n",vFileDescriptor);

      if( vFileDescriptor< 0 )
      {
         printf("*** Error open(%s) res= %d  errno=%d %s\n",a_FifoName,vRes,errno,strerror(errno));
         vRes = -1;
      }
      else
      {
         vPollFd[0].fd     = vFileDescriptor;
         vPollFd[0].events = POLLIN ; // POLLOUT | POLLPRI
      }
   }

   if( vRes == 0 )
   {
      do
      {
         memset(ReadBuffer,0,sizeof(ReadBuffer));

         vRes = poll(vPollFd , 1 ,vTimeOut );
         /*printf("vRes= %d POLLIN= %d , POLLPRI= %d POLLOUT= %d\n",                vRes,vPollFd[0].revents & POLLIN, vPollFd[0].revents & POLLPRI, vPollFd[0].revents & POLLOUT         );*/

         vPollFd[0].revents = 0;

         if(vRes  == -1)
         {
            perror("select"); /* an error accured */
            break;
         }
         else if(vRes  == 0)
         {
            printf("timeout"); /* a timeout occured */
            break;
         }
         else
         {
            LenToRead = 0;
            vRes  = ioctl (vFileDescriptor,FIONREAD,&LenToRead);
            // printf("ioctl res = %d LenToRead=%d\n",vRes,LenToRead);

            if ( LenToRead > 0 )
            {
               vRes  = read(vFileDescriptor,ReadBuffer,LenToRead);
               //printf("ReadBuffer= %c %d 0X%X\n", ReadBuffer[0] , ReadBuffer[0],ReadBuffer[0]);

               if(   ( vRes > 0 )
                  && ( ReadBuffer[0] == a_WaitCondition ) )
               {
                  break;
               }
            }
         }

      } while(  1 );

   }

   close(vFileDescriptor);

   /* remove the FIFO */
   unlink(a_FifoName);

   return vRes;
}
