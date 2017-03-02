/**
 * util.c 
 * contains util methods.
 *
 * Author:lwz
 * Since: 2017-02-28 
 **/

#include "./HuBaseInt.h"

/** 
 * Array copy routine,
 * copy src[src_off+len]
 * to tar[tar_off+len] (both lengths are counted in byte)
 * caller should ensure enough lengths in the arrays.
 *
 * Use cflag to control the sequence of copy. This is 
 * especially useful when the src and the tar are in the 
 * same array and the copying may cause data lost.
 * Set cflag to 1(which is also the default way) 
 * to copy from the beginning to the end. Set cflag to 2 
 * to copy from the end to the beginning, in case that 
 * the tar is in the "later part" of the src array.
 **/
int arrayCopy(void* src,u64 src_off,void* tar,u64 tar_off,u64 len,int cflag)
{
    char *s,*t;
    int i;
    if(src+src_off == tar+tar_off)
        return HUBASE_OK;
    t = (char*)tar;
    s = (char*)src;
    if(cflag==2)
    {
        for(i=len-1;i>-1;i--)
            *(t+tar_off+i) = *(s+src_off+i);
    }
    else
    {
        for(i=0;i<len;i++)
            *(t+tar_off+i) = *(s+src_off+i);
    }
    return HUBASE_OK;
}

/**
 * "Slide" data from begin(inclusive) in length len.
 * Calls arrayCopy routine to "slide".
 *
 * Note: caller should check if there are enough spaces in the array
 * before use.
 **/
int slideInArray(void* arr,u64 begin,u64 len,i64 offset)
{
    int cflag;
    u64 tar = 0;
    u64 t_offset;
    if(offset>-1)
    {
        cflag = 2;    /* copy from the end to the beginning */
        tar = begin+offset;
    }
    else
    {
        t_offset = (u64)(-offset);
        tar = begin - t_offset;
        cflag = 1;
    }
    arrayCopy(arr,begin,arr,tar,len,cflag);
    return HUBASE_OK;
}
