#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int fd;

void Usage() {

	char b[80];
	sprintf(b, "Usage: error en los argumentos\n");
	write(fd,b,strlen(b));
} 

void error_y_exit (char *msg, int exit_stat) {

	perror(msg);
	exit(exit_stat);
}

void trat_alarm (int s) {

	char buf[28];
	if (s == SIGALRM) {
		sprintf(buf, "Tiempo limite\n");
		write(fd,buf,strlen(buf));
		exit(0);
	}
}

int main (int argc, char *argv[]) {

	if (argc < 4) Usage();
	else {

		char buff[128];
		int n = atoi(argc[1]);
		int num = atoi(argc[2]);
		int sec = atoi(argc[3]);

		int ret, stat, code;
		//fichero 
		fd = open(argv[4], O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);

		if (fd < 0) error_y_exit("Error en el open\n",1);
		//signals
		sigset_t mask; 
		struct sigaction sa;

		sigemptyset(&mask);
		sigaddset(&mask, SIGALRM);
		sigprocmask(SIG_BLOCK,&mask, NULL);  //lo bloqueamos para evitar que llegue antes de reprogramarlo

		sa.sa_handler = trat_alarm;
		sa.sa_flags = SA_RESTART;
		sigfillset(&sa.sa_mask); //no entiendoooo!!!!
		if (sigaction(SIGALRM, &sa, NULL) < 1) error_y_exit("error al reprogramar signal",1);

		sigprocmask(SIG_UNBLOCK, &mask, NULL); //desbloqueamos porque ya lo hemos configurado

		alarm(sec); 
		for (int i = 0; i < n; ++i) {
			ret = fork();

			if (ret < 0) error_y_exit("error en el exit",1);

			//hijo
			else if (ret == 0) {

				if (i != 0) num += code;
				sprintf(buff,"%d",num);
				execlp("dummy.c","./dummy", buff, NULL);
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

		sprintf(buff,"El total es %d\n", num);
		write(fd,buff,strlen(buff));
	}
}