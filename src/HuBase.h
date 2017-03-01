/**
 * HuBase.h
 *
 * Since: 2017-02-06 
 * Author: lwz
 *
 * -------------------------------
 *
 * Apis of HuBase.
 *
 **/


#ifndef _HUBASE_H_
#define _HUBASE_H_

#include <sys/types.h>

/**
 *
 * Naming: usually start with "huBase_"
 *
 **/

/**
 *
 * define own standard types.
 *
 **/
/* unsigned */
typedef __uint8_t h_u8;
typedef __uint16_t h_u16;
typedef __uint32_t h_u32;
typedef __uint64_t h_u64;

/* signed */
typedef __int8_t h_i8;
typedef __int16_t h_i16;
typedef __int32_t h_i32;
typedef __int64_t h_i64;

typedef struct huBase huBase;

#endif /* _HUBASE_H_*/


