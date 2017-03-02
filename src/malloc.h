/**
 * Interfaces for malloc.c
 *
 * Author: lwz
 * Since: 2017-03-01 
 **/

#ifndef _HUBASE_MALLOC_H_
#define _HUBASE_MALLOC_H_

#include "./HuBaseInt.h"

/** 
 * "Raw" allocation methods. 
 * The memory blocks which are got from 
 * this method are not in the memory management system.
 **/
void* huBaseMallocRaw(u64);
void huBaseFreeRaw(void*);

/**
 * These methods are included in the memory management system(implement later).
 *
 * The memory management system, should link all allocated memory blocks in 
 * a doubly linked list.
 **/
int huBaseMalloc(u64 nBytes,void** pp);
int huBaseFree(void*);

#endif /* _HUBASE_MALLOC_H_ */
