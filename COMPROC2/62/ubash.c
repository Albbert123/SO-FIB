#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

void processa_comanda(int num_arguments, char *arguments[])
{
	// De moment, només sabem repetir els arguments
	for (int i = 0; i < num_arguments; ++i)
		printf("%s ", arguments[i]);

	printf("\n");
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

