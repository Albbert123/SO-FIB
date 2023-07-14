#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void usage() {
    char buf[100];
    sprintf(buf,"USAGE hermano.c: \n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[100];
    perror(txt);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[200];
    if(argc!=2) usage();
    int n = atoi(argv[1]);
    int pid;
    int ant=getppid();
    for(int i = 0; i<n; i++) {
        pid = fork();
        if(pid<0) error("fork");

        if(pid == 0) {
            sprintf(buf,"El pid de mi hermano mayor es: %d \n", ant);
            write(1,buf,strlen(buf));
            exit(0);
        }
        else ant = pid;
    }
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    
    int stat;
    while(waitpid(-1,&stat,0)>0);
}
