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
    int *ptr = malloc(128*sizeof(int));
    
    for(int i=0; i<128; ++i) {
        if(read(0, &ptr[i], sizeof(int))<0) error("read");
    }

    for(int i=0; i<128; i+=2) {
        write(1,&ptr,sizeof(int));
    }

    free(ptr);
}