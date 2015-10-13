#include <stdio.h>
#include <string.h> 

#include "indexacio-local/linked-list.h"

#define MAXCHAR 100
#define SIZE    100


int main(int argc, char ** argv) {
  List ** hash_table = calloc(SIZE, sizeof(List *));
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
