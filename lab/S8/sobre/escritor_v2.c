//Este programa al intentar abrir la pipe, si no hay ningún lector de la pipe, mostrará un mensaje por la salida
//estándar que indique que se está esperando a un lector y a continuación se bloqueará en el open
//de la pipe hasta que un lector abra la pipe para leer.

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


void error(char*msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    char buf[256];
    char c;
    int fp;
    while((fp = open("pipeAB", O_WRONLY | O_NONBLOCK)) == -1 && errno == ENXIO) {
        sprintf(buf, "Waiting for a reader...\n");
		write(1, buf, strlen(buf));
		sleep(3);
    }
    sprintf(buf, "Reader found!\n");
	write(1, buf, strlen(buf));

    while(read(0,&c,sizeof(char)) > 0) {
        write(fp,&c,sizeof(char));
    }
    close(fp);
}