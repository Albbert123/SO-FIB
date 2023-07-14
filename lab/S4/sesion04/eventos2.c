#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int count = 0;

void tracta(int s)
{
    if (s == SIGALRM) count += 1;
    if (s == SIGUSR1) count = 0;
    else if (s == SIGUSR2) {
        char buf[80];
        sprintf(buf, "Valor comptador: %d\n", count);
        write(1, buf, strlen(buf));
    }
    struct sigaction sa1;
    sa1.sa_handler = SIG_DFL;
    sa1.sa_flags = SA_RESETHAND;
    sigfillset(&sa1.sa_mask);
    sigaction(SIGALRM, &sa1, NULL);
    sigaction(SIGUSR1, &sa1, NULL);
    sigaction(SIGUSR2, &sa1, NULL);
}

int main(int argc,char *argv[]) {
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
	sigprocmask(SIG_BLOCK, &mask, NULL);

    struct sigaction sa;
    sa.sa_handler = &tracta;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);

    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    while (1) {
        alarm(1);
        sigfillset(&mask);
        sigdelset(&mask, SIGALRM);
        sigdelset(&mask, SIGUSR1);
        sigdelset(&mask, SIGUSR2);
        sigdelset(&mask, SIGINT);
        sigsuspend(&mask);
    }
}
