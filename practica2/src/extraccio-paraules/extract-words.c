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
    if (isalpha(currentChain[i])) {
      // If it's a letter, add it
      tmp[tmpWordCount] = currentChain[i];
      tmpWordCount++;
    }
    else if (isdigit(currentChain[i])) {
      //If it's a number, dump word
      tmp = calloc(len, sizeof(char));
    }
    else if (ispunct(currentChain[i])) {

      if (currentChain[i] == "-") {
        // If it's a hyphen, separate into another word
        extractedWords[extractedCount] = tmp;
        extractedCount++;
        tmp = calloc(len, sizeof(char));
      }
      else if (currentChain[i] == "'") {
        // If apostrophe, add it to word
        tmp[tmpWordCount] = currentChain[i];
        tmpWordCount++;
      }
      else {
        // Rest of cases, dump word
        extractedWords[extractedCount] = tmp;
        extractedCount++;
        tmp = calloc(len, sizeof(char));
      }
    }
    else if (isspace(currentChain[i])) {
      // If it's a space or linebreak, separate word
      extractedWords[extractedCount] = tmp;
      extractedCount++;
      tmp = calloc(len, sizeof(char));
    }
    else {
      tmp = calloc(len, sizeof(char));
    }
  }

  // FIXME
  if (tmp != NULL)
    extractedWords[extractedCount] = tmp;

  return extractedWords;
}