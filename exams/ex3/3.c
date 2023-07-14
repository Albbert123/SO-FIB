#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int recibido=0;

void usage() {
    char buf[100];
    sprintf(buf,"USAGE: ej1: Has de passar entre 1 i 10 argumentos. Muestra el nombre del fichero\n");
    write(1,buf,strlen(buf));
    exit(0);
}

void error(char*txt) {
    char buf[80];
    perror(txt);
    exit(1);
}

void trat(int s) {
    char buf[120];
    if(s==SIGUSR1) recibido=1;
    else if(s==SIGINT) {
        sprintf(buf,"Salimos de la espera por SIGINT\n");
        write(1,buf,strlen(buf));
    }
}

int main(int argc, char*argv[]) {
    char buf[200];
    if(argc<2 || argc > 11) usage();
    int pids[argc];

    struct sigaction sa;
    sigset_t mask;

    //BLOK
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1);
    sigaddset(&mask,SIGINT);
    sigaddset(&mask,SIGALRM);
    sigaddset(&mask,SIGKILL);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    //REP SIGUSR1
    sa.sa_handler=trat;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);

    if(sigaction(SIGUSR1,&sa,NULL) < 0) error("sigaction");
    if(sigaction(SIGINT,&sa,NULL) < 0) error("sigaction");
    if(sigaction(SIGALRM,&sa,NULL) < 0) error("sigaction");
    //if(sigaction(SIGKILL,&sa,NULL) < 0) error("sigaction");

    for(int i=1; i<argc; i++) {
        int ret = fork();
        if(ret < 0) error("fork");
        else if(ret == 0) {
            sigset_t mask1;
            //BLOK SIGUSR1
            sigemptyset(&mask);
            sigaddset(&mask,SIGUSR1);
            sigprocmask(SIG_BLOCK,&mask,NULL);

            //REP SIGUSR1
            sa.sa_handler=trat;
            sa.sa_flags = SA_RESTART;
            sigfillset(&mask1);
            sa.sa_mask = mask1;
            if(sigaction(SIGUSR1,&sa,NULL) < 0) error("sigaction");

            //DESBLOK SIGUSR1
            sigprocmask(SIG_UNBLOCK,&mask,NULL);

            //BLOK SIGINT para que solo lo reciba el padre
            sigemptyset(&mask);
            sigaddset(&mask,SIGINT);
            sigprocmask(SIG_BLOCK,&mask,NULL);

            while(!recibido);
            sprintf(buf, "mi programa a tratar es %s\n", argv[i]);
            write(1,buf,strlen(buf));
            exit(i);
        }
        else pids[i-1] = ret;
    }

    recibido = 0;

    struct sigaction trat2;
    sigset_t mask2;

    // Bloqueamos SIGINT antes de reprogramarlo
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    // Reprogramem SIGINT
    sigfillset(&mask2);
    trat2.sa_mask = mask2;
    trat2.sa_flags = 0;    
    trat2.sa_handler = trat;
    sigaction(SIGINT, &trat2, NULL);

    // Desbloqueamos SIGINT
    sigprocmask(SIG_UNBLOCK,&mask,NULL);

    // Desbloquejem SIGINT i SIGALRM
    sigfillset(&mask2);
    sigdelset(&mask2, SIGINT);
    sigdelset(&mask2, SIGALRM);

    int j=0;

    while(j<argc-1 && recibido==0) {
        alarm(2);
        sigsuspend(&mask2);
        kill(pids[j], SIGUSR1);
        ++j;
    }

    // Si hemos salido por SIGALRM
    if(recibido == 1)
    {
        for(int k = 0; k < argc-1; ++k) kill(pids[k], SIGKILL);
    }

    int status,pid;
    for (int j = 0; j < argc-1; j++)
    {
        pid = waitpid(-1, &status, 0);
        if(pid <  0) error_y_exit("waitpid", status);
        else
        {
            if (WIFEXITED(status))
            {
                sprintf(buf, "PID hijo muerto = %d // Con parametro de salida: %d\n", pid, WEXITSTATUS(status));
                write(1,buf, strlen(buf));
            }
            else if (WIFSIGNALED(status))
            {
                sprintf(buf, "PID hijo muerto = %d // Con numero de signal: %d\n", pid, WTERMSIG(status));
                write(1,buf, strlen(buf));
            }
        }
    }
}
