#include <stdio.h>
#include <string.h> // per la funcio strlen
#include <ctype.h>  // per les funcions isalpha, isdigit, ...
#include <sys/stat.h>

/*
  Generator of next word of file (taking into account given criteria)
*/
char* extractWord(FILE* fileToSeparate) {
  int tmpWordCount = 0, isEndWord = FALSE;
  char tmpChar;
  char* tmpWord = calloc(MAXCHAR, sizeof(char));

  while (isEndWord == FALSE && tmpChar != EOF) {
    tmpChar = (char)fgetc(fileToSeparate);
    switch(categorizeCharacter(tmpChar)){
      case -1 : // Dump Word
        tmpWord = calloc(MAXCHAR, sizeof(char)); tmpWordCount = 0; break;
      case 0 : // Add character
        tmpWord[tmpWordCount++] = tmpChar; break; 
      case 1 : // End word
        isEndWord = TRUE; tmpWordCount = 0; break;
    }
  }
  
  if (tmpChar != EOF || strlen(tmpWord) > 0){
    return tmpWord;
  } else {
    return NULL;
  }

/*
  Function that returns
    -1 -> dump current word
    0 -> continue with word
    1 -> end current word
*/
int categorizeCharacter(char character){
  char tmpChar = toLowerCase(character);
  if (isalpha(tmpChar)) {
    return 0;
  }
  else if (isdigit(tmpChar)) {
    return -1;
  }
  else if (ispunct(tmpChar)) {
    if (tmpChar == "-") {
      return 1;
    }
    else if (tmpChar == "'") {
      return 0;
    }
    else {
      return -1;
    }
  }
  else if (isspace(tmpChar)) {
    return 1;
  }
  else {
    return -1;
  }
}