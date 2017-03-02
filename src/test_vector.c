/**
 * test_vector.c
 *
 * Author: lwz
 * Since: 2017-03-02
 **/

#include "./vector.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char** argv)
{
    int temp;
    int i = 0;
    int iArray[100];
    void* p;
    h_vector* vec = initVector(sizeof(int),10,0.6,0.25);
    for(;i<20;i++)
    {
        temp = rand();
        /* printf("the %d round,capacity: %ld,size: %ld\n",i,vectorCap(vec),vectorSize(vec)); */
        if(insertElemAt(vec,1,&temp)==HUBASE_OUTOFMEMORY) return -1;
        iArray[19-i] = temp;
    }
    temp = 1;
    for(i=0;i<20;i++)
    {
        getElem(vec,i+1,&p);
        if(iArray[i] == *((int*)p))
            temp &= 1;
        else
            temp &= 0;
        printf("%d: array :%d, vector: %d\n",i,iArray[i],*(int*)p);
    }
    printf("size: %ld, cap: %ld\n",vectorSize(vec),vectorCap(vec));
    printf("vector is %d\n",temp);
    
    printf("\nBelow are the romoval part:\n");
    
    for(i=0;i<15;i++)
    {
        if(removeElem(vec,1,&temp)==HUBASE_OUTOFMEMORY) 
            return -1;
    }
    arrayCopy(iArray,15*sizeof(int),iArray,0,5*sizeof(int),1);
    temp = 1;
    for(i=0;i<5;i++)
    {
        getElem(vec,i+1,&p);
        if(iArray[i] == *((int*)p))
            temp &= 1;
        else
            temp &= 0;
        printf("%d: array :%d, vector: %d\n",i,iArray[i],*(int*)p);
    }
    printf("size: %ld, cap: %ld\n",vectorSize(vec),vectorCap(vec));
    printf("vector is %d\n",temp);
    releaseVector(vec); 
    return 0;
}
