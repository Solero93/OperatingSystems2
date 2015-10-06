#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void)
{
  int size;
  char *str;
  FILE *fp;

  struct stat st;
  stat("fitxer.txt", &st);
  size = st.st_size;
  
  printf("El fitxer te %d bytes!\n", size);
  
  fp = fopen("fitxer.txt", "r");
  if (!fp) {
    printf("No he pogut obrir fitxer.\n");
    exit(1);
  }

  str =  malloc(sizeof(char) * (size + 1));
  fread(str, sizeof(char), size, fp);
  fclose(fp);
  
  str[size] = 0;
  
  printf("Contingut del fitxer:\n");
  printf("%s", str);
  
  free(str);
}
