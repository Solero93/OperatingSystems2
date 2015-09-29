/**
 *
 * Main file 
 * 
 * This file calls the red-black-tree.c functions.
 *
 * Lluis Garrido, June 2015.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "red-black-tree.h"

/**
 *
 *  Main function. Contains a simple example using a red-black-tree. 
 *
 */

int main(int argc, char **argv)
{
  int a, maxnum, ct;

  RBTree *tree;
  RBData *treeData;

  if (argc != 2)
  {
    printf("Usage: %s maxnum\n", argv[0]);
    exit(1);
  }

  maxnum = atoi(argv[1]);

  printf("Test with red-black-tree\n");

  /* Random seed, same as before just to ensure that the same values are generated */
  srand(0);

  /* Allocate memory for tree */
  tree = (RBTree *) malloc(sizeof(RBTree));

  /* Initialize the tree */
  initTree(tree);

  for (ct = 0; ct < maxnum; ct++) {
    /* Generate random key to be inserted in the tree */
    a = rand() % 100 + 1;

    /* Search if the key is in the tree */
    treeData = findNode(tree, a); 

    if (treeData != NULL) {

      /* If the key is in the tree increment 'num' */
      treeData->num++;
    } else {

      /* If the key is not in the tree, allocate memory for the data
       * and insert in the tree */

      treeData = malloc(sizeof(RBData));
      treeData->key = a;
      treeData->num = 1;

      insertNode(tree, treeData); //TODO pass to lowercase before inserting (tolower)
    }
  }

  printf("Nombre total de nodes a l'arbre: %d\n", getNumNodes(tree));
  for(a = 0; a < 100; a++)
  {
    treeData = findNode(tree, a);

    if (treeData) 
      printf("El numero %d apareix %d cops a l'arbre.\n", a, treeData->num);
  }
  
  /* Delete the tree */
  deleteTree(tree);

  printf("Done.\n");

  return 0;
}

