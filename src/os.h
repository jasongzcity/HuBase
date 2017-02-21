/**
 * 
 * os.h
 *
 * Since: 2017-02-06
 * Author: lwz
 *
 * ---------------------------
 * 
 * The interfaces of OS layer.
 *
 **/

#ifndef _HUBASE_OS_H_
#define _HUBASE_OS_H_

#include "./HuBaseInt.h"

/* This database cannot run on Windows. */
#if defined(_WIN32) || defined(_WIN64)
 #error "HuBase cannot run on windows"
#endif 

/**
 *
 * Unlike sqlite3, I am not intended to use a 
 * virtual file system to represent different lock 
 * strategies.
 *
 * So, the upper layer will manipulate files using huBase_file.
 *
 **/
void huBaseClose( huBase_file* );                         /* Close the file */
int huBaseRead( huBase_file*, void* buff, int, i64 );     /* Read the file */
int huBaseWrite( huBase_file*, const void*, int, i64 );   /* Write buff */
int huBaseTruncate( huBase_file*, i64 );                  /* Truncate file */
int huBaseSync( huBase_file*, int );                      /* Sync file */
int huBaseFileSize( huBase_file*, i64* );
int huBaseLock( huBase_file*, int );
int huBaseUnlock( huBase_file*, int );
int huBaseFileControl( huBase_file*, int, void* );        /* file control */

/**
 * Open the file on the path and return the huBase_file object. 
 * use the flag to choose different io_methods strategies for the file.
 **/
int huBaseOpen( const char* path, huBase_file* fill, int openFlag );

#endif /* _HUBASE_OS_H_*/


