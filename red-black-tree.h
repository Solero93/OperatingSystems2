#include "linked-list.h"
/**
 *
 * Red-black tree header
 *
 *
 */

// Standard header collision detection
#ifndef RED_BLACK_TREE_HEADER
#define RED_BLACK_TREE_HEADER 1

#define TYPE_RBTREE_KEY char*

/**
 *
 * This structure holds the information to be stored at each node. Change this
 * structure according to your needs.  In order to make this library work, you
 * also need to adapt the functions compLT, compEQ and freeRBData. For the
 * current implementation the "key" member is used to index data within the
 * tree.
 *
 */

typedef struct RBData_
{
  // The variable used to index the tree has to be called "key".
  // The type may be any you want (float, char *, etc)
  TYPE_RBTREE_KEY key;

  // This is the additional information that will be stored
  // within the structure. You may adapt it to your needs:
  // add or remove elements as you need.
  int num;
  List* occurrences;
} RBData;

/**
 *
 * The node structure
 *
 */

typedef enum { BLACK, RED } nodeColor;

typedef struct Node_ {
    /* For internal use of the structure. Do not change. */
    struct Node_ *left;         /* left child */
    struct Node_ *right;        /* right child */
    struct Node_ *parent;       /* parent */
    nodeColor color;            /* node color (BLACK, RED) */

    /* Data to be stored at each node */
    RBData *data;                    /* data stored in node */
} Node;

/**
 *
 * The tree structure. It just contains the root node, from
 * which we may go through all the nodes of the binary tree.
 *
 */

typedef struct RBTree_ {
  Node *root;                   /* root of Red-Black tree */
  int numNodes;
  int scannedFiles;
} RBTree;

/*
 * Function headers. Note that not all the functions of
 * red-black-tree.c have been included here.
 */

void initTree(RBTree *tree);
int getNumNodes(RBTree *tree);
void insertNode(RBTree *tree, RBData *data);
RBData *findNode(RBTree *tree, TYPE_RBTREE_KEY key);
void deleteTree(RBTree *tree);
void saveTree(char * filename, RBTree * tree);
#endif
