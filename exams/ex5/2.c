#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>

//int ret;

void usage() {
    char buf[100];
    sprintf(buf,"USAGE 2.c: \n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[100];
    perror(txt);
    exit(1);
}

void trat(int s) {
    char buf[100];
    if(s==SIGALRM) {

    }
    else if(s==SIGUSR1) {
        exit(alarm(0));
    }
}

int main(int argc, char*argv[]) {
    char buf[200];
    if(argc!=2) usage();
    int n = atoi(argv[1]);
    if(n>10 || n<0) usage();
    int pids[10];

    struct sigaction sa;
    sigset_t mask;
    //BLOK
    sigemptyset(&mask);
    sigaddset(&mask,SIGALRM);
    sigaddset(&mask,SIGUSR1);
    if(sigprocmask(SIG_BLOCK,&mask,NULL)<0) error("sigprocmask");
    //REP
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = trat;
    if(sigaction(SIGALRM,&sa,NULL)<0) error("sigaction");
    if(sigaction(SIGUSR1,&sa,NULL)<0) error("sigaction");

    int i;
    for(i=0; i<n; i++) {
        pids[i] = fork();
        if(pids[i]<0) error("fork");
        else if(pids[i] == 0) {
            alarm(3);
            //espera evento
            sigfillset(&mask);
            sigdelset(&mask,SIGALRM);
            sigdelset(&mask,SIGUSR1);
            sigsuspend(&mask);

            sprintf(buf,"%d: TIMEOUT\n",getpid());
            write(1,buf,strlen(buf));

            exit(0);
        }
    }
    for(i=0;i<n;i++) {
        if(pids[i]%2==0) {
            if(kill(pids[i],SIGUSR1)<0) error("kill");
        }
    }

    int stat;
    for(i=0;i<n;i++) {
        if((pids[i]=waitpid(-1,&stat,0))<0) error("waitpid");
        if(pids[i]%2==0) {
            if(WIFEXITED(stat)){
                sprintf(buf,"Hijo %d: Segundos restantes %d\n", pids[i], WEXITSTATUS(stat));
                write(1,buf,strlen(buf));
            }
        }
    }
}
    