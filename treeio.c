#include "red-black-tree.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "treeio.h"
#include "strutils.h"
#include "extract-words.h"
#include "hashutils.h"

static pthread_mutex_t mutexTree, mutexConfigFile;

static struct thread_data threadData;

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

static void insertHashtableToTree(RBTree *tree, List **hash_table, char *filename) {
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
    printf("%s has %d different words\n", filename, differentWords);
}

void *processFile(void *threadArg) {
    FILE *currentFile;
    FILE *fp = threadData.fp;
    char *word;
    List **hash_table = createHashTable(SIZE);
    RBTree *tree = threadData.tree;
    while (threadData.filesLeft > 0) {
        // Note, this is malloc'd here due to the filename being used in the list of occurrences
        char *filename = malloc(sizeof(char) * MAXCHAR);
        /*
         * One file path per line. Plus we also have to lock to avoid race conditions
         */
        pthread_mutex_lock(&mutexConfigFile);
        // This is just if another thread finished the remaining files before freeing the lock
        if (threadData.filesLeft <= 0) {
            pthread_mutex_unlock(&mutexConfigFile);
            pthread_exit(NULL);
        }
        fscanf(fp, "%s", filename);
        threadData.filesLeft--;
        printf("reading file: %s\n", filename);
        pthread_mutex_unlock(&mutexConfigFile);

        currentFile = fopen(filename, "r");
        // We read words until we're done with the file
        while ((word = extractWord(currentFile)) != NULL) {
            insertToHash(hash_table, word);
        }
        // And now we just enter the numbers into the tree and reset the table
        pthread_mutex_lock(&mutexTree);
        insertHashtableToTree(tree, hash_table, filename);
        tree->scannedFiles++;
        pthread_mutex_unlock(&mutexTree);

        clearTable(hash_table);
        fclose(currentFile);
    }
    deleteTable(hash_table);
    free(hash_table);
    pthread_exit(NULL);
}

RBTree *createTree(char *dictionary, char *configFile) {
    // Thread declaration and Mutex initialisation
    pthread_mutex_init(&mutexConfigFile, NULL);
    pthread_mutex_init(&mutexTree, NULL);
    pthread_t threads[NUMTHREADS];
    pthread_attr_t attr;
    // Initialise the joinable attribute
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    RBTree *tree = malloc(sizeof(RBTree));
    FILE *fp;
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
    // Thread data setup
    threadData.fp = fp;
    threadData.tree = tree;
    threadData.filesLeft = numFiles;
    // We create and then we wait
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_create(&threads[i], &attr, processFile, NULL);
    }
    pthread_attr_destroy(&attr);
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    fclose(fp);
    return tree;
}
