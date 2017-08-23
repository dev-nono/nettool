/*/
 * queue.c
 *
 *  Created on: Aug 22, 2017
 *      Author: hico
 */



#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sItemData
{
      char  m_DeviceName[128];
      void* m_pfunction;
      int   m_FlagToCheck;
};
typedef struct sItemData ItemData_t;


TAILQ_HEAD(sTAILQ_List, sTAILQ_ListItem) ;


//struct sTAILQ_List
//{
//      struct sTAILQ_ListItem *tqh_first;
//      struct sTAILQ_ListItem * *tqh_last;
//
//};
typedef struct sTAILQ_List sTAILQ_List_t;

struct sTAILQ_ListItem
{
      TAILQ_ENTRY( sTAILQ_ListItem) m_Entries;

      void* p_Data;
};
typedef struct sTAILQ_ListItem sTAILQ_ListItem_t;


//****************************************************
//*
//****************************************************
int  list_init(sTAILQ_List_t* a_pList)
{
   int vRetcode = 0;


   if( a_pList )
   {
      TAILQ_INIT(a_pList);// Initialisation de liste
   }
   else
   {
      vRetcode  = -1;
   }

   return vRetcode;
}
//****************************************************
//*
//****************************************************
sTAILQ_ListItem_t* list_add_tail(sTAILQ_List_t* a_pList , void* a_pData)
{
   sTAILQ_ListItem_t * pRetvalue = 0;


   if( (a_pList == 0 ) || (a_pData == 0 ) )
   {
      pRetvalue = 0;
   }
   else
   {
      pRetvalue = (sTAILQ_ListItem_t * ) calloc(1,sizeof(sTAILQ_ListItem_t));
      pRetvalue->p_Data = a_pData;

      TAILQ_INSERT_TAIL(a_pList, pRetvalue, m_Entries);
   }

   return pRetvalue;

}
//****************************************************
//*
//****************************************************
int  list_remove(sTAILQ_List_t* a_pList , sTAILQ_ListItem_t *a_pItem)
{
   int pRetvalue = 0;


   if( (a_pList == 0 ) || (a_pItem == 0 ) )
   {
      pRetvalue = -1;
   }
   else
   {
      TAILQ_REMOVE(a_pList, a_pList->tqh_first, m_Entries);
      free(a_pItem);
      a_pItem = 0;
   }

   return pRetvalue;
}
int list_concat(sTAILQ_List_t* a_pList_1, sTAILQ_List_t* a_pList_2)
{
   int vRetvalue = 0;

   return vRetvalue;

}
int list_next(sTAILQ_ListItem_t* a_pItem)
{

}
int list_insert_after()
{

}
//****************************************************
//*
//****************************************************
int main_queue(int argc, char **argv)
{

   sTAILQ_List_t     vList;
   sTAILQ_ListItem_t *pItemList = 0;
   sTAILQ_ListItem_t *pItemList1 = 0;
   sTAILQ_ListItem_t *pItemList2 = 0;

   list_init(&vList);

   ItemData_t  vItemData1;
   ItemData_t  vItemData2;

   ItemData_t  *pItemData = 0;

   memset(&vItemData1,0,sizeof(vItemData1));
   memset(&vItemData2,0,sizeof(vItemData2));

   strcpy(vItemData1.m_DeviceName,"eth0");
   strcpy(vItemData2.m_DeviceName,"eth1");

   pItemList1 = list_add_tail(&vList,&vItemData1);
   pItemList2 = list_add_tail(&vList,&vItemData2);

   TAILQ_FOREACH(pItemList, &vList, m_Entries)
   {
      pItemData = (ItemData_t*)pItemList->p_Data;

      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
   }
   printf("***********************\n");

   list_remove(&vList,pItemList1);

   TAILQ_FOREACH(pItemList, &vList, m_Entries)
   {
      pItemData = (ItemData_t*)pItemList->p_Data;

      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
   }

   printf("***********************\n");

   list_remove(&vList,pItemList2);

   TAILQ_FOREACH(pItemList, &vList, m_Entries)
   {
      pItemData = (ItemData_t*)pItemList->p_Data;

      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
   }

   printf("***********************\n");


   TAILQ_FOREACH(pItemList, &vList, m_Entries)
   {
      pItemData = (ItemData_t*)pItemList->p_Data;

      printf("m_DeviceName = %s \n",pItemData->m_DeviceName);
   }

return 0;
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



   return 0;
}
