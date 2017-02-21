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

#include <sys/type.h>

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

typedef struct huBase_io_methods huBase_io_methods;
typedef struct huBase_file huBase_file;

/**
 *
 * IO_method "sets".
 * The abstraction of io methods.
 *
 * Different files can use different method "strategies".
 **/
struct huBase_io_methods {
    int (*xClose)( huBase_file* file );                                             /* Close the file */
    int (*xRead)( huBase_file* file, void* buff, int amount, h_i64 offset );        /* Read the file */
    int (*xWrite)( huBase_file* file, const void* buff, int amount, h_i64 offset ); /* Write buff */
    int (*xTrunc)( huBase_file* file, h_i64 size );                                 /* Truncate file */
    int (*xSync)( huBase_file* , int flag );                                        /* Sync file */
    int (*xLock)( huBase_file* file, int flag );
    int (*xUnlock)( huBase_file* file, int flag );
    int (*xFileSize)( huBase_file* file, h_i64* size );                             /* get the file size */
    int (*xFileControl)( huBase_file* file, int opcode, void* pArg );               /* file control */
};

/* File abstraction */
struct huBase_file {
    const huBase_io_methods* pMethods;   
};

#endif /* _HUBASE_H_*/


