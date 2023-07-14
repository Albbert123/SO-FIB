#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void error(char* msg) {
	perror(msg);
	exit(1);
}

//SE TIENE Q PASAR FICHERO I POSICION A DONDE SE INSERTARÀ

int main(int argc, char *argv[]) {
    char buff[32];
    sprintf(buff, "X");
    char buff2[32];
    int fd = open(argv[1], O_RDWR);
    int final = lseek(fd,0,SEEK_END);
    int i = atoi(argv[2]);
        
    while (i != final){
        lseek(fd, i, SEEK_SET); //nos colocamos donde esta el 4
        read(fd, &buff2, 1); //leemos el 4 en buf2
        lseek(fd, -1, SEEK_CUR); //posicion anterior al 4 
        write(fd, &buff, 1); //escribimos X --> 123X
        sprintf(buff, "%s", buff2); //escribimos el 4
        ++i;
    }
    close(fd);
}