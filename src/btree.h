/**
 *  btree.h
 *  apis of btree.
 *
 *  Author: lwz
 *  Since:  2017-02-23
 **/

#ifndef _HUBASE_BTREE_H_
#define _HUBASE_BTREE_H_

#include "./HuBaseInt.h"

typedef struct bTree bTree;
typedef struct key key;

int bTreeInit(int order, int(*)(key* keyA, key* keyB),bTree** ppTree);
int bTreeInsert(key* k);
int bTreeRemove(key* k);
int initKey(key** ppKey,void* buff,u32 length);
u32 keysInTheTree(bTree* pTree);
u32 nodesInTheTree(bTree* pTree);
u16 heightOfTree(bTree* pTree);
int initFromBuff(void* buff);
int dumpIntoBuff(void* buff);
int releaseTree(bTree* pTree);

#ifdef HUBASE_DEBUG
void testListInsert(bTree* pTree,int* pInt,int num);
void testListRemove(bTree* pTree,int num);
void displayList(bTree* testTree);
#endif

#endif /* _HUBASE_BTREE_H_ */ 
