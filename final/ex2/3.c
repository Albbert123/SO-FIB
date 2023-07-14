#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<wait.h>
#include <sys/types.h>
#include <sys/wait.h>

int n,s,h;
int pids[20];

void trat(int s) {
    char buf[100];
    if(s==SIGINT) {
        sprintf(buf,"SIGINT\n");
        write(1,buf,strlen(buf));
        for(int i=0; i<n; ++i) {
            kill(pids[i],SIGKILL);
        }
        exit(0);
    }
    else if(s==SIGALRM) {
        ++s;
        sprintf(buf,"llevamos %d segundos y %d hijos creados\n",s,h);
        write(2,buf,strlen(buf));
    }
}

void usage() {
    char buf[80];
    sprintf(buf,"usage\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*msg) {
    char buf[80];
    perror(msg);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[280];
    if(argc != 3) usage();

    n = atoi(argv[1]);
    pids[n];
    int i;
    int fexit = 0;
    s = 0;
    h = 0;

    struct sigaction sa;
    sigset_t mask;

    //blok
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigaddset(&mask,SIGALRM);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    //repro
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &trat;
    sigfillset(&sa.sa_mask);
    if(sigaction(SIGINT,&sa,NULL)<0) error("sig");
    if(sigaction(SIGALRM,&sa,NULL)<0) error("sig");

    //desblok
    sigfillset(&mask);
    sigdelset(&mask,SIGALRM);
    sigdelset(&mask,SIGINT);

    alarm(1);

    for(i=0; fexit=0 && i<n; ++i) {
        pids[i] = fork();
        ++h;
        if(pids[i] < 0) error("fork");

        else if(pids[i] == 0) {
            execlp("./hijos","./hijos",argv[2],NULL);
            error("exec");
        }

        int stat1;
        waitpid(-1,&stat1,0);
        if(WIFEXITED(stat1)) {
            if(WEXITSTATUS(stat1) == 0) {
                sprintf(buf,"pid %d ended correctly\n",pids[i]);
                write(1,buf,strlen(buf));
                for(int i=0; i<n; ++i) {
                    kill(pids[i],SIGKILL);
                }
                fexit = 1;
            }
            else {
                sprintf(buf,"pid %d ended by exit\n",pids[i]);
                write(1,buf,strlen(buf));
            }
        }
        else if(WIFSIGNALED(stat1)) {
            sprintf(buf,"pid %d ended by signal\n",pids[i]);
            write(1,buf,strlen(buf));
        }
    }
    
    int stat2;
    while(waitpid(-1,&stat2,0)>0) {
        if(WIFEXITED(stat2)) {
            if(WEXITSTATUS(stat2) == 0) {
                sprintf(buf,"pid %d ended correctly\n",pids[i]);
                write(1,buf,strlen(buf));
                //exit(0);
                for(int i=0; i<n; ++i) {
                    kill(pids[i],SIGKILL);
                }
                fexit = 1;
            }
            else {
                sprintf(buf,"pid %d ended by exit\n",pids[i]);
                write(1,buf,strlen(buf));
            }
        }
        else if(WIFSIGNALED(stat2)) {
            sprintf(buf,"pid %d ended by signal\n",pids[i]);
            write(1,buf,strlen(buf));
        }
    }
    if(fexit == 0) {
        sprintf(buf,"incalculable\n");
        write(1,buf,strlen(buf));
    }
    exit(0);
}
