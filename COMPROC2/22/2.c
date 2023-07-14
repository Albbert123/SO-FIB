#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    char buf[128];
    //usage()
    int pid1,pid2,pid3;
    int p1[2],p2[2];

    pipe(p1);
    pid1 = fork();
    if (pid1 == 0) {
        dup2(p1[1],1);
        close(p1[1]);
        close(p1[0]);
        execlp("cat","cat",argv[1],NULL);
        //error()
        exit(0);
    }
    close(p1[1]);

    pipe(p2);
    pid2 = fork();
    if(pid2 == 0) {
        dup2(p1[0],0);
        close(p1[0]);
        dup2(p2[1],1);
        close(p2[1]);
        close(p2[0]);
        execlp("grep","grep",argv[2],NULL);
        //error()
        exit(0);
    }
    close(p1[0]);
    close(p2[1]);

    pid3 = fork();
    if(pid3 == 0) {
        dup2(p2[0],0);
        close(p2[0]);
        execlp("wc","wc","-l",NULL);
        //error()
        exit(0);
    }
    close(p2[0]);

    while(waitpid(-1,NULL,0) > 0);
    exit(0);
}