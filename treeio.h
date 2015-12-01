#include "red-black-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifndef TREEIO_HEADER
#define TREEIO_HEADER 1

#define MAXCHAR 100
#define NUMTHREADS 4

struct thread_data {
    FILE *fp;
    RBTree *tree;
    int numFiles;
};

RBTree *createTree(char *dictionary, char *configFile);

RBTree *readTree(char *filename);

#endif
