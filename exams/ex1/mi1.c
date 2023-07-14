#include<stdlib.h>
#include<wait.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

int n1;
int n2;
int cont;

void usage() {
    char buf[120];
    sprintf(buf,"USAGE; mi1.c: Has de passar dos parametres enters, essent el segon major que 0\n");
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
    if(s == SIGALRM) {
        ++n1;
        ++cont;
        sprintf(buf,"%d\n",n1);
        write(1,buf,strlen(buf));
    }
    else if(s == SIGUSR1) {
        ++cont;
        int dif = n2-cont;
        sprintf(buf,"RESTAN POR ACABAR %d SEGUNDOS\n",dif);
        write(1,buf,strlen(buf));
    }
    else if(s == SIGINT) {
        sprintf(buf,"numeros mostrats: %d \n", cont);
        write(1,buf,strlen(buf));
        exit(cont);
    }
}

int main(int argc, char*argv[]) {
    char buf[120];
    if(argc != 3) usage();
    n1 = atoi(argv[1]);
    n2 = atoi(argv[2]);
    if(n2<0) usage();
    sprintf(buf,"n1 = %d\n",n1);
    write(1,buf,strlen(buf));
    
    struct sigaction sa;
    sigset_t mask;

    //NOS BLOQUEAMOS
    sigemptyset(&mask);
    sigaddset(&mask,SIGALRM);
    sigaddset(&mask,SIGUSR1);
    sigaddset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    //REPROGRAMAMOS SIGALARM i SIGUSR1
    sa.sa_handler = &trat;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);

    if(sigaction(SIGALRM,&sa,NULL) < 0) error("sigaction");
    if(sigaction(SIGUSR1,&sa,NULL) < 0) error("sigaction");
    if(sigaction(SIGINT,&sa,NULL) < 0) error("sigaction");

    //espero un evento desblokeandolos
    sigfillset(&mask);
    sigdelset(&mask,SIGALRM);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGINT);

    while(cont < n2) {
        alarm(1); //alarma 1 segundo
        sigsuspend(&mask);
    }
    sprintf(buf,"numeros mostrats: %d \n", cont);
    write(1,buf,strlen(buf));
    exit(cont);
    //trat(SIGINT);
}