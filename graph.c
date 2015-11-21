#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
//Sort descending
int cmpFunc(const void * a, const void * b){
    if (a->numItems < b->numItems){
        return 1;
    }
    if (a->numItems == b->numItems){
        return 0;
    }
    if (a->numItems > b->numItems){
        return -1;
    }
}

void drawGraph(RBTree* tree, char* word){
    RBData* treeNode = findNode(tree, word);
    float numApps[appVector->numItems];
    qsort(appVector, appVector->numItems, sizeof List, cmpFunc);
    for (int i=0; i<appVector->numItems; i++){
        numApps[i] = numApps / (appVector[i]->numTimes);
    }
    FILE *data = fopen("appearances.data", "w+");
    if (!data){
        printf("Error saving data");
    }
    fprintf(fp, "%f %s\n", numApps);
    fclose(fp);

    FILE* gnuplot = popen("gnuplot -p", "w");
    if (!gnuplot){
        printf("Error creating pipes\n");
    }
    fprintf(gnuplot, "plot \"appearances.data\" with lines\n");
    fflush(gnuplot);
    if (pclose(gnuplot) == -1){
        printf("Error closing pipes\n");
    }
}

int main(){
    drawGraph(1);
    return 0;
}