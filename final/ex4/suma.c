#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage() {
    char buff[256];
    sprintf(buff, "Usage:...");
    write(1, buff, strlen(buff));
    exit(1);
}

void error_y_exit(const char *msg, int code) {
    perror(msg);
    exit(code);
}

int main(int argc, char *argv[])
{
    if(argc < 2) usage();

    int fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP);
    
    int sum = 0, byt;
    char buff[256];
    while(byt = read(0, &buff, 1) > 0) 
    {
        sum += atoi(buff);
    }

    if(byt < 0) error_y_exit("Error en read", 2);
    
    sprintf(buff,"%d\n",sum);
    write(fd,buff,strlen(buff));

    sprintf(buff,"La suma es %d\n",sum);
    write(1,buff,strlen(buff));
}