#include <stdio.h>
#include <string.h> 

#include "indexacio-local/linked-list.h"
#include "arbre-binari/red-black-tree.h"

#define MAXCHAR 100
#define SIZE    100


int main(int argc, char ** argv) {
  List ** hash_table = calloc(SIZE, sizeof(List *));
  RBTree * tree = malloc(sizeof(RBTree));
  initTree(tree);
  insertDictionary(tree);
  char * buffer = malloc(sizeof(char) * MAXCHAR);
  for (file : files) {
    while (fscanf(fp, "%s", buffer) != EOF) {
      char ** paraules = extreureParaules(buffer);
      insertarParaules(hash_table, paraules, len);
    }
    insertarAlGlobal(arbre, hash_table);
  }
  free(hash_table);
}

void insertarAlGlobal(RBTree * tree, List ** hash_table) {
  List * list;
  ListItem * current;
  RBData * treeData;
  for (int i = 0; i < SIZE; i++) {
    list = hash_table[i];
    if (list != NULL) {
      current = list->first;
      while (current != NULL) {
	treeData = findNode(tree, current->data->key);
	treeData->num += current->data->numTimes;
	current = current->next;
      }
    }
  }
}

void clearTable(List ** table) {
  for(int i = 0; i < SIZE; i++) {
    if (table[i] != NULL) {
      deleteList(table[i]);
      free(table[i]);
      table[i] = NULL;
    }
  }
}

int hashWord(char* cadena)
{
  len = strlen(cadena) - 1; 
  sum = 0;
  seed = 131;
  for(i = 0; i < len; i++)
    sum = sum * seed + (int)cadena[i];

  hash = sum % SIZE;

  return hash
}
