/*
 * list.c
 *
 *  Created on: Aug 24, 2017
 *      Author: hico
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "listq_local.h"   // type & services NOT exported
#include "listq.h"         // type & services exported

//****************************************************
//*
//****************************************************
listq_t  listq_create()
{
   sListqContainer_t* pList = calloc(1,sizeof(sListqContainer_t));

   TAILQ_INIT(&(pList->m_Listq));// Initialisation de liste

   return pList;
}
//****************************************************
//*
//****************************************************
int listq_destroy(listq_t a_pList )
{
   int                        vRetcode    = 0;
   //sListqContainer_t*         pList       = a_pList;

   if( a_pList )
   {
      vRetcode = listq_clean(a_pList);
   }
   else
   {
      vRetcode =  -1;
   }

   if( a_pList )
   {
      free(a_pList);
   }

   return vRetcode;

}
//****************************************************
//*
//****************************************************
int listq_clean(listq_t a_pList )
{
   int                        vRetcode    = 0;
   sListqContainer_t*         pList       = a_pList;
   slistqContainerItem_t*     pItem       = 0;

   if( pList )
   {
      for(  pItem = pList->m_Listq.tqh_first;
            pItem ;
            pItem = pList->m_Listq.tqh_first)
      {
         listq_removeItem(a_pList,pItem);
      }

      pList->m_Size = 0;
   }
   else
   {
      vRetcode =  -1;
   }

   return vRetcode;
}
//****************************************************
//*
//****************************************************
int listq_size(    listq_t        a_pList)
{
   int                        vRetcode    = 0;
   sListqContainer_t*         pList       = a_pList;

   if( pList )
   {
      vRetcode =    pList->m_Size;
   }
   else
   {
      vRetcode =  -1;
   }

   return vRetcode;
}

//****************************************************
//*
//****************************************************
listq_item_t listq_head(listq_t        a_pList)
{
   sListqContainer_t*   pList = (sListqContainer_t*) a_pList;

   return TAILQ_FIRST(&(pList->m_Listq));
}
//****************************************************
//*
//****************************************************

listq_item_t listq_add_tail( listq_t   a_pList ,
                             void*     a_pData)
{
   listq_item_t               pRetvalue   = 0;
   slistqContainerItem_t*    pItem       = 0;

   sListqContainer_t*   plist = (sListqContainer_t*) a_pList;

   if( (a_pList == 0 ) || (a_pData == 0 ) )
   {
      pRetvalue = 0;
   }
   else
   {
      pItem                = (slistqContainerItem_t*)calloc(1,sizeof(slistqContainerItem_t));
      pItem->p_Data        = a_pData;
      pItem->p_Container   = (sListqContainer_t*)a_pList;

      TAILQ_INSERT_TAIL(&(plist->m_Listq), pItem, m_Entries);

      pRetvalue = (listq_item_t)pItem;

      plist->m_Size++;
   }

   return pRetvalue;
}
//****************************************************
//*
//****************************************************
void* listq_getData(    listq_item_t   a_pItem)
{
   void*          pData = 0;
   slistqContainerItem_t*  pItem = 0;

   if( a_pItem )
   {
      pItem = (slistqContainerItem_t*)a_pItem;

      pData = pItem->p_Data;
   }

   return pData;
}
//****************************************************
//*
//****************************************************
int  listq_FindItem(    listq_t       a_pList ,
                        listq_item_t  a_pItem)
{
   int                        vRetcode    = 0;
   sListqContainer_t*         pList       = a_pList;
   slistqContainerItem_t*     pItem       = a_pItem;

   if(   ( a_pList == 0 )
      || ( a_pItem == 0 )
      || ( pItem->p_Container != a_pList )
      )
   {
      vRetcode =  -1;
   }
   else
   {
      for(  pItem = pList->m_Listq.tqh_first;
            pItem ;
            pItem = pList->m_Listq.tqh_first->m_Entries.tqe_next)
      {
         if( pItem == a_pItem )
         {
            vRetcode  = 1;
            break;
         }
      }
   }

   return vRetcode;
}
//****************************************************
//*
//****************************************************
int  listq_removeItem(    listq_t       a_pList ,
                          listq_item_t  a_pItem)
{
   int                        vRetcode         = 0;
   sListqContainer_t*         pContainer        = a_pList;
   slistqContainerItem_t*     pContainerItem    = a_pItem;

   if(   (pContainer       == 0 )
      || (pContainerItem   == 0 )
      || (pContainerItem->p_Container  != pContainer   ) )
   {
      vRetcode = -1;
   }
   else
   {
      vRetcode = listq_FindItem(a_pList,a_pItem);

      if( vRetcode )
      {
         TAILQ_REMOVE(  &(pContainer->m_Listq),
                        pContainerItem,
                        m_Entries);

         memset(pContainerItem,0,sizeof(slistqContainerItem_t));
         free(a_pItem);
         pContainer->m_Size--;
      }
      else
      {
         vRetcode = -2;
      }
   }

   return vRetcode;
}
//****************************************************
//*
//****************************************************
//int listq_concat(    listq_t             a_pList ,
//                     void*                a_pData)
//{
//   int vRetvalue = 0;
//
//   return vRetvalue;
//}
//****************************************************
//*
//****************************************************
listq_item_t listq_next( listq_item_t  a_pItem)
{
   listq_item_t*   pNext = 0;

   if( a_pItem )
   {

      slistqContainerItem_t* pItem = (slistqContainerItem_t*)a_pItem;

      pNext = (listq_item_t)TAILQ_NEXT(pItem,m_Entries);
   }

   return pNext;
}
//****************************************************
//*
//****************************************************
//int listq_insert_after(    listq_t             a_pList ,
//                           void*                a_pData)
//{
//   int vRetvalue = 0;
//
//   return vRetvalue;
//}

