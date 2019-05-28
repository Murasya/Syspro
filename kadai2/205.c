#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_SIZE 100
#define STRING_SIZE2 10

size_t my_strlcpy(char *dst, const char *src, size_t size);
char *my_strdup(const char *string);

int main(void)
{
  int n1, n2;
  char str0[STRING_SIZE] = "abcdefg0123456789";
  char str1[STRING_SIZE], str2[STRING_SIZE], *str3, *str4;
  
  n1 = strlcpy(str1, str0, STRING_SIZE);
  n2 = my_strlcpy(str2, str0, STRING_SIZE);
  str3 = strdup(str0);
  str4 = my_strdup(str0);
  
  printf("元データ: %s\n", str0);
  printf("strlcpy: %s, %d\n", str1, n1);
  printf("my_strlcpy: %s, %d\n", str2, n2);
  printf("strdup: %s\n", str3);
  printf("my_strdup: %s\n", str4);
  
  free(str3);
  free(str4);
  return 0;
}

size_t my_strlcpy(char *dst, const char *src, size_t size)
{
  int i;
  for (i = 0; i < size; i++) {
    dst[i] = src[i];
    if (dst[i] == '\0')
      return i;
  }
  return i;
}

char *my_strdup(const char *string)
{
  int i;
  char *dup;
  if ((dup = malloc(strlen(string))) == NULL){
    fprintf(stderr, "malloc error");
    exit(1);
  }
  for (i = 0; i < strlen(string); i++){
    dup[i] = string[i];
  }
  return dup;
}
