#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
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

void trat_alarm(int s) {
	char buff[28];
	if (s == SIGALRM) {
		sprintf(buff,"Tiempo límite\n");
		write(1,buff,strlen(buff));
		exit(1);
	}
}

int main(int argc, char *argv[]) {

	if (argc < 4) Usage();
	else {

		int n = atoi(argc[1]);	
		int semilla = atoi(argc[2]);

		int ret;
		int num = semilla;

		int stat, code;
		char buff[128];


		struct sigaction sa;
		sigset_t mask;

		sigempty(&mask);
		sigaddset(&mask, SIGALRM);
		sigprocmask(SIG_BLOCK,&mask,NULL); //bloqueamos

		sigfillset(&sa.sa_mask);
		sa.sa_handler = &trat_alarm;
		sa.sa_flags = 0;

		sigaction(SIGALARM,&sa,NULL); 
		sigprocmask(SIG_UNBLOCK,&mask,NULL);//desbloqueamos

		alarm(argv[3]);
		
		for (int i = 0; i < n; ++i) {
			ret = fork();

			if (ret < 0) error_y_exit("error en el exit",1);

			//hijo
			else if (ret == 0) {

				if (i != 0) num += code;
				sprintf(buff,"%d",num);
				execlp("./dummy","dummy", buff, NULL);
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
		write(1,buff,strlen(buff));
	}
}