#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv){	
	int pd[2], i, npd, len, pid;
	char buf[256];

	if (argc < 2){//Error
		return -1;
	}

	if (pipe(pd)<0){
		perror("Error en pipe");
		return -1;
	}
	
	switch(fork()){//Hijo N+1
		case -1://Error
			perror("Error en primer fork");
			return -1;
		case 0://Hijo
			if ((mknod("MIPIPE", S_IFIFO|0644, 0) < 0)&&(errno != EEXIST)){
				perror("Error en mknod");
				return -1;
			}
			dup2(pd[0], 10); close(pd[0]); close(pd[1]);
			if ((npd = open("MIPIPE", O_WRONLY)) < 0){
				perror("Error en open MIPIPE");
				return -1;
			}
			dup2(npd, 20); close(npd);
			execlp("./micat", "./micat", NULL);
			perror("Error en execlp");
			return -1;
	}
	
	for(i=1; i<argc; i++){
		pid = fork();
		switch(pid){
			case -1://Error
				perror("Error en fork en bucle");
				return -1;
			case 0://Hijo
				dup2(pd[1], 1); close(pd[0]); close(pd[1]);
				execlp("cat", "cat", argv[i], NULL);
				perror("Error en execlp cat");
				return -1;
			default://Padre
				if (waitpid(pid, NULL, 0) != pid){
					perror("Error en waitpid de bucle");
					return -1;
				}
		}
	}

	close(pd[0]); close(pd[1]);
	if (waitpid(-1, NULL, 0)<0){
		perror("Error en ultimo waitpid");
		return -1;
	}
	len = sprintf(buf, "Fin padre\n");
	write(1, buf, len);

	return 0;
}
