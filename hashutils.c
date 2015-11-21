#include "hashutils.h"
#include "linked-list.h"
#include <string.h>
#include <stdlib.h>

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

List ** createHashTable(int size) {
    List ** table = malloc(sizeof(List *) * size);
    for (int i = 0; i < size; i++) {
        table[i] = malloc(sizeof(List));
        initList(table[i]);
    }
    return table;
}

/* Function to insert a word into the hash table */
void insertToHash(List ** hash_table, char * word) {
    int hash = hashWord(word);
    List * list;
    ListData * list_data;
    list = hash_table[hash];
    list_data = findList(list, word);
    if (list_data == NULL) {
      list_data = malloc(sizeof(ListData));
      list_data->key = word;
      list_data->numTimes = 1;
      insertList(list, list_data);
    } else {
      list_data->numTimes++;
      // Since we already have it we don't need to save it
      free(word);
    }
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
