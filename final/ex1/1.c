#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void error() {
exit(1);
}

void usage() {
exit(0);
}

int main(int argc, char*argv[]) {
    char buf[256];
    char s[50]; //semilla
    if(argc!=3) usage();
    int nump = atoi(argv[1]);
    int semilla = atoi(argv[2]);
    int pid,i,code=0;
    for(i=0; i<nump; ++i) {
        pid = fork();
        if(pid<0) error();
        if(pid == 0) {
            if(i!=0) semilla += code;
            sprintf(s,"%d",semilla);
            execlp("./dummy","./dummy",s,NULL);
            error();
        }
        int stat;
        waitpid(-1,&stat,0);

        if(WIFEXITED(stat)) {
            code = WEXITSTATUS(stat);
        }
    }
    sprintf(buf,"El total es %d\n",semilla);
    write(1,buf,strlen(buf));
}