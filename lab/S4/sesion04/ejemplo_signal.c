#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int contador = 0;
int hijos = 0;

void error_y_exit(char *msg,int exit_status)
{
    perror(msg);
    exit(exit_status);
}

void trata_alarma(int s) 
{

}

void trata_hijo(int s) {
    int pid, exit_code;
    char buff[256]; 
    while ((pid = waitpid(-1, &exit_code, WNOHANG)) > 0) {
        if (WIFEXITED(exit_code)) {
            int statcode = WEXITSTATUS(exit_code);
            sprintf(buff,"Termina el proceso %d com exit code %d\n",pid, statcode);
        }
        else {
            int signcode = WTERMSIG(exit_code);
            sprintf(buff,"Han matado al proceso %d antes de acabar alarm por el signal %d\n",pid, signcode);
        }
        write(1, buff, strlen(buff)); 
        hijos--;
        ++contador;
    }
}


int main(int argc, char* argv[])
{
    int pid, res;
    char buff[256];
    struct sigaction sa;
    sigset_t mask;

    /* Evitamos recibir el SIGALRM fuera del sigsuspend */

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    for (hijos = 0; hijos < 10; hijos++) {
        sprintf(buff, "Creando el hijo numero %d\n", hijos);
        write(1, buff, strlen(buff));

        pid = fork();
        if (pid == 0) /* Esta linea la ejecutan tanto el padre como el hijo */
        {

            sa.sa_handler = &trata_alarma;
            sa.sa_flags = SA_RESTART;
            sigfillset(&sa.sa_mask);
            if (sigaction(SIGALRM, &sa, NULL) < 0)
                error_y_exit("sigaction", 1);

            /* Escribe aqui el codigo del proceso hijo */
            sprintf(buff, "Hola, soy %d\n", getpid());
            write(1, buff, strlen(buff));

            alarm(2);
            sigfillset(&mask);
            sigdelset(&mask, SIGALRM);
            sigdelset(&mask, SIGINT);
            sigsuspend(&mask);

            /* Termina su ejecución */
            exit(0);
        } else if (pid < 0) {
            /* Se ha producido un error */
            error_y_exit("Error en el fork", 1);
        }
    }
    /* Esperamos que acaben los hijos */
    sa.sa_handler = &trata_hijo;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);
    while (hijos > 0);
    sprintf(buff, "Valor del contador %d\n", contador);
    write(1, buff, strlen(buff));
    return 0;
}

