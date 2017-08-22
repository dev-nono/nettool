/*
 * queue.c
 *
 *  Created on: Aug 22, 2017
 *      Author: hico
 */



#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>



int main_queue(int argc, char **argv)
{

   TAILQ_HEAD(NameMyList, sItemNotification) vListNotification;

   //struct sTailvMyList *pMyList;                 /* Tête de liste double */

   struct sItemNotification {
         TAILQ_ENTRY(sItemNotification) entries;         /* Liste double */
   } *pItem1, *pItem2, *pItem;

   TAILQ_INIT(&vListNotification);                      /* Initialisation de liste */

   pItem1 = malloc(sizeof(struct sItemNotification));      /* Insertion au début */
   TAILQ_INSERT_HEAD(&vListNotification, pItem1, entries);

   pItem1 = malloc(sizeof(struct sItemNotification));      /* Insertion à la fin */
   TAILQ_INSERT_TAIL(&vListNotification, pItem1, entries);

   pItem2 = malloc(sizeof(struct sItemNotification));      /* Insertion après */
   TAILQ_INSERT_AFTER(&vListNotification, pItem1, pItem2, entries);
   /* Parcours en avant */
   for (pItem = vListNotification.tqh_first; pItem != NULL; pItem = pItem->entries.tqe_next)
   {
      //np-> ...
      // Suppression
      while (vListNotification.tqh_first != NULL)
      {
         TAILQ_REMOVE(&vListNotification, vListNotification.tqh_first, entries);
      }
   }



   return 0;
}
