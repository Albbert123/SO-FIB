#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<wait.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void usage() {
    char buf[80];
    sprintf(buf,"usage\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[264];
    if(argc<1 || argc >10) usage();
    int pids[argc],i;
    for(i=1; i<=argc; ++i) {
        pids[i] = fork();
        if(pids[i] == 0) {
            sprintf(buf,"trato el programa: %s\n",argv[i]);
            write(1,buf,strlen(buf));
            exit(i);
        }

    }
    int stat, i=1;
    while(waitpid(-1,&stat,0)>0) {
        if(WIFEXITED(stat)) {
            sprintf(buf,"hijo %d muerto por exit %s\n", pids[i],WEXITSTATUS(stat));
            write(1,buf,strlen(buf));
            ++i;
        }
    }
    exit(0);
}
