#include<stdlib.h>
#include<wait.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

void usage() {
    char buf[80];
    sprintf(buf,"USAGE: mi2: Has de passar un parametre enter\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[80];
    perror(txt);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[120];
    if(argc!=2) usage();
    int pid;
    int n = atoi(argv[1]);

    for(int i=0; i<n; i++) {
        pid = fork();
        if(pid<0) error("fork");
        else if(pid==0) {
            sprintf(buf,"mi pid: %d y mi posicion: %d\n", getpid(),i);
            write(1,buf,strlen(buf));
        }
        else if(pid>0) {
            if(waitpid(-1,NULL,0)<0) error("waitpid");
        }
    }
}