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
    if((float)(vec->size+1)/vec->capacity > vec->exp_pt)
    {
        temp = huBaseMallocRaw((vec->capacity<<1)*vec->elemSize);
        if(!temp) return HUBASE_OUTOFMEMORY;
        arrayCopy(vec->pBuff,0,temp,0,(vec->size)*(vec->elemSize),1); 
        huBaseFreeRaw((void*)vec->pBuff);
        vec->pBuff = (char*)temp;
        vec->capacity <<= 1;
    }
    return HUBASE_OK;
}

static int checkShrink(h_vector* vec)
{
    void* temp;
    if((float)(vec->size-1)/vec->capacity < vec->shr_pt)
    {
        temp = huBaseMallocRaw((vec->capacity>>1)*vec->elemSize);
        if(!temp) return HUBASE_OUTOFMEMORY;
        arrayCopy(vec->pBuff,0,temp,0,vec->size*vec->elemSize,1);
        huBaseFreeRaw((void*)vec->pBuff);
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
    int rs;
    assert(rank <= vec->size+1);
    rs = checkExpand(vec);
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

/* Caller of this method should prepare the space for the pointer elem */
int removeElem(h_vector* vec,u64 rank,void* elem)
{
    int rs = 0;
    i64 offset = -(i64)(vec->elemSize);
    assert(rank<=vec->size);
    rs = checkShrink(vec);
    if(rs==HUBASE_OUTOFMEMORY) return rs;
    arrayCopy(vec->pBuff,(rank-1)*vec->elemSize,elem,0,vec->elemSize,1);
    if(rank<vec->size) /* need to slide */
        slideInArray(vec->pBuff,rank*vec->elemSize,(vec->size-rank)*vec->elemSize,offset);
    vec->size--;
    return HUBASE_OK;
}

int getElem(h_vector* vec,u64 rank,void** ppElem)
{
    assert(rank<=vec->size);
    *ppElem = (void*)(vec->pBuff+(rank-1)*vec->elemSize);
    return HUBASE_OK;
}

u64 vectorSize(h_vector* vec)
{
    assert(vec);
    return vec->size;
}

u64 vectorCap(h_vector* vec)
{
    assert(vec);
    return vec->capacity;
}

