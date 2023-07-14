#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage() {
    char buf[64];
    sprintf(buf,"pr3.c USAGE: ./pr3 file1 file2\n");
    write(1,buf,stlren(buf));
    exit(0);
}

void error(char*msg) {
    char buf[64];
    perror(msg);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[264];
    if(argc!=3) usage();
    int p[2], pid1, pid2;
    pipe(p);
    pid1 = fork();
    if(pid1==-1) error("fork");
    else if(pid1 == 0) {
        int fd = open(argv[1],O_RDONLY);
        if(fd<0) error("open");
        dup2(fd,0);
        close(argv[1]);
        dup2(p[1],1);
        close(p[1]);
        close(p[0]);
        execlp("./pr1", "./pr1", NULL);
        error("execlp");
    }

    pid2 = fork();
    if(pid2 == -1) error("fork");
    else if(pid2 == 0) {
        int fd2 = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0640);
        if(fd2<0) error("open");
        dup2(p[0],0);
        close(p[0]);
        close(p[1]);
        dup2(fd2,1);
        close(fd2);
        execlp("./pr2", "./pr2", NULL);
        error("execelp");
    }

    close(p[0]);
    close(p[1]);

    while((waitpid(-1,NULL,0))>0);

    exit(0);

}
