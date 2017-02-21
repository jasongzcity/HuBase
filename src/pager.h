/**
 * pager.h
 * Author: lwz
 * Since: 2017/02/20
 *
 * Define interfaces for pager layer
 **/

#ifndef _PAGER_H_
#define _PAGER_H_

#include "./HuBaseInt.h"

/* Operations on pager */
int pagerOpen(Pager** ppPager, int);
int pagerClose(Pager*);
int setPagerCacheSize(Pager*, u64);
int setPageSize(Pager*, u32);
int setMmapSize(Pager*, u64);          /* Memory map function is not supported currently */
int flushPager(Pager*);
int getSharedLock(Pager*);
int getReservedLock(Pager*);
int getExclusiveLock(Pager*);
int pagerBeginTransaction(Pager*);
int pagerCommit(Pager*);
int pagerRollback(Pager*);
int pagerMemoryUsed(Pager*);

/* Operations on page */
int getPage(Pager*, u32, Page**);
int releasePage(Pager*, Page*);
int getPageData(Page*, void**);

#endif /* _PAGER_H_ */
