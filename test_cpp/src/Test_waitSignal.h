/*
 * Test_waitSignal.h
 *
 *  Created on: Jun 26, 2017
 *      Author: hico
 */

#ifndef TEST_WAITSIGNAL_H_
#define TEST_WAITSIGNAL_H_

static void    Printf_(const char *fmt, ...);
static void    Printf(char* a_Message);
static int     WaitSignal_CreateThread();
static void*   WaitSignal_funcThread(void* a_pArg);
//static void    WaitSignal_Wait(sigset_t* a_pSigsetMask,struct timespec* a_pDateAfter);
static void    WaitSignal_Wait(struct timespec a_Timeout);
static void    WaitSignal_Config();


#endif /* TEST_WAITSIGNAL_H_ */
