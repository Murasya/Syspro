#include<stdio.h>
#define N 5

void loop(int i)
{
  if (i > 5)
    return;
  loop(++i);
}

int main()
{
  int i = 0;
  loop(i);
  return 0;
}
