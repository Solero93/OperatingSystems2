#include "red-black-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TREEIO_HEADER
#define TREEIO_HEADER 1

#define MAXCHAR 100

RBTree * createTree(char * dictionary, char * configFile);
RBTree * readTree(char * filename);
#endif
