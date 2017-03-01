/**
 *  btree.c 
 *  implementations of the frequent used btree structure and 
 *  HuBase's file format.
 *
 *  Author: lwz
 *  Since : 2017-02-23
 **/

#include "./btree.h"

/**
 * For easier implementation, the order of the btree is set 8,
 * which is efficient enough for most tables.
 **/
struct bTreeNode {
    u16 height;             /* height of the current tree node, leaves' heights are 0 */
    u8 isLeaf;              /* boolean for leaf */
    u16 keyNum;             /* number of keys in this node */
    bTreeNode* parent;      /* pointer to the parent */
    bTreeNode* rBrother;    /* pointer to the right brother */
    h_vector keyVector;     /* put keys in the vector */
    h_vector childVector;   /* put child in the vector */
};

struct bTree {
    u16 nodeNum;                    /* the number of nodes, could get from header */
    bTreeNode* root;                /* the root of the tree */
    u16 depth;                      /* the depth of the tree, could get from header */
    u16 order;                      /* the order of the btree */
    int(*comparator)(key*, key*);  /* the comparator of the btree. use the default comparator
                                       if not explicitly set */
};

struct key {                /* the struct represent the different kinds of keys */
    void* buff;             /* the buff for keys' storage */
    h_vector key_types;     /* use an array of int to represent the type of keys */
};

/**
 * This is method is to init the btree. To set the 
 * order and the comparator for the btree.
 *
 * The "comparator" is the pointer of the function 
 * which in charge of comparing to keys.
 * If A < B, return -1. If B == A, return 0.
 * If B > A, return 1.
 **/
int bTreeInit(int order, int(*comp)(key* keyA, key* keyB))
{

}
