#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

int N;
int segs;
int pid;

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

void mi_func(int s)
{
    // ESCRIBO SEGUNDOS RESTANTES
    if(s == SIGUSR1)
    {
        char buff[256];
        sprintf(buff, "Llevamos %d segundos\n", segs);
        write(1,buff,strlen(buff));
    }
    else if(s == SIGALRM)
    {
        ++s;
        if(s == N)
        {
            int code;
            if(kill(pid, SIGKILL) < 0) error_y_exit("Error en kill\n", 2);
            if(waitpid(pid, &code, 0) < 0) error_y_exit("Error en waitpid\n", 2);
            exit(1);
        }   
    }
    else if(s == SIGCHLD)
    {
        int code;
        if(kill(pid, SIGKILL) < 0) error_y_exit("Error en kill\n", 2);
        if(waitpid(pid, &code, 0) < 0) error_y_exit("Error en waitpid\n", 2);
        exit(1);
    }
}

int main(int argc, char *argv[]) 
{
    char buff[256];
    N = atoi(argv[1]);
    
    sigset_t mask;
    struct sigaction sa;

    // BLOQUEO SIGNALS QUE VOY A REPROGRAMAR
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGCHLD);
    if(sigprocmask(SIG_BLOCK,&mask, NULL) < 0) error_y_exit("Error en sigprocmask\n",2);

    // REPOGRAMO LOS 3 SIGNALS
    sigfillset(&sa.sa_mask);
    sa.sa_handler = &mi_func;
    if(sigaction(SIGALRM, &sa,NULL) < 0) error_y_exit("Error en sigaction\n",2);
    if(sigaction(SIGUSR1, &sa,NULL) < 0) error_y_exit("Error en sigaction\n",2);
    if(sigaction(SIGCHLD, &sa,NULL) < 0) error_y_exit("Error en sigaction\n",2);
    
    // LOS DESBLOQUEO
    if(sigprocmask(SIG_UNBLOCK,&mask, NULL) < 0) error_y_exit("Error en sigprocmask\n",2);

    // BLOQUEO TODOS LOS SIGNALS EXCEPTO LOS 3 QUE QUIERO RECIBIR
    sigfillset(&mask);
    sigdelset(&mask, SIGALRM);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGCHLD);
    if(sigprocmask(SIG_BLOCK,&mask, NULL) < 0) error_y_exit("Error en sigprocmask\n",2);

    pid = fork();
    if(pid == 0)
    {
        // RESTAURO MASCARA Y MUTO
        if(sigprocmask(SIG_UNBLOCK,&mask, NULL) < 0) error_y_exit("Error en sigprocmask\n",2);
        sprintf(buff,"./%s", argv[2]);
        execlp(buff,argv[2],NULL);
        error_y_exit("Error en execlp\n", 2);
    }
    else if(pid == -1) error_y_exit("Error en fork\n", 2);

    // CADA 1 SEGUNDO ESPERO ALARMA
    while(segs < N)
    {
        alarm(1);
        if(sigsuspend(&mask) < 0) error_y_exit("Error en sigsuspend\n",2);
        ++segs;
    }

    exit(1);
}
