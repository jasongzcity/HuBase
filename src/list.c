/**
 * list.c 
 *
 * Baseclass of doubly list.
 * Author: lwz
 * Since: 2017-03-03
 **/

#include "./list.h"
#include <assert.h>
#include "./malloc.h"

/**
 * Note: 
 * Any subclasses of this doubly list must
 * have conformed structure like the h_list 
 * and h_listNode.
 **/
struct h_list {
    h_listNode* pHeader;
    h_listNode* pTrailer;
    u64 size;
};

struct h_listNode {
    h_listNode* pPrev;
    h_listNode* pNext;
};

/**
 * Init a doubly list.
 *
 * Note: the subclasses should implement their own init methods.
 **/
int initList(h_list** ppList,u32 listSize,u32 nodeSize)
{
    void* temp;
    assert(ppList);
    if(huBaseMalloc(listSize,(void*)ppList)==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    if(huBaseMalloc(nodeSize,&temp)==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    (*ppList)->pHeader = (h_listNode*)temp; 
    if(huBaseMalloc(nodeSize,&temp)==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    (*ppList)->pTrailer = (h_listNode*)temp; 
    (*ppList)->size = 0;
    (*ppList)->pHeader->pPrev = (*ppList)->pTrailer->pNext = 0;
    (*ppList)->pHeader->pNext = (*ppList)->pTrailer;
    (*ppList)->pTrailer->pPrev = (*ppList)->pHeader;
    return HUBASE_OK;
}

int releaseList(h_list* list)
{
    h_listNode* backup;
    h_listNode* current;
    assert(list);
    current = list->pHeader->pNext;
    while(current!=list->pTrailer)
    {
        backup = current;
        current = current->pNext;
        huBaseFree((void*)backup);
    }
    return HUBASE_OK;
}

h_listNode* firstNode(h_list* list)
{
    assert(list&&list->size); /* make sure > 0 */
    return list->pHeader->pNext;
}

h_listNode* lastNode(h_list* list)
{
    assert(list&&list->size);
    return list->pTrailer->pPrev;
}

h_listNode* nodeAt(h_list* list, u64 rank)
{
    h_listNode* temp;
    assert(list&&list->size);
    assert(rank<=list->size);
    temp = list->pHeader;
    while(rank)
    {
        temp = temp->pNext;
        rank--;
    }
    return temp;
}

u64 listSize(h_list* list)
{
    assert(list);
    return list->size;
}
