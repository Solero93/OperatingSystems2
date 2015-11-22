#include "strutils.h"
#include <string.h>
#include <ctype.h>

/*
 Function that lowercases a word in-place
 */
void lowercaseWord(char * word) {
  for (int i = 0; word[i] != '\0'; i++) {
    word[i] = (char) tolower(word[i]);
  }
}
