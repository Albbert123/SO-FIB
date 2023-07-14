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
    int fd, ret;
    fd = open("mipipe",O_WRONLY);
    if(fd < 0 && errno == ENOENT) {
        mknod("mipipe",S_IFIFO|0660,0);
        fd = open("mipipe",O_WRONLY);
    }

    int num;
    while(ret = read(0,buf,sizeof(buf))>0) {
        buf[ret] = "/0";
        num = atoi(buf); //de ascii a formato interno
        write(fd,&num,sizeof(num));
    }
    close(fd);
} 
