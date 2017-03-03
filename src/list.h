/**
 * lish.h
 *
 * Declarations for doubly list.
 *
 * Author:lwz
 * Since: 2017-03-03
 **/

#ifndef _HUBASE_LISH_H_
#define _HUBASE_LISH_H_

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

#endif /* _HUBASE_LISH_H_ */
