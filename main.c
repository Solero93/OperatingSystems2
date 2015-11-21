#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "linked-list.h"
#include "red-black-tree.h"
#include "extract-words.h"
#include "main.h"

#define MAXCHAR 100
#define SIZE    100

int main(int argc, char ** argv) {
  List ** hash_table = createHashTable(SIZE);
  RBTree * tree = malloc(sizeof(RBTree));
  FILE * configFile;
  FILE * currentFile;
  char * filename = malloc(sizeof(char) * MAXCHAR);
  char * word;
  int numFiles;
  if (argc != 3) {
    printf("Usage: <dictionary> <cfg file>\n");
    exit(1);
  }
  // We start creating our dictionary tree
  initTree(tree);
  configFile = fopen(argv[1], "r");
  crearArbreDiccionari(tree, configFile);
  fclose(configFile);
  // We now have the dictionary created, let's now start with the word counting
  configFile = fopen(argv[2], "r");
  // First line of the .cfg file is the number of files
  fscanf(configFile, "%d", &numFiles);
  for (int i = 0; i < numFiles; i++) {
    // One file path per line
    fscanf(configFile, "%s", filename);
    printf("reading file: %s\n", filename);
    currentFile = fopen(filename, "r");
    // We read words until we're done with the file
    while ((word = extractWord(currentFile)) != NULL) {
      insertarPalabraAlHash(hash_table, word);
    }
    // And now we just enter the numbers into the tree and reset the table
    insertarAlGlobal(tree, hash_table, filename);
    clearTable(hash_table);
    fclose(currentFile);
  }
  // We could very well not do this as when we finish the kernel will cleanup
  // after us and we're not doing anything else with our data
  free(filename);
  deleteTable(hash_table);
  fclose(configFile);
  free(hash_table);
  deleteTree(tree);
  free(tree);
}

/*
 Function that lowercases a word in-place
 */
void lowercaseWord(char * word) {
  for (int i = 0; word[i] != '\0'; i++) {
    word[i] = tolower(word[i]);
  }
}

/*
 Given a file and an empty tree we insert each word of the file into the tree
 */
void crearArbreDiccionari(RBTree * tree, FILE * fp) {
  RBData * treeData;
  char * buffer = (char*) malloc(sizeof(char) * MAXCHAR);
  char * tmpChar;
  while (fscanf(fp, "%s", buffer) != EOF) {
    tmpChar = (char*) malloc(sizeof(char) * MAXCHAR);

    strcpy(tmpChar, buffer);
    lowercaseWord(tmpChar);

    //Search if the key is in the tree
    treeData = findNode(tree, tmpChar);

    if (treeData == NULL) {
      //If the key is not in the tree, allocate memory for the data and insert in the tree
      treeData = malloc(sizeof(RBData));
      treeData->key = tmpChar;
      treeData->num = 1;
      treeData->occurrences = malloc(sizeof(List));

      insertNode(tree, treeData);
    }

  }
  // Let's not forget we used a buffer :D
  free(buffer);
}

void insertarAlGlobal(RBTree * tree, List ** hash_table, char * filename) {
  List * list;
  ListItem * current;
  RBData * treeData;
  int differentWords = 0;
  // We now scan through the whole hash_table and insert all words
  for (int i = 0; i < SIZE; i++) {
    list = hash_table[i];
    current = list->first;
    while (current != NULL) {
      treeData = findNode(tree, current->data->key);
      if (treeData != NULL) { // If it's null it's a word not in the dictionary, so we avoid it
        treeData->num += current->data->numTimes;
        ListData * occurrences = malloc(sizeof(ListData));
        occurrences->key = filename;
        occurrences->numTimes = current->data->numTimes;
        differentWords++;
      }
      current = current->next;
    }
  }
  printf("File has %d different words\n", differentWords);
}

/* Evil string hash magic */
int hashWord(char* cadena) {
  int len = strlen(cadena) - 1;
  int sum = 0;
  int seed = 131;
  for(int i = 0; i < len; i++)
    sum = sum * seed + (int)cadena[i];

  int hash = sum % SIZE;
  if (hash < 0) // The hash might be negative, so we offset it to a positive number
    return SIZE + hash;

  return hash;
}

/* Function to insert a word into the hash table */
void insertarPalabraAlHash(List ** hash_table, char * palabra) {
  int hash = hashWord(palabra);
  List * list;
  ListData * list_data;
  list = hash_table[hash];
  list_data = findList(list, palabra);
  if (list_data == NULL) {
    list_data = malloc(sizeof(ListData));
    list_data->key = palabra;
    list_data->numTimes = 1;
    insertList(list, list_data);
  } else {
    list_data->numTimes++;
    // Since we already have it we don't need to save it
    free(palabra);
  }
}

/* Name says it all for these functions */

List ** createHashTable(int size) {
    List ** table = malloc(sizeof(List *) * size);
    for (int i = 0; i < size; i++) {
        table[i] = malloc(sizeof(List));
        initList(table[i]);
    }
    return table;
}

void clearTable(List ** hash_table) {
  for(int i = 0; i < SIZE; i++) {
    if (hash_table[i]->first != NULL) {
      deleteList(hash_table[i]);
    }
  }
}

void deleteTable(List ** table) {
    for (int i = 0; i < SIZE; i++) {
        free(table[i]);
    }
}
