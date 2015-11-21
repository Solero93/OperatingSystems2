#include "red-black-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TREEIO_HEADER
#define TREEIO_HEADER 1

#define NIL &sentinel           /* all leafs are sentinels */
static Node sentinel = { NIL, NIL, 0, BLACK, NULL};

RBTree * readTree(char * filename);

void saveTree(char * filename, RBTree * tree);

void saveTreeRecursively(FILE * fp, Node * node);

void saveTreeData(FILE * fp, RBData * data);
#endif
