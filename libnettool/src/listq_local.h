/*
 * listq_local.h
 *
 *  Created on: Aug 24, 2017
 *      Author: hico
 */

#ifndef LISTQ_LOCAL_H_
#define LISTQ_LOCAL_H_

#include <sys/queue.h>
#include <stddef.h>

//******************************************************************
struct sListqContainer
{
      TAILQ_HEAD(sListq, sListqContainerItem) m_Listq;

      int m_Size;
};
typedef struct sListqContainer sListqContainer_t;

//******************************************************************
struct sListqContainerItem
{
      TAILQ_ENTRY( sListqContainerItem) m_Entries;

      void*                   p_Data;
      sListqContainer_t*      p_Container;
};

typedef struct sListqContainerItem slistqContainerItem_t;


#endif /* LISTQ_LOCAL_H_ */
