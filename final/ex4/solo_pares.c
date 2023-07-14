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
    int *ptr = malloc(128*sizeof(int));
    int i = 0;

    while(i<128)
    {
        if(read(0, &ptr[i],sizeof(int)) < 0) error_y_exit("Error en el read", 2);
        ++i;
    }
    
    for (int i = 0; i < 128; i+=2)
        write(1,&ptr[i],sizeof(int));
    
    free(ptr);
}