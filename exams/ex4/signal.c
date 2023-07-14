#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid;

void usage() {
    char buf[100];
    sprintf(buf,"USAGE signal.c: \n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[100];
    perror(txt);
    exit(1);
}

void trat(int s) {
    if(s==SIGALRM) {
        if(kill(pid,SIGUSR1)<0) error("kill");
    }
}

int main(int argc, char*argv[]) {
    char buf[200];
    if(argc!=2) usage();
    pid = atoi(argv[1]);

    struct sigaction sa;
    sigset_t mask;

    //blok
    sigemptyset(&mask);
    sigaddset(&mask,SIGALRM);
    sigaddset(&mask,SIGUSR1);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    //reprogramar SIGALRM
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = trat;
    if(sigaction(SIGALRM,&sa,NULL)<0) error("sigaction");
    //if(sigaction(SIGUSR1,&sa,NULL)<0) error("sigaction");

    //espera
    sigfillset(&mask);
    sigdelset(&mask,SIGALRM);
    sigdelset(&mask,SIGUSR1);

    alarm(1);
    sigsuspend(&mask);

}