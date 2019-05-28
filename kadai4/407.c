#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int pipe_fd[2];
int pipe_fd2[2];
int i, j;

void do_child(int a)
{
  char *num = "0123456789";
  char c;
  int count = 0;
  close(pipe_fd[0]);
  close(pipe_fd2[1]);
  
  while (++count <= a) {
    if (count % 2 == 1) {
      if (i == 10) {
        i = 0;
      }
      putchar(num[i++]);
      fflush(stdout);
      if (write(pipe_fd[1], &num[i], 1) < 0) {
        perror("write");
        exit(1);
      }

      if (read(pipe_fd2[0], &c, 1) < 0) {
        perror("read");
        exit(1);
      }
    }
  }
}

void do_parent(int a)
{
  char *c = "abcdefghijklmnopqrstuvwxyz";
  char num;
  int count = 0;
  close(pipe_fd[1]);
  close(pipe_fd2[0]);
  
  while (count < a) {
    if (count % 2) {
      if (read(pipe_fd[0], &num, 1) < 0) {
        perror("read");
        exit(1);
      }
      if (j == 26) {
        j = 0;
      }
      putchar(c[j++]);
      fflush(stdout);
      if (write(pipe_fd2[1], &c[j], 1) < 0) {
        perror("write");
        exit(1);
      }
    }
    count++;
  }
  if (read(pipe_fd[0], &c, 1) < 0) {
    perror("read");
    exit(1);
  }
  printf("\n");
}

int main(int argc, char *argv[])

  int child, status, i;

  if (argc != 2) {
    perror("argc");
    exit(1);
  }

  if (pipe(pipe_fd) < 0) {
    perror("pipe");
    exit(1);
  }

  if (pipe(pipe_fd2) < 0) {
    perror("pipe");
    exit(1);
  }

  if ((child = fork()) < 0) {
    perror("fork");
    exit(1);
  }
  
  if (child) {
    do_parent(atoi(argv[1]));
  } else {
    do_child(atoi(argv[1]));
  }

  return 0;
}
