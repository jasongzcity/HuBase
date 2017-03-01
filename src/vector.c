/**
 * vector.c
 * implementation of vector.
 *
 * Author: lwz
 * Since: 2017-02-28
 **/

#include "./vector.h"
#include <assert.h>
#include "./malloc.h"

struct h_vector {
    float exp_pt;
    float shr_pt;
    u32 elemSize;       /* size of each element */
    u32 capacity;
    u32 size;
    char* pBuff;
};

static int checkExpand(h_vector* vec)
{
    void* temp;
    if((vec->size+1)/vec->capacity > vec->exp_pt)
    {
        temp = huBaseMallocRaw(vec->capacity<<1);
        if(!temp) return HUBASE_OUTOFMEMORY;
        arrayCopy(vec->pBuff,0,temp,0,vec->size*vec->elemSize,1);
        huBaseFreeRaw(vec->pBuff);
        vec->pBuff = (char*)temp;
        vec->capacity <<= 1;
    }
    return HUBASE_OK;
}

static int checkShrink(h_vector* vec)
{
    void* temp;
    if((vec->size-1)/vec->capacity < vec->shr_pt)
    {
        temp = huBaseMallocRaw(vec->capacity>>1);
        if(!temp) return HUBASE_OUTOFMEMORY;
        arrayCopy(vec->pBuff,0,temp,0,vec->size*vec->elemSize,1);
        huBaseFreeRaw(vec->pBuff);
        vec->pBuff = (char*)temp;
        vec->capacity >>= 1;
    }
    return HUBASE_OK;
}

/**
 * Init the vector.
 *
 * Some restrictions on the init parameters:
 * the expand percentage should be between 0 and 1.
 * the expand percentage should be larger than the shrink one.
 * the shrink percentage should be between 0 and 0.5 because 
 * the buff shrink into half of the its size every time.
 **/
h_vector* initVector(u32 ele_size, u32 init_size, float expand_pt, float shrink_pt)
{
    h_vector* rs;
    void* temp;
    assert(expand_pt > shrink_pt);
    assert(expand_pt>0 && expand_pt<=1);
    assert(shrink_pt>0 && shrink_pt<=0.5);
    rs = (h_vector*)huBaseMallocRaw(sizeof(h_vector));
    temp = huBaseMallocRaw(init_size*ele_size);     /* byte number = element size*element number */
    if(rs==0||temp==0)
    {
        /* TODO: set error code or print error message */
        return 0; 
    }
    rs->exp_pt = expand_pt;
    rs->shr_pt = shrink_pt;
    rs->elemSize = ele_size;
    rs->capacity = init_size;
    rs->size = 0;
    rs->pBuff = (char*)temp;
    return rs;
}

int releaseVector(h_vector* vec)
{
    huBaseFreeRaw(vec->pBuff);
    huBaseFreeRaw(vec);
    return HUBASE_OK;
}

/**
 * In this vector, I will use the primitive arrayCopy routine for
 * element assigning.
 *
 * This is a database project, not a container :-).
 **/
int insertElemAt(h_vector* vec,u64 rank,void* elem)
{
    int rs = checkExpand(vec);
    if(rs == HUBASE_OUTOFMEMORY)
        return rs;
    if(rank<=vec->size)     /* need to slide in these circumstances */
        slideInArray(vec->pBuff,(rank-1)*vec->elemSize,(vec->size-rank+1)*vec->elemSize,vec->elemSize);
    arrayCopy(elem,0,vec->pBuff,(rank-1)*vec->elemSize,vec->elemSize,1);
    vec->size++;
    return HUBASE_OK;
}

/* insert the element at the end of the vector */
int insertElem(h_vector* vec,void* elem)
{
    return insertElemAt(vec,vec->size+1,elem);
}

int removeElem(h_vector* vec,u64 rank,void* elem)
{
    return 0;
}

int getElem(h_vector* vec,u64 rank,void* elem)
{
    return 0;
}

