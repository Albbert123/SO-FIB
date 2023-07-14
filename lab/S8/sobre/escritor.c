//Crea pipe con nombre
//leerá de la entrada estándar hasta que la lectura le indique que no quedan datos y escribirá en la pipe todo
//lo que vaya leyendo

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void error(char*msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    char c;
    int fp = open("pipeAB",O_WRONLY | O_CREAT);
    if(fp == -1) error("erro en crear pipe");
    while(read(0,&c,sizeof(char)) > 0) {
        write(fp,&c,sizeof(char));
    }
    close(fp);
}