/**
 *  btree.c 
 *  implementations of the frequent used btree structure and 
 *  HuBase's file format.
 *
 *  Author: lwz
 *  Since : 2017-02-23
 **/

#include "./btree.h"
#include "./vector.h"
#include "./list.h"
#include "./malloc.h"
#include <assert.h>

#ifdef HUBASE_DEBUG
#include <stdio.h>
#endif

/**
 * HuBase will use B*-Tree to organize data in a file.
 * This kind of BTree stores no data but keys in the internal
 * nodes and stores keys and data(page number aka "page pointer"
 * in the leaf nodes.
 **/

typedef struct nodeCell nodeCell;
typedef struct bTreeNode bTreeNode;
typedef struct cellList cellList;
/**
 * In each node there is a list of "cell"
 * and each "cell" contains a key and a "pointer"(actually page number)
 * to the child less than the key.
 * 
 * There are two kinds of tree node: leaf and internal.
 * The leaf node's pChild in the cell are null(no child), the 
 * page number for the keys are in the vector pageVector.
 * Also, the nodes in the leaf level are linked using the 
 * rBrother pointer. And the rmChild(stands for rightmostChild) 
 * is set null.
 * 
 * The internal nodes' rBrother and pageVector are set null.
 * But the pChild and the rmChild are set.
 **/
struct bTreeNode {
    u16 height;             /* height of the current tree node, leaves' heights are 0 */
    u8 isLeaf;              /* boolean for leaf */
    bTreeNode* parent;      /* pointer to the parent */
    bTreeNode* rBrother;    /* pointer to the right brother */
    bTreeNode* rmChild;     /* right most child */
    h_vector* pageVector;   /* put child in the vector */
    cellList* cells;        /* a list of nodeCell */
};

struct bTree {
    u32 nodeNum;                    /* the number of nodes, could get from header */
    bTreeNode* root;                /* the root of the tree */
    u16 order;                      /* the order of the btree */
    u32 keyNum;                     /* the number of keys in the whole tree */
    int(*comparator)(key*, key*);   /* the comparator of the btree. use the default comparator
                                       if not explicitly set */
};

struct key {                /* the struct represent the different kinds of keys */
    void* buff;             /* the buff for keys' storage */
    h_vector* pTypes;    /* use an array of int to represent the type of keys */
};

/* this is the subclass of h_listNode */
struct nodeCell {
    nodeCell* pPrev;
    nodeCell* pNext;
    key* pKey;
    bTreeNode* pChild;
};

/**
 * This is the subclass of h_list.
 * It has the same structure of h_list but different
 * methods.
 **/
struct cellList {
    nodeCell* pHeader;
    nodeCell* pTrailer;
    u64 keyNum;
};

/**
 * Init a key object with given buff.
 * Caller should provide the length of buff.
 * This method will fill the buff with arrayCopy routine.
 * 
 * Also, the pTypes vector will be init with element size 
 * of sizeof(int). But caller should insert types information 
 * himself.
 **/
int initKey(key** ppKey,void* buff,u32 length)
{
    key* temp;
    void* tmpBuff;
    assert(ppKey&&buff);
    if((huBaseMalloc(length,&tmpBuff)==HUBASE_OUTOFMEMORY)|| \
            (huBaseMalloc(sizeof(key),(void**)&temp)==HUBASE_OUTOFMEMORY))
    {
        return HUBASE_OUTOFMEMORY;
    }
    arrayCopy(buff,0,tmpBuff,0,length,1);
    temp->pTypes = (h_vector*)initVector(sizeof(int),5,0.75,0.25);
    if(!(temp->pTypes)) return HUBASE_OUTOFMEMORY;
    temp->buff = tmpBuff;
    (*ppKey) = temp;
    return HUBASE_OK;
}

/* Fast route for keys in the node */
/* assure node not null before call */
inline static u64 keysInNode(bTreeNode* pNode)
{
    return pNode->cells->keyNum;
}

u32 keysInTheTree(bTree* pTree)
{
    assert(pTree);
    return pTree->keyNum;
}

u32 nodesInTheTree(bTree* pTree)
{
    assert(pTree);
    return pTree->nodeNum;
}

u16 heightOfTree(bTree* pTree)
{
    assert(pTree);
    return pTree->root->height;
}

static int isRoot(bTreeNode* node)
{
    assert(node);
    if(node->parent) return 0;
    else return 1;
}

