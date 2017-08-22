/*
 * WaitFifo.h
 *
 *  Created on: Jan 23, 2017
 *      Author: hico
 */

#ifndef WAITFIFO_H_
#define WAITFIFO_H_

/*
#ifdef __cplusplus
extern "C" {
#endif
*/
#include <stdlib.h>
#define WAIT_OUTPUT_PATH   "/var/home/ammk/jira/290"
#define WAIT_CMD           "/var/home/ammk/listfd "

extern int gIdx;

#define WAITFIFO_BEFORE(a_Title)    printf("\n ===> WaitFifo IN %s \n",a_Title);
#define WAITFIFO_WAIT(a_Title)      WaitFifoChar((char*)"/var/home/ammk/wait.fifo",'1');
//#define WAITFIFO_CMD(a_Idx,a_Title) { char vMessage[255]; sprintf(vMessage,"%s %s%s_%s",WAIT_CMD,WAIT_OUTPUT_PATH , a_Idx , a_Title);  system(vMessage );}
#define WAITFIFO_CMD(a_Title)    { char vMessage[255]; \
                                          sprintf(vMessage,"%s %s/%d_%s",   \
                                          WAIT_CMD , WAIT_OUTPUT_PATH , gIdx++ , a_Title); \
                                          system(vMessage );}



#define WAITFIFO_AFTER(a_Title)     printf("\n ===> WaitFifo OUT %s \n",a_Title);

//#define WAITFIFO(a_Idx,a_Title) WAITFIFO_BEFORE(a_Title) WAITFIFO_WAIT(a_Msg) WAITFIFO_CMD(a_Idx,a_Title) WAITFIFO_AFTER(a_Title)
#define WAITFIFO(a_Idx,a_Title)  WAITFIFO_CMD(a_Idx,a_Title); sleep(1);
#define WAIT_1(a_Title) WAITFIFO_BEFORE(a_Title); WAITFIFO_WAIT(a_Title) WAITFIFO_AFTER(a_Title)

/*
 #define WAIT_OUTPUT_PATH   "/var/home/ammk/"
#define WAIT_CMD           "/var/home/ammk/listfd "

#define  WAITFIFO_BEFORE(a_Title)    printf(" ===> // WAITFIFO IN %s \n",a_Title);
#define  WAITFIFO_WAIT(a_Title)      // WAITFIFOChar((char*)"/var/home/ammk/wait.fifo",'1');
//#define  WAITFIFO_CMD(a_Idx,a_Title) { char vMessage[255]; sprintf(vMessage,"%s %s%s_%s",WAIT_CMD,WAIT_OUTPUT_PATH , a_Idx , a_Title);  system(vMessage );}
#define  WAITFIFO_AFTER(a_Title)     printf("\n // WAITFIFO OUT ===> %s \n",a_Title);

#define WAITFIFO_CMD(a_Title)    { char vMessage[255]; \
                                          sprintf(vMessage,"%s %s%d_%s",   \
                                          WAIT_CMD , WAIT_OUTPUT_PATH , gIdx++ , a_Title); \
                                          system(vMessage );}


//#define // WAITFIFO(a_Idx,a_Title) // WAITFIFO_BEFORE(a_Title) // WAITFIFO_WAIT(a_Msg) // WAITFIFO_CMD(a_Idx,a_Title) // WAITFIFO_AFTER(a_Title)
#define  WAITFIFO(a_Idx,a_Title)  // WAITFIFO_CMD(a_Idx,a_Title); sleep(1);
*/


int WaitFifoString(char* a_FifoName ,char* a_WaitCondition );
int WaitFifoChar(char* a_FifoName ,char a_WaitCondition );

/*
#ifdef __cplusplus
}
#endif
*/
#endif /* WAITFIFO_H_ */
