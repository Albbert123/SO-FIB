#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void error_y_exit(char *missatge, int exit_st) {
	perror(missatge);
	exit(exit_st);
}

/* Ejecuta el comando ps –u username mediante la llamada al sistema execlp */
/* Devuelve: el código de error en el caso de que no se haya podido mutar */
void muta_a_PS(char *username) {
	execlp("ps", "ps", "-u", username, (char*)NULL);
	error_y_exit("Ha fallado la mutación al ps", 1);
}

int main(int argc, char *argv[]) {
	int pid = fork();
	char buffer[80];

	if (argc != 2) error_y_exit("Error debes especificar al menos un parametro", -1);
	if (pid == -1) error_y_exit("Error del proceso hijo", -1);

	sprintf(buffer, "Soy el proceso: %d\n", getpid());
	write(1,buffer,strlen(buffer));

	if (pid == 0){
		sprintf(buffer, "Parametros: %s\n",argv[1]);
		write(1,buffer,strlen(buffer));	
		muta_a_PS(argv[1]);
	}

	while(1);
}