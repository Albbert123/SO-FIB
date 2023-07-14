#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void error(char*msg){
    perror(msg);
    exit(1);
}

int main() {
    char buf[256];
    int f = creat("salida.txt",O_CREAT|O_TRUNC|0600);
    if(f==-1) error("error creat");
    sprintf(buf,"ABCD");
    write(f,buf,strlen(buf));
}
