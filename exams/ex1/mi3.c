#include<stdlib.h>
#include<wait.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

#define MAXPROC 20
int pidh[MAXPROC];

int pid,nprocs,stat,recibida=0;


void usage() {
    char buf[80];
    sprintf(buf,"USAGE mi3.c: Has de passar un parametre enter\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[80];
    perror(txt);
    exit(1);
}

void trat(int s) {
    if(s==SIGALRM) {
        int j;
        recibida = 1;

        for (j=0;j<nprocs;j++) {
            if (pidh[j] != -1) kill(pidh[j], SIGKILL);
        }
    }
}

int buscar_pid(int pidh[],int ret){
    //recorrer pidh y encontrar ret para ponerle -1
}

int main(int argc, char*argv[]) {
    char buf[120];
    char b1[50];//para el 2o parametro
    char b2[50];//para el 1r parametro
    if(argc!=2) usage();
    nprocs = atoi(argv[1]);
    
    int i;

    struct sigaction sa;
    sigset_t mask;

     //NOS BLOQUEAMOS
    sigemptyset(&mask);
    sigaddset(&mask,SIGALRM);
    sigaddset(&mask,SIGCHLD);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    //REPROGRAMAMOS SIGALARM
    sa.sa_handler = &trat;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);

    if(sigaction(SIGALRM,&sa,NULL) < 0) error("sigaction");
    if(sigaction(SIGCHLD,&sa,NULL) < 0) error("sigaction");

    sprintf(b1,"%d",nprocs);//de num a string, 2o param, nproc
    for(i=0; i<nprocs; i++) {
        sprintf(b2,"%d",i);//de num a string, 1r param
        pidh[i] = fork();
        if(pidh[i]<0) error("error fork");

        if(pidh[i]== 0) {
            execlp("./mi1", "./mi1", b2, b1, NULL);
            error("execlp");
        }
    }

    int ret;
    int esperados=0,j,alarma_desactivada=0,encontrado;
    //espero un evento
    sigfillset(&mask);
    sigdelset(&mask,SIGALRM);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGINT);
    while (esperados < nprocs) {
        if (!alarma_desactivada) {
            alarm(nprocs);
            sigsuspend(&mask);
        }
        ret=waitpid(-1,&stat,0);
        if (recibida) alarma_desactivada=1;
        else {
	  	    alarm(0);
            encontrado = 0;
            j = 0;
            while ((!encontrado) && (j<nprocs)) {
                if (pidh[j] == ret){
                    pidh[j] = -1;
                    encontrado=1;
                }
                j++;
            }
            esperados++;
            if(WIFEXITED(stat)){
                sprintf(buf,"Termina proceso %d por exit code %d \n", getpid(), WEXITSTATUS(stat));
                write(1,buf,strlen(buf));
            }
            else {
                sprintf(buf,"Han matado al proceso %d por el signal %d \n", getpid(), WTERMSIG(stat));
                write(1,buf,strlen(buf));
            } 
        }

    }
}