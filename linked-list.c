/**
 *
 * Linked-list implementation.
 *
 * This is an implementation of a simple linked-list. A minimal
 * set of necessary functions have been included.
 *
 * Lluis Garrido, 2014.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * We include the linked-list.h header. Note the double
 * quotes.
 */

#include "linked-list.h"

/**
 *
 * Free data element. The user should adapt this function to their needs.  This
 * function is called internally by deleteList.
 *
 */

static void freeListData(ListData *data)
{
  // Key is a char*, so we must free it
  free(data->key);
  free(data);
}

/**
 *
 * Dumps data contents to stdout. To be used for debugging.
 *
 */

static void dumpListData(ListData *data)
{
  printf("Key %03d appears %d times\n", 69, data->numTimes);
}

/**
 *
 * Compares if key1 is equal to key2. Should return 1 (true) if condition
 * is satisfied, 0 (false) otherwise.
 *
 */

static int compEQ(TYPE_LIST_KEY key1, TYPE_LIST_KEY key2)
{
  int rc;

  rc = 0;

  if (strcmp(key1, key2) == 0)
    rc = 1;

  return rc;
}

/**
 *
 * Please do not change the code below unless you really know what you are
 * doing.
 *
 */

/**
 *
 * Initialize an empty list
 *
 */

void initList(List *l)
{
  l->numItems = 0;
  l->first = NULL;
}

/**
 *
 * Insert data in the list.  This function does not perform a copy of data
 * when inserting it in the list, it rather creates a list item and makes
 * this item point to the data. Thus, the contents of data should not be
 * overwritten after calling this function.
 *
 */

void insertList(List *l, ListData *data)
{
  ListItem *tmp, *x;

  x = malloc(sizeof(ListItem));

  if (x == 0) {
    printf("insufficient memory (insertItem)\n");
    exit(1);
  }

  /* Insert item at first position */

  tmp = l->first;
  l->first = x;
  x->next = tmp;

  /* Link data to inserted item */
  x->data = data;

  l->numItems++;
}

/**
 *
 * Find item containing the specified key. Returns the data
 * that it points to (not the item itself).
 *
 */

ListData *findList(List *l, TYPE_LIST_KEY key)
{
  ListItem *current;

  current = l->first;

  while (current != NULL)
  {
    if (compEQ(current->data->key, key))
      return (current->data);

    current = current->next;
  }

  return (NULL);
}

/**
 *
 * Deletes the first item of the list. The data to which
 * the deleted item points to also is deleted.
 *
 */

void deleteFirstList(List *l)
{
  ListItem *tmp;

  tmp = l->first;

  if (tmp)
  {
    l->first = tmp->next;
    freeListData(tmp->data);
    free(tmp);
    l->numItems--;
  }
}

/**
 *
 * Deletes a list including the data to which their
 * items point to.
 *
 */

void deleteList(List *l)
{
  ListItem *current, *next;

  current = l->first;

  while (current != NULL)
  {
    next = current->next;
    freeListData(current->data);
    free(current);
    current = next;
  }

  l->numItems = 0;
  l->first = NULL;
}

/**
 *
 * Dumps the contents of the list. Internally this function
 * called dumpListData which is user defined.
 *
 */

void dumpList(List *l)
{
  ListItem *current;

  current = l->first;

  while (current != NULL)
  {
    dumpListData(current->data);
    current = current->next;
  }

  printf("Total number of items: %d\n", l->numItems);
}

void saveList(FILE * fp, List * list) {
    fwrite(&list->numItems, sizeof(int), 1, fp);
    for (ListItem* item = list->first; item != NULL; item = item->next) {
        ListData* data = item->data;
        size_t filenameLen = strlen(data->key) + 1;// We need the NULL byte
        fwrite(&filenameLen, sizeof(size_t), 1, fp);
        fwrite(data->key, sizeof(char), filenameLen, fp);
        fwrite(&data->numTimes, sizeof(int), 1, fp);
    }
}

List * readList(FILE * fp) {
    int numElems;
    fread(&numElems, sizeof(int), 1, fp);
    List* list = malloc(sizeof(List));
    initList(list);
    for (int i = 0; i < numElems; i++) {
        // Extract data
        size_t filenameLen;
        fread(&filenameLen, sizeof(size_t), 1, fp);
        char * filename = malloc(sizeof(char) * filenameLen);
        fread(filename, sizeof(char), filenameLen, fp);
        int occurrences;
        fread(&occurrences, sizeof(int), 1, fp);
        // Process data
        ListData * item = malloc(sizeof(ListData));
        item->key = filename;
        item->numTimes = occurrences;
        insertList(list, item);
    }
    return list;
}
