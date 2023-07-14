#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<wait.h>
#include <sys/types.h>
#include <sys/wait.h>

void usage() {
    char buf[80];
    sprintf(buf,"usage\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*msg) {
    char buf[80];
    perror(msg);
    exit(1);
}

int main(int argc, char*argv[]) {
    char buf[280];
    if(argc != 3) usage();

    int n = atoi(argv[1]);
    int i,pids[n];
    int fexit = 0;

    for(i=0; fexit=0 && i<n; ++i) {
        pids[i] = fork();
        if(pids[i] < 0) error("fork");

        else if(pids[i] == 0) {
            execlp("./hijos","./hijos",argv[2],NULL);
            error("exec");
        }

        int stat1;
        waitpid(-1,stat1,NULL);
        if(WIFEXITED(stat1)) {
            if(WEXITSTATUS(stat1) == 0) {
                sprintf(buf,"pid %d ended correctly\n",pids[i]);
                write(1,buf,strlen(buf));
                for(int i=0; i<n; ++i) {
                    kill(pids[i],SIGKILL);
                }
                fexit = 1;
            }
            else {
                sprintf(buf,"pid %d ended by exit\n",pids[i]);
                write(1,buf,strlen(buf));
            }
        }
        else if(WIFSIGNALED(stat1)) {
            sprintf(buf,"pid %d ended by signal\n",pids[i]);
            write(1,buf,strlen(buf));
        }
    }
    
    int stat2;
    while(waitpid(-1,NULL,0)>0) {
        if(WIFEXITED(stat2)) {
            if(WEXITSTATUS(stat2) == 0) {
                sprintf(buf,"pid %d ended correctly\n",pids[i]);
                write(1,buf,strlen(buf));
                //exit(0);
                for(int i=0; i<n; ++i) {
                    kill(pids[i],SIGKILL);
                }
                fexit = 1;
            }
            else {
                sprintf(buf,"pid %d ended by exit\n",pids[i]);
                write(1,buf,strlen(buf));
            }
        }
        else if(WIFSIGNALED(stat2)) {
            sprintf(buf,"pid %d ended by signal\n",pids[i]);
            write(1,buf,strlen(buf));
        }
    }
    if(fexit == 0) {
        sprintf(buf,"incalculable\n");
        write(1,buf,strlen(buf));
    }
    exit(0);
}
