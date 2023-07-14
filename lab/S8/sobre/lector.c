//Crea pipe con nombre
//leerá de la pipe hasta que la lectura le indique que no quedan más datos para leer y
//mostrará en salida estándar todo lo que vaya leyendo

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
    int fp = open("pipeAB",O_RDONLY | O_CREAT);
    if(fp == -1) error("error en crear pipe con nombre");
    while(read(fp,&c,sizeof(char)) > 0) {
        write(1,&c,sizeof(char));
    }
    close(fp);
}