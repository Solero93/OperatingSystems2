#include <stdio.h>
#include <ctype.h>  // per les funcions isalpha, isdigit, ...
#include <stdlib.h>

#include "extract-words.h"

/*
  Generator of next word of file (taking into account given criteria)
*/
char *extractWord(FILE *fileToSeparate) {
    int tmpWordCount = 0, isEndWord = FALSE;
    char tmpChar = ' ';
    char *tmpWord = calloc(MAXCHAR, sizeof(char));

    while (isEndWord == FALSE && tmpChar != EOF) {
        tmpChar = (char) fgetc(fileToSeparate);
        tmpChar = (char) tolower(tmpChar);
        switch (categorizeCharacter(tmpChar)) {
            case DUMP_WORD :
                free(tmpWord);
                tmpWord = calloc(MAXCHAR, sizeof(char));
                tmpWordCount = 0;
                break;
            case ADD_CHARACTER :
                tmpWord[tmpWordCount++] = tmpChar;
                break;
            case FINISH_WORD :
                // If it's a false positive (there are no letters in tmpWord), we ignore it
                if (tmpWordCount == 0)
                    break;
                isEndWord = TRUE;
                tmpWordCount = 0;
                break;
        }
    }

/* This is subtle, let's break it down, we first see if we reached the end of
   the file, if we haven't then it means we have a word (isEndWord is TRUE).
   Otherwise, it means we reached the end of the file, so we see if we have extracted
   anything until now.
 */
    if (tmpChar != EOF || tmpWordCount > 0) {
        return tmpWord;
    } else {
        free(tmpWord);
        return NULL;
    }
}

/*
  Function that tells us what to do with the characterh
*/
int categorizeCharacter(char character) {
    if (isalpha(character)) {
        return ADD_CHARACTER;
    }
    else if (isdigit(character)) {
        return DUMP_WORD;
    }
    else if (ispunct(character)) {
        if (character == '\'') {
            return ADD_CHARACTER;
        }
        else {
            return FINISH_WORD;
        }
    }
    else if (isspace(character)) {
        return FINISH_WORD;
    }
    else {
        return DUMP_WORD;
    }
}
