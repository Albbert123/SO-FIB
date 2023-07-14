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
    char buf2[256];

    int f = open("salida.txt",O_RDWR);
    if(f==-1) error("error open");

    //leemos ultimo caracter
    lseek(f,-1,SEEK_END);
    read(f,buf,sizeof(char));

    //posicion anterior al ultimo añadimos X
    lseek(f,-1,SEEK_CUR);
    sprintf(buf2,"X");
    write(f,buf2,strlen(buf2)); // -> ABCX

    //se añade el ultimo caracter de nuevo
    write(f,buf,strlen(buf)); // -> ABCXD
}