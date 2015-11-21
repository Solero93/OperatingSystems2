#include "red-black-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeio.h"
#include "linked-list.h"

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

void saveTree(char * filename, RBTree * tree) {
    FILE* fp = fopen(filename, "w");
    if (tree->root != NIL) {
        saveTreeRecursively(fp, tree->root);
    }
    fclose(fp);
}

void saveTreeRecursively(FILE * fp, Node * node) {
    if (node->right != NIL) {
        saveTreeRecursively(fp, node->right);
    }
    if (node->left != NIL) {
        saveTreeRecursively(fp, node->left);
    }
    saveTreeData(fp, node->data);
}

void saveTreeData(FILE * fp, RBData * data) {
    int len = strlen(data->key);
    fwrite(&len, sizeof(int), 1, fp);
    fwrite(data->key, sizeof(char), len, fp);
    fwrite(&data->num, sizeof(int), 1, fp);
    // Save the list
    List * list = data->occurrences;
    saveList(fp, list);
}
