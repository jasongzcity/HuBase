/**
 * Interfaces for malloc.c
 *
 * Author: lwz
 * Since: 2017-03-01 
 **/

#ifndef _HUBASE_MALLOC_H_
#define _HUBASE_MALLOC_H_

#include "./HuBaseInt.h"

void* huBaseMallocRaw(u64);
void huBaseFreeRaw(void*);
#endif /* _HUBASE_MALLOC_H_ */
