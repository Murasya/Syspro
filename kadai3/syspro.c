#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>

#define LOOP_NUM 3
#define COPY_NUM 4

int SIZE;
char *line;

void copy1(char *src, char *dst) {
    FILE *fps, *fpd;
    int c;

    printf("copy1\n");
    fps = fopen(src, "r");
    if (fps == NULL) {
        perror(src);
        exit(1);
    }

    fpd = fopen(dst, "w");
    if (fpd == NULL) {
        perror(dst);
        exit(1);
    }

    while ((c = fgetc(fps)) != EOF) {
        fputc(c, fpd);
    }

    fclose(fpd);
    fclose(fps);
}

void copy2(char *src, char *dst) {
    FILE *fps, *fpd;

    printf("copy2\n");
    fps = fopen(src, "r");
    if (fps == NULL) {
        perror(src);
        exit(1);
    }

    fpd = fopen(dst, "w");
    if (fpd == NULL) {
        perror(dst);
        exit(1);
    }

    while (fgets(line, SIZE, fps) != NULL) {
        fputs(line, fpd);
    }

    fclose(fpd);
    fclose(fps);
}

void copy3(char *src, char *dst) {
    FILE *fps, *fpd;
    long count;

    fps = fopen(src, "rb");
    if (fps == NULL) {
        perror(src);
        exit(1);
    }

    fpd = fopen(dst, "wb");
    if (fpd == NULL) {
        perror(dst);
        exit(1);
    }
    printf("copy3\n");

    while ((count = fread(line, sizeof(char), SIZE,fps)) > 0) {
        fwrite(line, sizeof(char), count, fpd);
    }

    fclose(fpd);
    fclose(fps);
}

void copy4(char *src, char *dst) {
    long fps, fpd;
    long count;

    fps = open(src, O_RDONLY);
    if (fps < 0) {
        perror(src);
        exit(1);
    }

    fpd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fpd < 0) {
        perror(dst);
        close(fps);
        exit(1);
    }
    printf("copy4\n");

    while ((count = read(fps, line, SIZE)) > 0) {
        if (write(fpd, line, count) < 0) {
            perror("write");
            close(fpd);
            close(fps);
            exit(1);
        }
    }

    if (count < 0) {
        perror("read");
        close(fpd);
        close(fps);
        exit(1);
    }

    close(fpd);
    close(fps);
}



int main(int argc, char *argv[])
{
    int i, j;
    FILE *fps, *fpd;
    double t[COPY_NUM] = {0.0};
    clock_t st[COPY_NUM], en[COPY_NUM]; 

    if (argc != 2) {
        perror("argc");
        exit(1);
    }

    SIZE = atoi(argv[1]);

    if ((line = (char *)malloc(SIZE*2)) == NULL){
        perror("malloc");
        exit(1);
    }

    for(i = 0; i < LOOP_NUM; i++) {
        st[0] = clock();
        copy1("test.txt", "cp1.txt");
        en[0] = clock();

        st[1] = clock();
        copy2("test.txt", "cp2.txt");
        en[1] = clock();

        st[2] = clock();
        copy3("test.txt", "cp3.txt");
        en[2] = clock();

        st[3] = clock();
        copy4("test.txt", "cp4.txt");
        en[3] = clock();

        for (j = 0; j < COPY_NUM; j++) {
            t[j] += (double)(en[j] - st[j]) / CLOCKS_PER_SEC;
            printf("t[%d]:%f\n", j, t[j]);
        }
    }

    for (i = 0; i < COPY_NUM; i++) {
        t[i] /= (double)LOOP_NUM;
    }

    printf("buffer size  : %d\n", SIZE);
    printf("fgetc, fputc : %f\n", t[0]);
    printf("fgets, fputs : %f\n", t[1]);
    printf("fread, fwrite: %f\n", t[2]);
    printf("read , write : %f\n", t[3]);

    free(line);

    return 0;
}

