/**
 * vector.h
 * vector is a wrapper around an array. 
 * 
 * Author: lwz
 * Since: 2017-02-28
 **/

#ifndef _HUBASE_VECTOR_H_
#define _HUBASE_VECTOR_H_

#include "./HuBaseInt.h"

typedef struct h_vector h_vector;

/* init a vector using element size, initial size, expand percentage & shrink percentage */ 
h_vector* initVector(u32 ele_size,u32 init_size,float expand_pt,float shrink_pt);
int releaseVector(h_vector*); /* free a vector */
int insertElemAt(h_vector*,u64 rank,void* elem);
int insertElem(h_vector*,void* elem);
int removeElem(h_vector*,u64 rank,void* elem);
int getElem(h_vector*,u64 rank,void** elem);

u64 vectorSize(h_vector* vec);
u64 vectorCap(h_vector* vec);

#endif /* _HUBASE_VECTOR_H_ */
