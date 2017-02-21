/**
 * HuBaseInt.h
 *
 * Defines internal apis and structure.
 * Since: 2017/02/17
 * Author: lwz
 **/

#ifndef _HUBASEINT_H_
#define _HUBASEINT_H_


#include "./HuBase.h"


/**
 * define own types.
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
};

struct pageCache {
    /* implement later, should be the subclass for hash table */
};
#endif /* _HUBASEINT_H_ */
