#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char*argv[]) {
    char buf[128];
    int fd[2], pid1, pid2, ret;
    pipe(fd);
    pid1 = fork();
    if(pid1 == 0) {
        dup2(fd[1],1);
        close(fd[1]);
        close(fd[0]);
        execlp("./escritorv2","./escritorv2",NULL);
        //error()
        exit(0);
    }
    close(fd[1]);

    pid2 = fork();
    if(pid2==0) {
        dup2(fd[0],0);
        close(fd[0]);
        close(fd[1]);
        execlp("./lectorv2","./lectorv2",argv[1],NULL);
        //error()
        exit(0);
    }
    close(fd[0]);

    while(waitpid(-1,NULL,0)>0);
} 
