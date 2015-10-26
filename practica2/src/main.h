#include <stdio.h>
#include <linked-list.h>
#include <red-black-tree.h>

void lowerWord(char * word);
void crearArbreDiccionari(RBTree * tree, FILE * fp);
void insertarAlGlobal(RBTree * tree, List ** hash_table);
int hashWord(char* cadena);
void insertarPalabraAlHash(List ** hash_table, char * palabra);
List ** createHashTable(int size);
void clearTable(List ** hash_table);
void deleteTable(List ** table);
