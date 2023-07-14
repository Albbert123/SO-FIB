#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int n,seg,pid;
void trat(int s) {
    char buf[264];
    if(s == SIGUSR1) {
        sprintf(buf,"llevamos %d segundos\n",seg);
        write(1,buf,strlen(buf));
    }

    else if(s == SIGALRM) {
        ++s;
        if(s == n) {
            if(kill(pid,SIGKILL)<0) error("kill");
            waitpid(-1,NULL,0);
            exit(1);
        }
    }

    else if(s == SIGCHLD) {
        if(kill(pid,SIGKILL)<0) error("kill");
        waitpid(-1,NULL,0);
        exit(1);
    }
}

void usage() {
    char buf[80];
    sprintf(buf,"usage\n");
    wrie(1,buf,strlen(buf));
    exit(0);
}

void error(char*msg) {
    char buf[80];
    perror(msg);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[264];
    if(argc!=3) usage();
    n = atoi(argv[1]);
    
    struct sigaction sa;
    sigset_t mask;

    //blok
    sigemptyset(&mask);
    sigaddset(&mask,SIGALRM);
    sigaddset(&mask,SIGUSR1);
    sigaddset(&mask,SIGCHLD);
    if(sigprocmask(SIG_BLOCK,&mask,NULL)<0) error("sig");

    //repro
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &trat;
    sigfillset(&sa.sa_mask);
    if(sigaction(SIGALRM,&sa,NULL)<0) error("sig");
    if(sigaction(SIGUSR1,&sa,NULL)<0) error("sig");
    if(sigaction(SIGCHLD,&sa,NULL)<0) error("sig");

    //deblok
    sigfillset(&mask);
    sigdelset(&mask,SIGALRM);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGCHLD);

    pid = fork();
    if (pid < 0) error("fork");
    else if(pid == 0) {
        if(sigprocmask(SIG_UNBLOCK,&mask,NULL)<0) error("sig"); //restauro
        sprintf(buf,"./%s",argv[2]);
        execlp(buf,buf,NULL);
        error("exec");
    }

    while(seg < n) {
        alarm(1);
        if(sigsuspend(&mask) < 0) error("sigsuspend");
        ++seg;
    }
    exit(0);
}
