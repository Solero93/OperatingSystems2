#include <stdio.h>
#include "menu.h"
#include "treeio.h"

RBTree* tree;

void createTree(char dict[], char config[]){
    printf("%s %s", dict, config);
}

void saveTree(char filename[]){
    printf("%s", filename);
}

void loadTree(char filename[]){
    printf("%s", filename);
}

void drawGraph(){
    printf("Draw graph");
}

void showMenu(){
    int choice;
    char tmp1[200], tmp2[200];

    do{
        printf("\tTREE MENU:\n");
        printf("1. Create Tree\n");
        printf("2. Save Tree\n");
        printf("3. Read Tree\n");
        printf("4. Word appearance probability graph\n");
        printf("5. Exit\n");
        printf("Enter your option: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                printf("Enter the dictionary file: ");
                scanf("%s", tmp1);
                printf("Enther the configuration file: ");
                scanf("%s", tmp2);
                createTree(tmp1, tmp2);
                break;
            case 2:
                printf("Enter the filename to save: ");
                scanf("%s", tmp1);
                saveTree(tmp1);
                break;
            case 3:
                printf("Enter the filename to load: ");
                scanf("%s", tmp1);
                loadTree(tmp1);
                break;
            case 4:
                drawGraph();
                break;
            case 5:
                printf("Exit!");
                break;
            default:
                printf("Wrong option, try again");
                break;
        }
        printf("\n");
    } while (choice!=5);
}

int main(){
    showMenu();
    return 0;
}