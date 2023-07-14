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
    sprintf(buf,"USAGE: Has de passar minim 1 argument\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[80];
    perror(txt);
    exit(0);
}

int main(int argc, char*argv[]) {
    char buf[120];
    if(argc==1) usage();
    int ret;
    for(int i = 1; i < argc; i++) {
        ret = fork();
        if(ret == 0) {
            sprintf(buf,"soy el hijo %d con argumento %s \n", getpid(), argv[i]);
            write(1,buf,strlen(buf));
            exit(0);
        }
    }
    int pidh;
    while((pidh=waitpid(-1,NULL,0))>0) {
        sprintf(buf,"soy el hijo %d\n", pidh);
        write(1,buf,strlen(buf)); 
    }
    exit(0);
}