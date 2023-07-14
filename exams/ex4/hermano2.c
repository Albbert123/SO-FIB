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
    int pid, ant=getppid();
    for(int i = 0; i<n; i++) {
        pid = fork();
        if(pid<0) error("fork");
        else if(pid == 0) {
            sprintf(buf,"El pid de mi hermano mayor es: %d \n", ant);
            write(1,buf,strlen(buf));
            sprintf(buf,"%d",ant);
            if(i>0)execlp("./signal", "signal", buf, NULL);
            error("execlp");
        }
        else {
            ant = pid;
        }
    }
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    
    int stat,ret;
    while((ret=waitpid(-1,&stat,0))>0) {
        if(WIFEXITED(stat)) {
            sprintf(buf,"El pid hjo %d ha muerto por exit code %d\n", ret, WEXITSTATUS(stat));
            write(1,buf,strlen(buf));
        }
        else if(WIFSIGNALED(stat)) {
            sprintf(buf,"El pid hjo %d ha muerto por signal %d\n", ret, WTERMSIG(stat));
            write(1,buf,strlen(buf));
        }
    }
}