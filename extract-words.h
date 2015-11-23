#ifndef EXTRACT_WORDS_HEADER
#define EXTRACT_WORDS_HEADER 1

#ifndef MAXCHAR
#define MAXCHAR 100
#endif

#include <stdio.h>

#define FALSE 0
#define TRUE 1

#define DUMP_WORD -1
#define ADD_CHARACTER 0
#define FINISH_WORD 1

char *extractWord(FILE *fileToSeparate);

int categorizeCharacter(char character);

#endif
