/**
 *
 * Linked-list header
 *
 * Include this file in order to be able to call the
 * functions available in linked-list.c. We include
 * here only those information we want to make visible
 * to other files.
 *
 * Lluis Garrido, 2015.
 *
 */

/**
 *
 * The TYPE_LIST_KEY is used to define the type of the primary
 * key used to index data in the list.
 *
 */

#define TYPE_LIST_KEY char*

/**
 *
 * This structure holds the information to be stored at each list item.  Change
 * this structure according to your needs.  In order to make this library work,
 * you also need to adapt the functions compEQ and freeListData. For the
 * current implementation the "key" member is used search within the list.
 *
 */

typedef struct ListData_ {
  // The variable used to index the list has to be called "key".
  TYPE_LIST_KEY key;

  // This is the additional information that will be stored
  // within the structure. This additional information is associated
  // to the key.
  int numTimes;
} ListData;


/**
 *
 * The item structure
 *
 */

typedef struct ListItem_ {
  ListData *data;
  struct ListItem_ *next;
} ListItem;

/**
 *
 * The list structure
 *
 */

typedef struct List_ {
  int numItems;
  ListItem *first;
} List;

/**
 *
 * Function heders we want to make visible so that they
 * can be called from any other file.
 *
 */

void initList(List *l);
void insertList(List *l, ListData *data);
ListData *findList(List *l, TYPE_LIST_KEY key);
void deleteFirstList(List *l);
void deleteList(List *l);
void dumpList(List *l);
