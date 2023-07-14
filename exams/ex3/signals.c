#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

void error_y_exit (char *msg, int exit_status)
{
  perror (msg);
  exit (exit_status);
}

void Usage()
{
	char bf[256];
	sprintf(bf, "Usage:signals.c arg1 [arg2..arg10]\nEste programa muestra por pantalla un mensaje con el nombre del fichero que le ha tocado y mas cosas\n");
	write(1,bf, strlen(bf));
    exit(1);
}

char bf[256];
int recibido = 0;

void f_recibido()
{
    recibido = 1;
}

void f_sigint()
{
    sprintf(bf, "El padre ha finalizado la espera.\n");
    write(1, bf, strlen(bf));
}

int main(int argc, char* argv[])
{
    int pids[argc];
    
    int status, pid;
    sigset_t mask1; //blok i desbok signal que reprog

    if(argc < 2 || argc > 11) Usage();

    for (int i = 1; i < argc; i++)
    {
        int ret = fork();
        switch (ret)
        {
        case -1:
            error_y_exit("fork", 1);
            
        case 0:
            
            struct sigaction trat;
            sigset_t mask; //nomes pel fill

            // Bloqueamos SIGUSR1 antes de reprogramarlo
            sigemptyset(&mask1);
            sigaddset(&mask1,SIGUSR1);
            sigprocmask(SIG_BLOCK,&mask1,NULL);

            // Reprogramem SIGUSR1
            sigfillset(&mask);
            trat.sa_mask = mask;
            trat.sa_flags = 0;    
            trat.sa_handler = f_recibido;
            sigaction(SIGUSR1, &trat, NULL);

            // Lo desbloqueamos
            sigprocmask(SIG_UNBLOCK,&mask1,NULL);

            /*-----------------------------------------------------*/

            // Bloqueamos el SIGINT para que solo lo reciba el padre
            sigemptyset(&mask1);
            sigaddset(&mask1,SIGINT);
            sigprocmask(SIG_BLOCK,&mask1,NULL);

            /*-----------------------------------------------------*/
            
            //Espera activa
            while(!recibido);

            //Tractament
            sprintf(bf, "Nombre del fichero: %s\n",argv[i]);
            write(1, bf, strlen(bf));
            exit(i);

        default:
            pids[i-1] = ret;
        }  
    }

    recibido = 0;
    struct sigaction trat2;
    sigset_t mask2;

    // Bloqueamos SIGINT antes de reprogramarlo
    sigemptyset(&mask1);
    sigaddset(&mask1,SIGINT);
    sigprocmask(SIG_BLOCK,&mask1,NULL);

    // Reprogramem SIGINT
    sigfillset(&mask2);
    trat2.sa_mask = mask2;
    trat2.sa_flags = 0;    
    trat2.sa_handler = f_sigint;
    sigaction(SIGINT, &trat2, NULL);

    // Desbloqueamos SIGINT
    sigprocmask(SIG_UNBLOCK,&mask1,NULL);

    /*-----------------------------------------------------*/

    // Bloqueamos SIGALRM antes de reprogramarlo
    sigemptyset(&mask1);
    sigaddset(&mask1,SIGALRM);
    sigprocmask(SIG_BLOCK,&mask1,NULL);

    // Reprogramem SIGALRM
    trat2.sa_handler = f_recibido;
    sigaction(SIGALRM, &trat2, NULL);

    // Desbloqueamos SIGINT
    sigprocmask(SIG_UNBLOCK,&mask1,NULL);

    /*-----------------------------------------------------*/

    // Desbloquejem SIGINT i SIGALRM
    sigfillset(&mask2);
    sigdelset(&mask2, SIGINT);
    sigdelset(&mask2, SIGALRM);

    int j = 0;
    
    while(j < argc-1 && recibido == 0)
    {
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

    for (int j = 0; j < argc-1; j++)
    {
        pid = waitpid(-1, &status, 0);
        if(pid <  0) error_y_exit("waitpid", status);
        else
        {
            if (WIFEXITED(status))
            {
                sprintf(bf, "PID hijo muerto = %d // Con parametro de salida: %d\n", pid, WEXITSTATUS(status));
                write(1,bf, strlen(bf));
            }
            else if (WIFSIGNALED(status))
            {
                sprintf(bf, "PID hijo muerto = %d // Con numero de signal: %d\n", pid, WTERMSIG(status));
                write(1,bf, strlen(bf));
            }
        }
    }
}
