#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mis_funciones.h"

int main(int argc, char *argv[]) {
	if (argc == 1) usage();
	char buf[80];
	int res = 0;
	for (int i = 1; i < argc; ++i) {
		if (esNumero(argv[i]) == 1) {
			sprintf(buf, "Error: el parámetro %s no es un número\n", argv[i]); // %s perque char *argv es un string 
			write(1, buf, strlen(buf));
		} else {
			res += mi_atoi(argv[i]);
		}
		sprintf(buf, "La suma es %d\n", res);
		write(1, buf, strlen(buf));
	}
	return 0;
}