/* Initialize nodeCell with given key and child */
static int initNodeCell(nodeCell** ppCell,key* pKey,bTreeNode* child) 
{
    if(huBaseMalloc(sizeof(nodeCell),(void**)ppCell)==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    (*ppCell)->pKey = pKey;
    (*ppCell)->pChild = child;
    return HUBASE_OK;
}

/**
 * Insert the key at given TreeNode and given rank
 **/
static int insertKeyAtNodeNRank(key* pKey,bTreeNode* treeNode,u64 rank)
{
    nodeCell* pCell;
    assert(pKey&&treeNode);
    if(initNodeCell(&pCell,pKey,0)==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    insertNodeAt((h_list*)treeNode->cells,rank,(h_listNode*)pCell);
    return HUBASE_OK;
}

/**
 * Init a new node using the isLeaf flag.
 * If it is a leaf, then the node is initialized with an empty 
 * list of cell, and a empty pageVector.
 * 
 * If it is an internal node, the node is initialized with
 * an empty list of cells, and the pageVector is set null.
 **/
static int newTreeNode(bTreeNode** ppBTreeNode,int isLeaf,u16 height,
                        bTreeNode* parent,bTreeNode* rBrother,bTreeNode* rmChild)
{
    assert(ppBTreeNode);
    if(huBaseMalloc(sizeof(bTreeNode),(void**)ppBTreeNode)==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    (*ppBTreeNode)->height = height;
    (*ppBTreeNode)->parent = parent;
    (*ppBTreeNode)->rmChild = rmChild;
    (*ppBTreeNode)->isLeaf = isLeaf;
    (*ppBTreeNode)->rBrother = rBrother;
    if(initList((h_list**)&((*ppBTreeNode)->cells),sizeof(cellList),sizeof(nodeCell))==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    if(isLeaf)
    {
        if(((*ppBTreeNode)->pageVector = (h_vector*)initVector(sizeof(u32),5,0.75,0.25)))
            return HUBASE_OK;
        else
            return HUBASE_OUTOFMEMORY;
    }
    return HUBASE_OK;  /* don't init pageVector for internal nodes. */
}

/**
 * This is method is to init the btree. To set the 
 * order and the comparator for the btree.
 *
 * The "comparator" is the pointer of the function 
 * which in charge of comparing to keys.
 * If A < B, return -1. If B == A, return 0.
 * If B > A, return 1.
 **/
int bTreeInit(int order, int(*comp)(key* keyA, key* keyB), bTree** ppBTree)
{
    assert(ppBTree);
    if(huBaseMalloc(sizeof(bTree),(void**)ppBTree)==HUBASE_OUTOFMEMORY)
        return HUBASE_OUTOFMEMORY;
    (*ppBTree)->order = order;
    (*ppBTree)->nodeNum = 1;
    (*ppBTree)->comparator = comp;
    (*ppBTree)->keyNum = 0;
    if(newTreeNode(&(*ppBTree)->root,1,0,0,0,0)==HUBASE_OUTOFMEMORY)/* init an empty root */
        return HUBASE_OUTOFMEMORY;
    return HUBASE_OK;
}

#ifdef HUBASE_DEBUG
/** 
 * Use methods below to test the list in the btree 
 * (and test the correctness of doubly list)
 * 
 * The method will init a btree and test the list in the btree's root.
 * This method will use int as the key(thus the cellList can be 
 * regarded as a linklist containing integers. 
 * 
 * And after massive insert/remove, to check the lists' correctness.
 **/
void testListInsert(bTree* pTree,int* pInt,int num)
{
    key* pKey;
    int i;
    for(i=0;i<num;i++)
    {
        initKey(&pKey,(void*)(pInt+i),sizeof(int));
        insertKeyAtNodeNRank(pKey,pTree->root,i+1);
    }
}

void testListRemove(bTree* pTree,int num)
{
    int i;
    h_listNode* node;
    for(i=0;i<num;i++)
    {
        removeNodeAt((h_list*)pTree->root->cells,1,(h_listNode**)&node);
    }
}

void displayList(bTree* testTree)
{
    nodeCell* temp;
    int i=0;
    for(temp=testTree->root->cells->pHeader->pNext;temp!=testTree->root->cells->pTrailer;temp=temp->pNext)
    {
        i++;
        printf("key %d is: %d\n",i,*((int*)temp->pKey->buff));
    }
    printf("%d\n",i);
}
#endif
