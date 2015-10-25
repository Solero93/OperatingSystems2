#include <stdio.h>
#include <string.h> // per la funcio strlen
#include <ctype.h>  // per les funcions isalpha, isdigit, ...
#include <sys/stat.h>
#include <stdlib.h>

#include "extract-words.h"

/*
  Generator of next word of file (taking into account given criteria)
*/
char* extractWord(FILE* fileToSeparate) {
  int tmpWordCount = 0, isEndWord = FALSE;
  char tmpChar = ' ';
  char* tmpWord = calloc(MAXCHAR, sizeof(char));

  while (isEndWord == FALSE && tmpChar != EOF) {
    tmpChar = (char)fgetc(fileToSeparate);
    tmpChar = (char) tolower(tmpChar);
    switch(categorizeCharacter(tmpChar)){
      case -1 : // Dump Word
        free(tmpWord);
        tmpWord = calloc(MAXCHAR, sizeof(char));
        tmpWordCount = 0;
        break;
      case 0 : // Add character
        tmpWord[tmpWordCount++] = tmpChar;
        break;
      case 1 : // End word
        isEndWord = TRUE;
        tmpWordCount = 0;
        break;
    }
  }

  if (strlen(tmpWord) > 0){
    return tmpWord;
  } else {
    return NULL;
  }
}
/*
  Function that returns
    -1 -> dump current word
    0 -> continue with word
    1 -> end current word
*/
int categorizeCharacter(char character){
  if (isalpha(character)) {
    return 0;
  }
  else if (isdigit(character)) {
    return -1;
  }
  else if (ispunct(character)) {
    if (character == '\'') {
      return 0;
    }
    else {
      return 1;
    }
  }
  else if (isspace(character)) {
    return 1;
  }
  else {
    return -1;
  }
}
