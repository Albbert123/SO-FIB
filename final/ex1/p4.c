#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void Usage() {

	char b[80];
	sprintf(b, "Usage: error en los argumentos\n");
	write(1,b,strlen(b));
} 

void error_y_exit (char *msg, int exit_stat) {

	perror(msg);
	exit(exit_stat);
}

int main (int argc, char *argv[]) {

	if (argc < 3) Usage();
	else {

		int fd_pipe[2];
		if (pipe(fd_pipe) == -1) error_y_exit("Fallo al crear la pipe",1); //creamos la pipe

		int n = atoi(argc[1]);
		int semilla = atoi(argc[2]);

		int ret;
		int num = semilla;

		int stat, code;
		char buff[128];

		for (int i = 0; i < n; ++i) {
			ret = fork();

			if (ret < 0) error_y_exit("error en el fork",1);

			//hijo
			else if (ret == 0) {
				dup2(fd_pipe[1],1);
				
				if (i != 0) num += code;
				sprintf(buff,"%d",num);
				execlp("./dummy2","dummy2", buff, NULL);
			}

			//padre
			else {
				
				waitpid(-1,&stat,0); //waitpid bloqueante del padre porq es esquema secuencial

				if (WIFEXITED(stat)) {
					code = WEXITSTATUS(stat);
				}
				//num += code;
			}
		}

		ret = fork();
		if (ret < 0) error_y_exit("Error en el fork\n",1);
		else (ret == 0) {
			dup2(fd_pipe[0],0);

		}

		sprintf(buff,"El total es %d\n", num);
		write(1,buff,strlen(buff));
	}
}