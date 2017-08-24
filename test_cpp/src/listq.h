/*
 * list.h
 *
 *  Created on: Aug 24, 2017
 *      Author: hico
 */

#ifndef LISTQ_H_
#define LISTQ_H_


typedef void*     listq_t;
typedef void*     listq_item_t;


listq_t              listq_create();


listq_item_t         listq_add_tail(   listq_t        a_pList ,
                                       void*          a_pData);

void*                listq_getData(    listq_item_t   a_pItem);

int                  listq_removeItem(        listq_t       a_pList ,
                                          listq_item_t  a_pItem);

int                  listq_concat(     listq_t        a_pList ,
                                       void*          a_pData);

listq_item_t         listq_head(       listq_t        a_pList);

listq_item_t         listq_next(       listq_item_t   a_pItem);

listq_item_t         listq_end(        listq_t        a_pList);

int                  listq_insert_after( listq_t      a_pList ,
                                        void*         a_pData);


#endif /* LISTQ_H_ */
