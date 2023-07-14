#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>

void usage() {
    char buf[100];
    sprintf(buf,"USAGE 1.c: \n");
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
    if(argc>6) usage();
    int i;
    int*pidh;
    for(i=1; i<argc; i++) {
        pidh[i]=fork();
        if(pidh[i]<0) error("fork");
        else if(pidh[i] == 0) {
            execlp("touch","touch",argv[i],NULL);
            error("execlp");
        }
    }
    int stat;
    for(i=1; i<argc;i++) {
        if(waitpid(-1,&stat,0)<0) error("waitpid");
        sprintf(buf,"fecha %s actualizada por %d\n", argv[i], pidh[i]);
        write(1,buf,strlen(buf));
    }
}

