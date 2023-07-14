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
    int fd, ret, f;

    fd = open("mipipe",O_WRONLY);
    f = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0660);

    int num;
    while(ret = read(fd,&num,sizeof(num))>0) {
        sprintf(buf,"%d\n",num); //de formato interno a ascii
        write(f,buf,strlen(buf));
    }
    close(fd);
} 
