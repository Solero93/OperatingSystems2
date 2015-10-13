#include <stdio.h>
#include <string.h> // per la funcio strlen
#include <ctype.h>  // per les funcions isalpha, isdigit, ...
#include <sys/stat.h>

#define MAXCHAR 100

int extract_words(char* wordToSeparate) {
  int len = strlen(wordToSeparate), extractedCount=0, tmpWordCount=0;
  char* tmpWord;
  char** extractedWords;

  wordToSeparate = toLowerCase(wordToSeparate);

  extractedWords = calloc(len, sizeof(char*));
  tmp = calloc(len, sizeof(char));

  for(int i = 0; i < len; i++) {
    //printf("Posicio: %02i, caracter: %c, es de tipus: ", i, (char)currentChain[i]);
    if (isalpha(currentChain[i]))
      tmp[tmpWordCount] = currentChain[i];
      tmpWordCount++;
    else if (isdigit(currentChain[i]))
      tmp = calloc(len, sizeof(char));
      printf("nombre");
    else if (ispunct(currentChain[i]))
      //Search for hyphen or apostrophe
    else if (isspace(currentChain[i]))
      extractedWords[extractedCount] = tmp;
      tmp = calloc
      printf("espai o retorn de carro");
    else 
      printf("no se sap");

    printf("\n");
  }
}