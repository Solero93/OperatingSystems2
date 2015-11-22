#include "red-black-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeio.h"
#include "linked-list.h"
#include "strutils.h"
#include "extract-words.h"
#include "hashutils.h"

RBTree * readTree(char * filename) {
    FILE* fp = fopen(filename, "r");
    RBTree* tree = malloc(sizeof(RBTree));
    initTree(tree);
    char* word;
    int wordLength, wordCount;
    while (feof(fp) == 0) {
        /* Read the data
           The data is formatted as follows: strlen, string, number of times it appears
         */
        fread(&wordLength, sizeof(int), 1, fp);
        word = malloc(sizeof(char) * wordLength);
        fread(word, sizeof(char), wordLength, fp);
        fread(&wordCount, sizeof(int), 1, fp);
        // Process the data
        RBData* data = malloc(sizeof(RBData));
        data->key = word;
        data->num = wordCount;
        data->occurrences = readList(fp);
        insertNode(tree, data);
    }
    fclose(fp);
    return tree;
}

static void saveTreeData(FILE * fp, RBData * data) {
    int len = strlen(data->key);
    fwrite(&len, sizeof(int), 1, fp);
    fwrite(data->key, sizeof(char), len, fp);
    fwrite(&data->num, sizeof(int), 1, fp);
    // Save the list
    List * list = data->occurrences;
    saveList(fp, list);
}

static void saveTreeRecursively(FILE * fp, Node * node) {
    if (node->right != NIL) {
        saveTreeRecursively(fp, node->right);
    }
    if (node->left != NIL) {
        saveTreeRecursively(fp, node->left);
    }
    saveTreeData(fp, node->data);
}

void saveTree(char * filename, RBTree * tree) {
    FILE* fp = fopen(filename, "w");
    if (tree->root != NIL) {
        saveTreeRecursively(fp, tree->root);
    }
    fclose(fp);
}

/*
 Given a file and an empty tree we insert each word of the file into the tree
 */
static void createDictionaryTree(RBTree * tree, FILE * dictionary) {
  RBData * treeData;
  char * buffer = (char*) malloc(sizeof(char) * MAXCHAR);
  char * tmpChar;
  while (fscanf(dictionary, "%s", buffer) != EOF) {
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

static void insertHashtableToTree(RBTree * tree, List ** hash_table, char * filename) {
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
          insertList(treeData->occurrences, occurrences);
          differentWords++;
        }
        current = current->next;
      }
    }
    printf("File has %d different words\n", differentWords);
}

RBTree * createTree(char * dictionary, char * configFile) {
    List ** hash_table = createHashTable(SIZE);
    RBTree * tree = malloc(sizeof(RBTree));
    FILE * fp;
    FILE * currentFile;
    char * filename = malloc(sizeof(char) * MAXCHAR);
    char * word;
    int numFiles;
    // We start creating our dictionary tree
    initTree(tree);
    fp = fopen(dictionary, "r");
    createDictionaryTree(tree, fp);
    fclose(fp);
    // We now have the dictionary created, let's now start with the word counting
    fp = fopen(configFile, "r");
    // First line of the .cfg file is the number of files
    fscanf(fp, "%d", &numFiles);
    for (int i = 0; i < numFiles; i++) {
        // One file path per line
        fscanf(fp, "%s", filename);
        printf("reading file: %s\n", filename);
        currentFile = fopen(filename, "r");
        // We read words until we're done with the file
        while ((word = extractWord(currentFile)) != NULL) {
            insertToHash(hash_table, word);
        }
        // And now we just enter the numbers into the tree and reset the table
        insertHashtableToTree(tree, hash_table, filename);
        clearTable(hash_table);
        fclose(currentFile);
    }
    // We could very well not do this as when we finish the kernel will cleanup
    // after us and we're not doing anything else with our data
    free(filename);
    deleteTable(hash_table);
    fclose(fp);
    free(hash_table);
    return tree;
}
