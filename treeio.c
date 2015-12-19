#include "red-black-tree.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "treeio.h"
#include "strutils.h"
#include "extract-words.h"
#include "hashutils.h"

static pthread_mutex_t mutexConfigFile, mutexHashTable;
static pthread_cond_t prodQueue, consQueue;
static List ***hashBuffer;
static int bufferWriteIndex, numElem, numFilesLeft;
static bool PRODUCERS_NOT_DONE;

RBTree *readTree(char *filename) {
    FILE *fp = fopen(filename, "r");
    RBTree *tree = malloc(sizeof(RBTree));
    initTree(tree);
    // First 4 bytes (int) are the number of scanned files
    fread(&tree->scannedFiles, sizeof(int), 1, fp);
    char *word;
    int wordCount;
    size_t wordLength;
    while (feof(fp) == 0) {
        /* Read the data
           The data is formatted as follows: strlen, string, number of times it appears
           and then it's the list of how many times it appears in every file
         */
        fread(&wordLength, sizeof(size_t), 1, fp);
        word = calloc(sizeof(char), wordLength);
        fread(word, sizeof(char), wordLength, fp);
        fread(&wordCount, sizeof(int), 1, fp);
        // Process the data
        RBData *data = malloc(sizeof(RBData));
        data->key = word;
        data->num = wordCount;
        data->occurrences = readList(fp);
        insertNode(tree, data);
    }
    fclose(fp);
    return tree;
}

/*
 Given a file and an empty tree we insert each word of the file into the tree
 */
static void createDictionaryTree(RBTree *tree, FILE *dictionary) {
    RBData *treeData;
    char *buffer = (char *) malloc(sizeof(char) * MAXCHAR);
    char *tmpChar;
    while (fscanf(dictionary, "%s", buffer) != EOF) {
        tmpChar = (char *) malloc(sizeof(char) * MAXCHAR);
        strcpy(tmpChar, buffer);
        lowercaseWord(tmpChar);
        //Search if the key is in the tree
        treeData = findNode(tree, tmpChar);
        if (treeData == NULL) {
            //If the key is not in the tree, allocate memory for the data and insert in the tree
            treeData = malloc(sizeof(RBData));
            treeData->key = tmpChar;
            treeData->num = 0;
            treeData->occurrences = malloc(sizeof(List));
            initList(treeData->occurrences);
            insertNode(tree, treeData);
        }
    }
    // Let's not forget we used a buffer :D
    free(buffer);
}

static void insertHashtableToTree(RBTree *tree, List **hash_table) {
    List *list;
    ListItem *current;
    RBData *treeData;
    int differentWords = 0;
    // We now scan through the whole hash_table and insert all words
    for (int i = 0; i < SIZE; i++) {
        list = hash_table[i];
        current = list->first;
        while (current != NULL) {
            treeData = findNode(tree, current->data->key);
            if (treeData != NULL) { // If it's null it's a word not in the dictionary, so we avoid it
                treeData->num += current->data->numTimes;
                ListData *occurrences = malloc(sizeof(ListData));
                occurrences->key = calloc(1, sizeof(char));
                occurrences->numTimes = current->data->numTimes;
                insertList(treeData->occurrences, occurrences);
                differentWords++;
            }
            current = current->next;
        }
    }
}

void insertToBuffer(List **hash_table) {
    pthread_mutex_lock(&mutexHashTable);
    while (numElem == BUFFERSIZE) { // Buffer full -> wait
        pthread_cond_wait(&prodQueue, &mutexHashTable);
    }
    hashBuffer[bufferWriteIndex] = hash_table; // Write to buffer
    bufferWriteIndex = (bufferWriteIndex + 1) % BUFFERSIZE; // Determine next index to write
    numElem++;
    if (numElem == 1) { // If buffer was empty -> signal consume
        pthread_cond_signal(&consQueue);
    }
    pthread_mutex_unlock(&mutexHashTable);
}

