#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 128

void error_y_exit(char *msg) {
    perror(msg);
    exit(1);
}
void usage() {
    char buff[BUFSIZE];
    sprintf(buff,"Usage: ./count_lines <filename> <word>\n");
    write(1,buff,strlen(buff));
    exit(1);
}

int main(int argc, char *argv[]) {
    int pid, fd1[2], fd2[2], ret;
    char buff[BUFSIZE];
    char buff2[BUFSIZE];

    if (argc!=3) usage();

    // pipe1: between cat and grep
    pipe(fd1);
    pid = fork();
    if (pid == 0) {
        //first child - cat

        // redirect stdout to the write end of pipe1
        close(1);
        dup(fd1[1]);

        // close all original channels from pipe1
        close(fd1[0]);
        close(fd1[1]);

        execlp("cat", "cat", argv[1], (char*) NULL);
    }

    // close write end of pipe1 - noone else will need it
    close(fd1[1]);

    // pipe2: between grep and wc
    pipe(fd2);
    pid = fork ();
    if (pid == 0) {
        // second child - grep

        // redirect stdin to the read end of pipe1
        close(0);
        dup(fd1[0]);
        // redirect stdout to the write end of pipe2
        close(1);
        dup(fd2[1]);

        // close all original channels from pipe1 AND pipe2
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);

        execlp("grep", "grep", argv[2], (char*) NULL);
    }

    // close read end of pipe1 and write end of pipe2 - noone else needs them
    close(fd1[0]);
    close(fd2[1]);

    pid = fork ();
    if (pid == 0) {
        // third child - wc -l

        // redirect stdin to the read end of pipe2
        close(0);
        dup(fd2[0]);

        // close remaining original pipe2 channels - read end of pipe2
        close(fd2[0]);

        execlp("wc", "wc", "-l", (char*) NULL);
    }

    // close remaining original pipe2 channels - read end of pipe2
    close(fd2[0]);


    while(waitpid(-1, NULL, 0) > 0);

    return 0;
}
