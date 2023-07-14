#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void error_y_exit (char *msg,int exit_status)
{
	perror(msg);
	exit(exit_status);
}

void usage()
{
        char buff[256];

        sprintf(buff, "Usage: actualizar_fecha fichero_1 .. fichero_n\n");
        write(1, buff, strlen(buff));
        exit(1);
}

int main (int argc, char *argv[])
{
	int i, j, pid;
	char buff[256];
	int pids[5];

	if (argc < 2)
		usage();

	for (i = 1; i < argc; i++) {
		
		pids[i-1] = fork();

		if (pids[i-1] == 0) {
			execlp("touch", "touch", argv[i], (char *)0);
			error_y_exit("execlp", 1);
		}

		else if (pid < 0)
			error_y_exit("fork", 1);
		
	}

	for (i = 1; i < argc; i++) {
		if ((pid = waitpid(-1, NULL, 0)) < 0)
			error_y_exit("waitpid", 1);
		
		j = 0;
		while (pid != pids[j]) j++;

		sprintf(buff, "Fecha %s actualizada por %d\n", argv[j+1], pid);
		write(1, buff, strlen(buff));
	}
}
