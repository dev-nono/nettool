/*
 * WaitGpio.h
 *
 *  Created on: Jan 3, 2017
 *      Author: hico
 */

#ifndef WAITGPIO_H_
#define WAITGPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/inotify.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


extern int WaitGpio(char* a_Filename,char a_WaitCondition,char a_ResetValue );
#ifdef __cplusplus
}
#endif

#endif /* WAITGPIO_H_ */
