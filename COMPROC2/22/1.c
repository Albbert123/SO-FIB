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
    //usage)()
    int fdout,fd,ret,num;

    fdout = open("addition.txt",O_WRONLY|O_CREAT|O_TRUNC,0600);
    //error()

    fd = open(argv[1],O_RDONLY);
    //error()

    int suma = 0;
    while(ret = read(fd,&num,sizeof(int)) > 0) {
        suma += num;
    }

    sprintf(buf,"%d",suma);
    write(1,buf,strlen(buf));
    write(fdout,buf,strlen(buf));

    close(fd);
    close(fdout);

    exit(0);
}