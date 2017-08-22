/*
 * GpioSelect.c
 *
 *  Created on: Jul 26, 2017
 *      Author: hico
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>

#define FILE_CARRIER "/sys/class/net/eth1/carrier"

//static pthread_t g_TreadID = 0;


//********************************************************
//*
//*********************#include <pthread.h>***********************************
//static void* GpioSelect_funcThread(void* a_pArg)
//{
//
//}

//********************************************************
//*
//********************************************************
//static int GpioSelect_CreateThread()
//{
//   int vRetcode  = 0;
//   PrintfLn("GpioSelect_CreateThread");
//
//   vRetcode = pthread_create(&g_TreadID,0,GpioSelect_funcThread,0);
//
//   return vRetcode;
//}

//********************************************************
//*
//********************************************************
void GpioSelect_Select()
{
	fd_set 			vFdset;
	struct timeval 	vTv;
	int 			vRetval = 0;
	int				vFd 	= 0;
	//int				vFlags	= O_RDWR;
	char 			vBuffer[1024];

	vFd = open(FILE_CARRIER,O_RDONLY);

	/* Surveiller stdin (fd 0) en attente d'entrées */
	FD_ZERO(&vFdset);
	FD_SET(vFd, &vFdset);

	/* Attends jusqu'à 5 secondes. */
	vTv.tv_sec = 1000;
	vTv.tv_usec = 0;

	memset(vBuffer,0,sizeof(vBuffer));
	vRetval = read(vFd,vBuffer,1);
	printf("gpio : %s value %c \n",FILE_CARRIER,*vBuffer);

	do
	{
		lseek(vFd, 0, SEEK_SET);

		vRetval = select(1+1, &vFdset, 0, 0, &vTv);

		printf(" select : vRetval = %d \n",vRetval);

		memset(vBuffer,0,sizeof(vBuffer));
		vRetval = read(vFd,vBuffer,1);

		printf("gpio : %s change %c \n",FILE_CARRIER,vBuffer[0]);

	}while(1);

	close(vFd);
}

//********************************************************
//*
//********************************************************
void GpioSelect_Poll()
{
	struct pollfd	vPollfd;
	nfds_t 			vNfds = 1;

	int 			vRetval = 0;
	char 			vBuffer[1024];

	vPollfd.fd = open(FILE_CARRIER,O_RDONLY);

	vPollfd.events = POLLIN | POLLPRI ;
	//vPollfd.events = POLLIN ;
	//vPollfd.events = POLLPRI ;

	memset(vBuffer,0,sizeof(vBuffer));
	vRetval = read(vPollfd.fd,vBuffer,1);
	printf("gpio : %s value  %c \n",FILE_CARRIER,vBuffer[0]);


	do
	{
		lseek(vPollfd.fd, 0, SEEK_SET);

		vRetval = poll(&vPollfd,vNfds,-1);

		printf(" poll : vRetval = %d \n",vRetval);

		memset(vBuffer,0,sizeof(vBuffer));
		vRetval = read(vPollfd.fd,vBuffer,1);

		printf("gpio : %s change %c \n",FILE_CARRIER,*vBuffer);

	}while(1);

	close(vPollfd.fd);

}

void Test_Gpio_Select()
{
	//GpioSelect_CreateThread();

	//GpioSelect_Select();
	GpioSelect_Poll();

}
