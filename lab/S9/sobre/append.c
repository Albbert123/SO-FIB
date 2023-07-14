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


int main() {
    char buf[256];
    int f1 = open("file.txt",O_RDONLY);
    int f2 = open("file.txt",O_WRONLY);
    if((f2 || f1) == -1) error("Ha fallado open");

    lseek(f1,0,SEEK_SET); // f1 inicio
    int f3 = lseek(f2,0,SEEK_END); //size i f2 al final

    while(f3 != 0) {
        read(f1,buf,1); //leemos del inicio i avanzamos
        write(f2,buf,1); //escribimos al final
        --f3; //reducimos 1 el size
    }
    close(f1);
    close(f2);
}