/*
cree una pipe sin nombre y a continuación un proceso hijo, cuyo canal de entrada estándar deberá estar asociado al extremo
de lectura de la pipe. El proceso hijo deberá mutar su imagen para pasar a ejecutar el comando ‘cat’ visto en el trabajo
previo. 
Por su parte, el proceso padre enviará a través de la pipe el mensaje de texto “Inicio” a su hijo, cerrará el canal de 
escritura de la pipe y se quedará a la espera de que el hijo acabe.
Cuando eso suceda, el padre mostrará el mensaje “Fin” por la salida estándar y acabará la ejecución.
*/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<wait.h>

void error(char*msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    char buf[256];
    int fd[2];
    if(pipe(fd) == -1) error("error creando pipe");
    int pid = fork();

    if(pid == 0) { //hijo
        dup2(fd[0],0); //canal de entrada estándar de hijo asociado al extremo de lectura de la pipe
        close(fd[0]);
        close(fd[1]);
        execlp("cat","cat",NULL);
        error("error en execlp");
    }
    else { //padre
        sprintf(buf,"inici\n");
        write(fd[1],buf,strlen(buf));
        close(fd[1]);
        close(fd[0]);
        waitpid(-1,NULL,0);
        sprintf(buf,"fin\n");
        write(1,buf,strlen(buf));
    }
}