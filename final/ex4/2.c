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
    if(argc!=2) usage();
    int fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0640);
    int sum=0;
    int ret;
    
    while(ret = read(fd,buf,1)>0) {
        sum += atoi(buf);
    }

    sprintf(buf,"%d",sum);
    write(fd,buf,strlen(buf));

    sprintf(buf,"la suma es %d\n",sum);
    write(1,buf,strlen(buf));
}