#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void trat_a(int s) {
    char buf[80];
    sprintf(buf,"tiempo limite\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*msg) {
    perror(msg);
exit(1);
}

void usage() {
    char buf[80];
    sprintf(buf,"usage\n");
    write(1,buf,strlen(buf));
exit(0);
}

int main(int argc, char*argv[]) {
    char buf[256];
    char s[50]; //semilla
    if(argc!=4) usage();
    int nump = atoi(argv[1]);
    int semilla = atoi(argv[2]);
    int maxseg = atoi(argv[3]);
    int pid,i,code=0;

    struct sigaction sa;
    sigset_t mask;
    //blok
    sigemptyset(&mask);
    sigaddset(&mask,SIGALRM);
    sigprocmask(SIG_BLOCK,&mask, NULL);

    //repro
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &trat_a;
    sigfillset(&sa.sa_mask);
    if(sigaction(SIGALRM,&sa,NULL) < 0) error("s");

    //desblok
    sigfillset(&mask);
    sigdelset(&mask,SIGALRM);

    //sigprocmask(SIG_UNBLOCK, &mask, NULL);
    alarm(maxseg);
    //sigsuspend(&mask);

    for(i=0; i<nump; ++i) {
        pid = fork();
        if(pid<0) error("f");
        if(pid == 0) {
            if(i!=0) semilla += code;
            sprintf(s,"%d",semilla);
            execlp("./dummy","./dummy",s,NULL);
            error("e");
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