#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage() {
    char buf[80];
    sprintf(buf,"usage\n");
    wrie(1,buf,strlen(buf));
    exit(0);
}

void error(char*msg) {
    char buf[80];
    perror(msg);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[264];
    if(argc!=3) usage();
    int p[2],pid1,pid2;
    pipe(p);
    pid1 = fork(); 
    if(pid1 == 0) {
        int fd = open(argv[1],O_RDONLY);
        dup2(fd,0);
        dupe(p[1],1);
        close(fd);
        close(p[0]);
        close(p[1]);
        execlp("./solo_pares","./solo_pares",NULL);
    }
    pid2 = fork();
    if(pid2 == 0) {
        dup2(p[0],0);
        close(p[0]);
        close(p[1]);
        execlp("./suma","./suma",argv[2],NULL);
    }
    close(p[0]);
    close(p[1]);

}