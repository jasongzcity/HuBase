/**
 * lish.h
 *
 * Declarations for doubly list.
 *
 * Author:lwz
 * Since: 2017-03-03
 **/

#ifndef _HUBASE_LIST_H_
#define _HUBASE_LIST_H_

#include "./HuBaseInt.h"

/* Subclass of doubly list. */
typedef struct h_list h_list;
typedef struct h_listNode h_listNode;

int initList(h_list** ppList,u32 listSize,u32 nodeSize);
int releaseList(h_list* list);

/* getter */
h_listNode* firstNode(h_list* list);
h_listNode* lastNode(h_list* list);
h_listNode* nodeAt(h_list* list, u64 rank);

u64 listSize(h_list* list);

/* Insert the given node at given position. Note: the position should range from 1 to size+1 */
void insertNodeAt(h_list* list,u64 position,h_listNode* pNode);
void insertNodeAtLast(h_list* list,h_listNode* pNode);

/** 
 * Remove node at the given position, and return the node in pointer. Note: the position should
 * range from 1 to size. 
 **/
void removeNodeAt(h_list* list,u64 position,h_listNode** ppNode);
#endif /* _HUBASE_LIST_H_ */
