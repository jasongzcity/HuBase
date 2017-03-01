/**
 * HuBaseInt.h
 *
 * Defines internal apis and structure.
 * Since: 2017/02/17
 * Author: lwz
 **/

#ifndef _HUBASEINT_H_
#define _HUBASEINT_H_

#include <stddef.h>
#include "./HuBase.h"

/* define result codes */
#define HUBASE_OK 0
#define HUBASE_BUSY 1
#define HUBASE_ERROR 2
#define HUBASE_OUTOFMEMORY 3

/* round 8 of x for byte alignment while allocating */
#define ROUND8(x) (((x)+7)&~7)

/**
 * define internal types.
 **/
/* unsigned */
typedef h_u8 u8;
typedef h_u16 u16;
typedef h_u32 u32;
typedef h_u64 u64;

/* signed */
typedef h_i8 i8;
typedef h_i16 i16;
typedef h_i32 i32;
typedef h_i64 i64;

typedef struct Pager Pager;
typedef struct Page Page;
typedef struct PageCache pageCache;
typedef struct IOmethods IOmethods;
typedef struct huBase_file huBase_file;

/*  the "huBase" instance */
struct huBase {
    Pager* pPager;            /* pager handler */
};

/**
 * IO_method "sets".
 * The abstraction of io methods.
 *
 * Different files can use different method "strategies".
 **/
struct IOmethods {
    int (*xClose)( huBase_file* file );                                             /* Close the file */
    int (*xRead)( huBase_file* file, void* buff, int amount, h_i64 offset );        /* Read the file */
    int (*xWrite)( huBase_file* file, const void* buff, int amount, h_i64 offset ); /* Write buff */
    int (*xTrunc)( huBase_file* file, h_i64 size );                                 /* Truncate file */
    int (*xSync)( huBase_file*, int flag );                                         /* Sync file */
    int (*xLock)( huBase_file* file, int flag );
    int (*xUnlock)( huBase_file* file, int flag );
    int (*xFileSize)( huBase_file* file, h_i64* size );                             /* get the file size */
    int (*xFileControl)( huBase_file* file, int opcode, void* pArg );               /* file control */
};

/* File abstraction */
struct huBase_file {
    const IOmethods* pMethods;   
};

struct Pager {
    huBase_file* pDbf;          /* pager handle the database file directyly */
    u32 pageSz;                 /* the size of the page, default 1024 */
    u32 pageNum;                /* number of pages currently in database. */
    u32 origPageNum;            /* number of pages in the database before transaction */
    huBase_file* pJournal;      /* the pointer to the Journal file */
    u8 lockMode;                /* current lock mode for the database file */
    int errCode;                /* the error code for the last operation */
    u64 mmapSz;                 /* the size of the mmap buffer, default 1G */
    pageCache* pPgCache;        /* Cache of the pages. */
};

struct Page {
    u32 pgNo;           /* the number of this page. */
    Pager* pPager;      /* the pager which owns this page */
    void* pData;        /* pointer the data in this page */
    u16 hashValue;      /* the hash Value of this page */
    Page* pNext;        /* next page in the bucket, used in pageCache */
    int isPinned;       /* equals 1 if this page is pinned, 0 if not pinned */ 
};

struct PageCache {
    /* implement later, should be the subclass for hash table */
};



int arrayCopy(void* src,u64 src_off,void* tar,u64 tar_off,u64 len,int cflag);
int slideInArray(void* arr,u64 begin,u64 len,i64 offset);

#endif /* _HUBASEINT_H_ */
