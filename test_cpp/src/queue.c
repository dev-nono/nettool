/*/
 * queue.c
 *
 *  Created on: Aug 22, 2017
 *      Author: hico
 */



#include <listq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "listq.h"

struct sItemData
{
      char  m_DeviceName[128];
      void* m_pfunction;
      int   m_FlagToCheck;
};
typedef struct sItemData ItemData_t;


//****************************************************
//*
//****************************************************
int main_queue(int argc, char **argv)
{

   listq_t       pList        = 0;
   listq_item_t  pItemList    = 0;
   listq_item_t  pItemList1   = 0;
   listq_item_t  pItemList2   = 0;

   pList = listq_create();

   ItemData_t  vItemData1;
   ItemData_t  vItemData2;

   ItemData_t  *pItemData = 0;

   memset(&vItemData1,0,sizeof(vItemData1));
   memset(&vItemData2,0,sizeof(vItemData2));

   strcpy(vItemData1.m_DeviceName,"eth0");
   strcpy(vItemData2.m_DeviceName,"eth1");

   pItemList1 = listq_add_tail(pList,&vItemData1);
   pItemList2 = listq_add_tail(pList,&vItemData2);


   for(  pItemList = listq_head(pList);
         pItemList ;
         pItemList = listq_next(pItemList)
   )
   {
      pItemData = listq_getData(pItemList);

      if( pItemData )
      {
         printf("m_DeviceName_1 = %s \n" , pItemData->m_DeviceName);
      }
   }
   listq_removeItem(pList,pItemList2);

   for(  pItemList = listq_head(pList);
         pItemList ;
         pItemList = listq_next(pItemList)
   )
   {
      pItemData = listq_getData(pItemList);

      if( pItemData )
      {
         printf("m_DeviceName_2 = %s \n" , pItemData->m_DeviceName);
      }
   }

   listq_removeItem(pList,pItemList1);

   for(  pItemList = listq_head(pList);
         pItemList ;
         pItemList = listq_next(pItemList)
   )
   {
      pItemData = listq_getData(pItemList);

      if( pItemData )
      {
         printf("m_DeviceName_3 = %s \n" , pItemData->m_DeviceName);
      }
   }

//   TAILQ_FOREACH(pItemList, pList, m_Entries)
//   {
//      pItemData = (ItemData_t*)pItemList->p_Data;
//
//      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
//   }
//   printf("***********************\n");
//
//   list_remove(&pList,pItemList1);
//
//   TAILQ_FOREACH(pItemList, &pList, m_Entries)
//   {
//      pItemData = (ItemData_t*)pItemList->p_Data;
//
//      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
//   }
//
//   printf("***********************\n");
//
//   list_remove(&pList,pItemList2);
//
//   TAILQ_FOREACH(pItemList, &pList, m_Entries)
//   {
//      pItemData = (ItemData_t*)pItemList->p_Data;
//
//      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
//   }
//
//   printf("***********************\n");
//
//
//   TAILQ_FOREACH(pItemList, &pList, m_Entries)
//   {
//      pItemData = (ItemData_t*)pItemList->p_Data;
//
//      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
//   }

#if 0
   //***********************************
   TAILQ_HEAD(sNameMyList, sItemNotification) vListNotification;

   struct sItemNotification {
         TAILQ_ENTRY(sItemNotification) m_Entries;         // Liste double */
         ItemData_t  m_ItemData;
   } *pItem1, *pItem2, *pItem;



   TAILQ_INIT(&vListNotification);                      // Initialisation de liste

   pItem1 = calloc(1,sizeof(struct sItemNotification));      // Insertion au début
   TAILQ_INSERT_TAIL(&vListNotification, pItem1, m_Entries);

   strcpy(pItem1->m_ItemData.m_DeviceName,"eth0");


   pItem2 = calloc(1,sizeof(struct sItemNotification));      // Insertion à la fin
   TAILQ_INSERT_TAIL(&vListNotification, pItem2, m_Entries);

   strcpy(pItem2->m_ItemData.m_DeviceName,"eth1");

   //   pItem3 = calloc(1,sizeof(struct sItemNotification));      // Insertion après
   //   TAILQ_INSERT_AFTER(&vListNotification, pItem1, pItem2, m_Entries);
   // Parcours en avant
   for (pItem = vListNotification.tqh_first; pItem != NULL; pItem = pItem->m_Entries.tqe_next)
   {
      printf("pItem->m_ItemData.m_DeviceName = %s \n",
             pItem->m_ItemData.m_DeviceName);

   }

   // Suppression
   while (vListNotification.tqh_first != NULL)
   {
      TAILQ_REMOVE(&vListNotification, vListNotification.tqh_first, m_Entries);
      free(vListNotification.tqh_first);
   }


#endif

   return 0;
}