void *processFile(void *threadArg) {
    FILE *currentFile;
    FILE *fp = (FILE *) threadArg;
    char *word;
    char *filename = malloc(sizeof(char) * MAXCHAR);
    while (numFilesLeft > 0) {
        // We create a hash_table for the current file
        List **hash_table = createHashTable(SIZE);
        // Note, this is malloc'd here due to the filename being used in the list of occurrences
        /*
         * One file path per line. Plus we also have to lock to avoid race conditions
         */
        pthread_mutex_lock(&mutexConfigFile);
        // This is just if another thread finished the remaining files before freeing the lock
        if (numFilesLeft == 0) {
            pthread_mutex_unlock(&mutexConfigFile);
            deleteTable(hash_table);
            break;
        }
        fscanf(fp, "%s", filename);
        numFilesLeft--;
        printf("reading file: %s\n", filename);
        pthread_mutex_unlock(&mutexConfigFile);

        currentFile = fopen(filename, "r");
        // We read words until we're done with the file
        while ((word = extractWord(currentFile)) != NULL) {
            insertToHash(hash_table, word);
        }
        insertToBuffer(hash_table); // Insert result to buffer
        fclose(currentFile);
    }
    free(filename);
    pthread_exit(NULL);
}

void *insertToTree(void *threadArgs) {
    RBTree *tree = (RBTree *) threadArgs;
    int bufferReadIndex = 0;
    // The "OR" is for when we're finished adding elements to the queue, to empty it
    while (PRODUCERS_NOT_DONE || numElem > 0) {
        pthread_mutex_lock(&mutexHashTable);
        while (numElem == 0) { // Buffer empty -> wait
            pthread_cond_wait(&consQueue, &mutexHashTable);
        }
        insertHashtableToTree(tree, hashBuffer[bufferReadIndex]);
        tree->scannedFiles++;
        clearTable(hashBuffer[bufferReadIndex]); // Delete read data
        deleteTable(hashBuffer[bufferReadIndex]);
        bufferReadIndex = (bufferReadIndex + 1) % BUFFERSIZE; // Determine next index to read
        numElem--;
        if (numElem == BUFFERSIZE - 1) { // If buffer was full -> broadcast producers
            pthread_cond_broadcast(&prodQueue);
        }
        pthread_mutex_unlock(&mutexHashTable);
    }
    pthread_exit(NULL);
}

RBTree *createTree(char *dictionary, char *configFile) {
    hashBuffer = (List ***) malloc(sizeof(List **) * BUFFERSIZE); // Circular buffer to store data
    bufferWriteIndex = 0;
    numElem = 0;
    RBTree *tree = malloc(sizeof(RBTree));
    initTree(tree);
    FILE *fp = fopen(dictionary, "r");
    createDictionaryTree(tree, fp);
    fclose(fp);
    // We now have the dictionary created, let's now start with the word counting
    fp = fopen(configFile, "r");
    fscanf(fp, "%d", &numFilesLeft); // Beginning of configFile is number of files

    pthread_t threads[NUMTHREADS + 1];
    // Thread declaration and Mutex initialisation
    pthread_mutex_init(&mutexConfigFile, NULL);
    pthread_mutex_init(&mutexHashTable, NULL);
    pthread_cond_init(&prodQueue, NULL);
    pthread_cond_init(&consQueue, NULL);

    // Initialise the joinable attribute
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // We create the producers and then we wait
    PRODUCERS_NOT_DONE = true;
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_create(&threads[i], &attr, processFile, fp);
    }
    pthread_create(&threads[NUMTHREADS], &attr, insertToTree, tree); // Last thread -> to insert to tree
    pthread_attr_destroy(&attr);
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // The producers are done working, therefore we now wait for the consumer thread to finish
    PRODUCERS_NOT_DONE = false;
    pthread_join(threads[NUMTHREADS], NULL);

    // Free all
    free(hashBuffer);
    fclose(fp);
    pthread_mutex_destroy(&mutexHashTable);
    pthread_mutex_destroy(&mutexConfigFile);
    pthread_cond_destroy(&prodQueue);
    pthread_cond_destroy(&consQueue);
    return tree;
}