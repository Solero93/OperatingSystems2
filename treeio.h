#include "red-black-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifndef TREEIO_HEADER
#define TREEIO_HEADER 1

#define MAXCHAR 100
#define NUMTHREADS 4
#define BUFFERSIZE NUMTHREADS

RBTree *createTree(char *dictionary, char *configFile);

RBTree *readTree(char *filename);

#endif
