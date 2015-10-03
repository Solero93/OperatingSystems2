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
#include <string.h>

#include "red-black-tree.h"

//NOTE Defined maximum size of char vector
#define MAXCHAR 100

/**
 *
 *  Main function. Contains a simple example using a red-black-tree. 
 *
 */

int main(int argc, char **argv)
{
  char* buffer;
  RBTree *tree;
  RBData *treeData;
  FILE *fp;

  buffer = (char*) malloc(sizeof(char) * MAXCHAR);
  
  if (argc != 2)
  {
    printf("Usage: <filename> \n");
    exit(1);
  }
  
  fp = fopen(argv[1], "r");

  printf("Reading words and inserting them to black-red tree\n");

  /* Allocate memory for tree */
  tree = (RBTree *) malloc(sizeof(RBTree));

  /* Initialize the tree */
  initTree(tree);  
  
  while (fscanf(fp, "%s", buffer) != EOF) {
     char* tmpChar;
     tmpChar = (char*) malloc(sizeof(char) * MAXCHAR);
     
     strcpy(tmpChar, buffer);
      
     //Search if the key is in the tree
     treeData = findNode(tree, tmpChar);
     
     if (treeData != NULL) {
       //If the key is in the tree increment 'num'
       treeData->num++;
     } else {
       //If the key is not in the tree, allocate memory for the data and insert in the tree
       treeData = malloc(sizeof(RBData));
       treeData->key = tmpChar;
       treeData->num = 1;
       
       insertNode(tree, treeData);
     }
  }
  
  printf("Nombre total de nodes a l'arbre: %d\n", getNumNodes(tree));
  
  /* Delete the tree */
  deleteTree(tree);

  printf("Done.\n");
  
  fclose(fp); // Close file

  return 0;
}