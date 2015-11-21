#include "linked-list.h"

#ifndef HASHUTILS_HEADER
#define HASHUTILS_HEADER 1

#define SIZE    100

int hashWord(char* cadena);
List ** createHashTable(int size);
void insertToHash(List ** hash_table, char * word);
void clearTable(List ** hash_table);
void deleteTable(List ** table);
#endif
