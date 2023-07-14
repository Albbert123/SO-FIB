#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void usage() {
    char buf[100];
    sprintf(buf,"USAGE: ej1: Has de passar entre 1 i 10 argumentos. Muestra el nombre del fichero\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[80];
    perror(txt);
    exit(0);
}

int main(int argc, char*argv[]) {
    char buf[200];
    if(argc<2 || argc > 11) usage();
    int pids[argc];
    for(int i=1; i<argc; i++) {
        pids[i] = fork();
        if(pids[i] < 0) error("fork");
        else if(pids[i] == 0) {
            sprintf(buf, "mi programa a tratar es %s\n", argv[i]);
            write(1,buf,strlen(buf));
            exit(i);
        }
    }
    int stat,i=1;
    while(waitpid(pids[i],&stat,0)>0) {
        if(WIFEXITED(stat)) {
            sprintf(buf,"hijo %d muerto por exit %d\n", pids[i],WEXITSTATUS(stat));
            write(1,buf,strlen(buf));
            ++i;
        }
    } 
    exit(0);      
}

