#include<stdio.h>
#define SIZE 10000

int main()
{
    char a[SIZE];
    int i;
    FILE *fp;

    for(i = 0; i < SIZE-1; i++){
        a[i] = 'a';
    }

    fp = fopen("test.txt", "w");
    for(i = 0; i < 10000; i++) {
        fputs(a, fp);
    }
    fclose(fp);
}