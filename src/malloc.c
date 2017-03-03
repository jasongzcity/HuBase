/**
 * malloc.c
 * wrapper around routines malloc() and free()
 * 
 * Author: lwz
 * Since: 2017-03-01
 **/

/**
 * For now, 
 *
 * In multi-process circumstances, I should use mutex to acquire and reuse heap 
 * memory. How
 **/

#include <stdlib.h>
#include "./HuBaseInt.h"
#include <assert.h>

/* stats for memory management */
typedef struct memStat { 
    u64 bytesUsing;
    u32 blocksHold;
}memStat;


/* round 8 for byte-alignment */
static u64 memRoundUp(u64 nBytes)
{
    return ROUND8(nBytes);
}

/**
 * A "raw" method for malloc() routine.
 * Simple wrap around.
 * Should use mutex & linkedlist to manage 
 * heap memory blocks later.
 *
 * Also,the "raw" memory block can reduce the
 * "overheads" for each mem block thus faster.
 **/
void* huBaseMallocRaw(u64 nBytes)
{
    u64 realBytes = memRoundUp(nBytes);
    return malloc(realBytes);
}

/* Simple wrap around. */
void huBaseFreeRaw(void* p)
{
    free(p);
}

/**
 * allocate memory of nBytes. 
 * return by pp.
 **/
int huBaseMalloc(u64 nBytes,void** pp)
{
    assert(pp);
    *pp = malloc(memRoundUp(nBytes));
    if((*pp)==0) return HUBASE_OUTOFMEMORY;
    else return HUBASE_OK;
}

int huBaseFree(void* p)
{
    assert(p);
    free(p);
    return HUBASE_OK;
}
