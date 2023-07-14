#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 32

void error_y_exit(char *msg) {
    perror(msg);
    exit(-1);
}
void usage() {
    char buff[BUFSIZE];
    sprintf(buff,"Usage: ./suma_enters <file>\n");
    write(1,buff,strlen(buff));
    exit(-1);
}

int main(int argc, char *argv[]) {
    int ret, fd, fdout, num, suma;
    char buff[BUFSIZE];

    if (argc!=2) usage();

    fdout = open("suma.txt", O_CREAT|O_WRONLY|O_TRUNC, 0600);
    if (fdout == -1) error_y_exit("Error open suma.txt\n");

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) error_y_exit("Error open firts parameter\n");
    suma = 0;

    while(ret = read(fd, &num, sizeof(int))) {
        suma +=num;
    }

    sprintf(buff, "%d\n", suma);
    write(1, buff, strlen(buff));
    write(fdout, buff, strlen(buff));

    close(fd);
    close(fdout);

    return 0;
}

