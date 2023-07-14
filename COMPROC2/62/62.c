#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void processa_comanda(int num_arguments, char *arguments[])
{

	arguments[num_arguments] = NULL;

	int argument_actual_inici = 0;
	int ultima_pipe = 0;
	int i = 0;

	while (i < num_arguments) {
		if (strcmp(arguments[i], "|") == 0) {
			int pipefd[2];
			pipe(pipefd);
			
			int pid = fork();
			if (pid == 0) {
				close(0);
				dup(ultima_pipe);
				close(ultima_pipe);

				dup2(pipefd[1], 1);
				close(pipefd[1]);

				arguments[i] = NULL;
				execvp(arguments[argument_actual_inici],
						&arguments[argument_actual_inici]);
				exit(1);
			}

			close(pipefd[1]);
		
			if (ultima_pipe != 0)
				close(ultima_pipe);
			ultima_pipe = pipefd[0];

			i++;
			argument_actual_inici = i;
		}

		i++;
	}

	int pid = fork();
	if (pid == 0) {
		dup2(ultima_pipe, 0);
		close(ultima_pipe);
		execvp(arguments[argument_actual_inici], &arguments[argument_actual_inici]);
		// Ha fallat
		printf("Exec falla\n");
		exit(1);
	}

	int status;
	waitpid(pid, &status, 0);

	if (WIFEXITED(status)) {
		int statret = WEXITSTATUS(status);
		printf("Comanda retorna %d\n", statret);
	}
}

int main(int argc, char *argv[])
{
	while (1) {
		printf("ubash> ");
		char *arguments[512];
		char *line = NULL;
		size_t n = 0;
		ssize_t bytes_read = getline(&line, &n, stdin);

		if (bytes_read < 0)
			return 0;

		// Remove the last newline
		line[bytes_read - 1] = '\0';

		int i = 0;
		char *tmp_line = line;

		while (tmp_line[0] != '\0') {
			while (tmp_line[0] != '\0' && tmp_line[0] == ' ')
				tmp_line++;

			if (tmp_line[0] != '\0') {
				arguments[i++] = tmp_line;

				while (tmp_line[0] != '\0' && tmp_line[0] != ' ')
					tmp_line++;

				if (tmp_line[0] != '\0') {
					tmp_line[0] = '\0';
					tmp_line++; 
				}
			}
		}

		if (i > 0)
			processa_comanda(i, arguments);

		free(line);
	}
}

