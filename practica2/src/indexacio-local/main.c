/**
 *
 * Main file 
 * 
 * This file calls the linked-list.c function files.
 *
 * Lluis Garrido, 2014.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "linked-list.h"

/**
 *
 *  Main function. Contains a simple example using both
 *  linked-list and red-black-tree.
 *
 */

int main(int argc, char **argv)
{
  int a, maxnum, ct;

  List list;
  ListData *listData;

  if (argc != 2)
  {
    printf("Usage: %s maxnum\n", argv[0]);
    exit(1);
  }

  maxnum = atoi(argv[1]);

  printf("Test with linked-list\n");

  /* Random seed */
  srand(0);

  /* Initialize the list */
  initList(&list);

  for(ct = maxnum; ct; ct--) {
    /* Generate random key to be inserted in the tree */
    a = rand() % 100 + 1;

    /* Search if the key is in the tree */
    listData = findList(&list, a); 

    if (listData != NULL) {

      /* We increment the number of times current item has appeared */
      listData->numTimes++;
    } else {

      /* If the key is not in the list, allocate memory for the data and
       * insert it in the list */

      listData = malloc(sizeof(ListData));
      listData->key = a;
      listData->numTimes = 1;

      insertList(&list, listData);
    }
  }

  printf("Printing contents of list:\n");
  /* Dump contents to stdout */
  dumpList(&list);

  /* Delete the list */
  deleteList(&list);

  printf("Done.\n");

  return 0;
}

void insertarPalabras(List ** hash_table, char ** palabras, int len) {
  char * palabra;
  for (int i = 0; i < len; i++) {
    insertarPalabra(hash_table, palabras[i]);
  }
}

void insertarPalabra(List ** hash_table, char * palabra) {
  int hash = hashWord(palabra);
  List * list;
  ListData * list_data;
  list = hash_table[hash];
  if (list == NULL) {
    list = malloc(sizeof(List))
    initList(list);
  }
  list_data = findList(list, palabra);
  if (list_data == NULL) {
    list_data = malloc(sizeof(ListData));
    list_data->key = palabra;
    list_data->numTimes++;
    insertList(list, list_data);
  } else {
    list_data->numTimes++;
  }
}
