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
typedef struct bTreeNode bTreeNode;
typedef struct bTree bTree;
typedef struct key key;

int bTreeInit(int order, int(*)(key* keyA, key* keyB));
int bTreeInsert(key* k);
int bTreeRemove(key* k);

int initFromBuff(void* buff);
int dumpIntoBuff(void* buff);

#endif /* _HUBASE_BTREE_H_ */ 
