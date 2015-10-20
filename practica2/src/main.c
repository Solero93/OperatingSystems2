#include <stdio.h>
#include <string.h> 

#include "indexacio-local/linked-list.h"
#include "arbre-binari/red-black-tree.h"

#define MAXCHAR 100
#define SIZE    100


int main(int argc, char ** argv) {
  List ** hash_table = calloc(SIZE, sizeof(List *));
  RBTree * tree = malloc(sizeof(RBTree));
  FILE * fp, currentFile;
  char * filename = malloc(sizeof(char) * MAXCHAR);
  char * word;
  int numFiles;
  if (argc != 3) {
    printf("Usage: <dictionary> <cfg file>\n");
    exit(1);
  }
  initTree(tree);
  fp = fopen(argv[1], "r");
  crearArbreDiccionari(tree, fp);
  fclose(fp);
  // We now have the dictionary created, let's now start with the counting
  fp = fopen(argv[2], "r");
  fscanf(fp, "%d", &numFiles);
  for (int i = 0; i < numFiles; i++) {
    fscanf(fp, "%s", filename);
    currentFile = fopen(filename)
    hash_table
    while ((word = extreureParaules(currentFile)) != NULL) {
      insertarPalabraAlHash(hash_table, word);
    }
    insertarAlGlobal(tree, hash_table);
    clearTable(hash_table);
    fclose(currentFile);
  }
  free(hash_table);
  deleteTree(tree);
  free(tree);
}

void crearArbreDiccionari(RBTree * tree, FILE * fp) {
  char * buffer = (char*) malloc(sizeof(char) * MAXCHAR);
  while (fscanf(fp, "%s", buffer) != EOF) {
    char* tmpChar;
    tmpChar = (char*) malloc(sizeof(char) * MAXCHAR);
     
    strcpy(tmpChar, buffer);
     
    toLowercase(tmpChar);
     
    //Search if the key is in the tree
    treeData = findNode(tree, tmpChar);
     
    if (treeData == NULL) {
      //If the key is not in the tree, allocate memory for the data and insert in the tree
      treeData = malloc(sizeof(RBData));
      treeData->key = tmpChar;
      treeData->num = 1;
      
      insertNode(tree, treeData);
    }
  }
  free(buffer);
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

void insertarPalabraAlHash(List ** hash_table, char * palabra) {
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

void clearTable(List ** hash_table) {
  for(int i = 0; i < SIZE; i++) {
    if (hash_table[i] != NULL) {
      deleteList(hash_table[i]);
      free(hash_table[i]);
      hash_table[i] = NULL;
    }
  }
}

int hashWord(char* cadena) {
  len = strlen(cadena) - 1; 
  sum = 0;
  seed = 131;
  for(i = 0; i < len; i++)
    sum = sum * seed + (int)cadena[i];

  hash = sum % SIZE;

  return hash
}
