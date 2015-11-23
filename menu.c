#include <stdio.h>
#include "treeio.h"
#include "graph.h"

void showMenu() {
    RBTree *tree = NULL;
    char choice;
    char tmp1[100], tmp2[100];
    do {
        printf("\tTREE MENU:\n");
        printf("1. Create Tree\n");
        printf("2. Save Tree\n");
        printf("3. Read Tree\n");
        printf("4. Word appearance probability graph\n");
        printf("5. Exit\n");
        printf("Enter your choices: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                printf("Enter the dictionary file: ");
                scanf("%s", tmp1);
                printf("Enter the configuration file: ");
                scanf("%s", tmp2);
                /*if (tree != NULL) {
                    deleteTree(tree);
                    free(tree);
                }*/
                tree = createTree(tmp1, tmp2);
                break;
            case '2':
                if (tree == NULL) {
                    printf("No tree created yet. Create one and try to save it afterwise.");
                } else {
                    printf("Enter the filename to save: ");
                    scanf("%s", tmp1);
                    saveTree(tmp1, tree);
                }
                break;
            case '3':
                printf("Enter the filename to load: ");
                scanf("%s", tmp1);
                /*if (tree != NULL) {
                    deleteTree(tree);
                    free(tree);
                }*/
                tree = readTree(tmp1);
                break;
            case '4':
                if (tree == NULL) {
                    printf("No tree created yet. Create one and try to draw a graph afterwise.");
                }
                else {
                    printf("Enter a word to analyze: ");
                    scanf("%s", tmp1);
                    drawGraph(tmp1, tree);
                }
                break;
            case '5':
                printf("See ya!");
                break;
            default:
                printf("Wrong option, try again");
                break;
        }
        printf("\n");
    } while (choice != '5');
    if (tree != NULL) {
        deleteTree(tree);
        free(tree);
    }
}

int main() {
    showMenu();
    return 0;
}