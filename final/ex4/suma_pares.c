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
    if(argc < 2) usage();

    int fd[2];
    pipe(fd);

    int pid = fork();
    if(pid < 0) error_y_exit("Error en el fork", 2);
    if(pid == 0)
    {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);

        int lec = open(argv[1],O_RDONLY, S_IRWXU);
        dup2(lec, 0);
        close(lec);

        execlp("./solo_pares", "solo_pares", NULL);
        error_y_exit("Error en execlp", 2);
    }

    pid = fork();
    if(pid < 0) error_y_exit("Error en el fork", 2);
    if(pid == 0)
    {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execlp("./suma", "suma", argv[2], NULL);
        error_y_exit("Error en execlp", 2);
    }

    close(fd[0]);
    close(fd[1]);
    while(pid = waitpid(-1,NULL, 0) > 0);
    if(pid<0) error_y_exit("Error en waitpid", 2);
}
