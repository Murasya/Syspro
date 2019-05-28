#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define TIMEOUT 10

volatile sig_atomic_t alrm_count;

void alrm(int signum)
{
    alrm_count--;
}

int mygetchar(int timeout)
{
    int ch;
    struct sigaction sa_alarm;
    struct itimerval itimer;

    ch = getchar();
    if (ch != EOF) {
        return ch;
    }

    arlm_count = timeout;

    memset(&sa_alarm, 0, sizeof(sa_alarm));
    sa_alarm.sa_handler = alrm;
    sa_alarm.sa_flags = SA_RESTART;

    if (sigaction(SIGALRM, &sa_alarm, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }

    itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = 1;
    itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
        perror("setitimer");
        exit(1);
    }

    while (alrm_count) {
        pause();
        printf("%d: %d\n", alrm_count, time(NULL));
    }

    itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = 0;
    itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;
    
    if (settimer(ITIMER_REAL, &itimer, NULL) < 0) {
        perror("setitimer");
        exit(1);
    }
}

int main(void)
{
    int n;
    n = mygetchar(TIMEOUT);

    printf("mygetchar :%d\n", n);

    return 0;
}