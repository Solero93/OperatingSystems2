/**
 *
 * Main file
 *
 * Lluis Garrido, 2015.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAXCHAR  5

/**
 *
 * Given a filename, this function extracts the dictionary words stored in it.
 * Words are assumed to be separated by newlines.
 *
 */

void processDictionary(char *filename)
{
  FILE *fp;

  char paraula[MAXCHAR];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Could not open file '%s'\n", filename);
    exit(1);
  }

  while (fscanf(fp, "%s", paraula) != EOF)
  {
     printf("%s\n", paraula);
     fflush(stdout);
  }

  fclose(fp);
}

/**
 *
 * Main function.
 *
 */

int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("Usage: %s <fitxer.txt>\n", argv[0]);
    exit(1);
  }

  processDictionary(argv[1]);

  return 0;
}
