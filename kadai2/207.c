#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SIZE 1000000000
#define LOOP_NUM 3

size_t my_strlen1(const char *s);
size_t my_strlen2(const char *s);

int main(void)
{
  long i, j, n1, n2, n3;
  char *str0;
  clock_t st[3], en[3];
  double t[3]={0};

  str0 = (char *)malloc(SIZE+3);

  memset(str0, '1', SIZE);

  for (i = 0; i < LOOP_NUM; i++){
    st[1] = clock();
    n2 = my_strlen1(str0);
    en[1] = clock();

    st[2] = clock();
    n3 = my_strlen2(str0);
    en[2] = clock();

    st[0] = clock();
    n1 = strlen(str0);
    en[0] = clock();

    for (j = 0; j < 3; j++){
      t[j] += (double)(en[j]-st[j])/CLOCKS_PER_SEC;
    }
  }


  printf("strlen: %ld\nmy_strlen1: %ld\nmy_strlen2: %ld\n", n1, n2, n3);
  printf("strlen     time: %fs\n", t[0]/LOOP_NUM);
  printf("my_strlen1 time: %fs\n", t[1]/LOOP_NUM);
  printf("my_strlen2 time: %fs\n", t[2]/LOOP_NUM);

  free(str0);
  return 0;
}

size_t my_strlen1(const char *s)
{
  long i = 0;
  while (s[i] != '\0') {
    i++;
  }
  return i;
}

size_t my_strlen2(const char *s)
{
  size_t len = 0;
  for(;;){
    unsigned x = *(unsigned*)s;
    if ((x & 0xff) == 0) return len;
    if ((x & 0xff00) == 0) return len + 1;
    if ((x & 0xff0000) == 0) return len + 2;
    if ((x & 0xff000000) == 0) return len + 3;
    s += 4, len += 4;
  }
}
