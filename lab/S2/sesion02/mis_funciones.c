#include "mis_funciones.h"

unsigned int char2int(char c) {
	return c - '0';
}

int mi_atoi(char *s) {
	int n = 0;
	int neg = 0;
	if (*s == '-') {
		++s;
		neg = 1;
	}
	while (*s != '\0') {
		n += char2int(*s);
		n *= 10;
		++s;
	}
	n /= 10;

	if (neg) return -n;
	else return n;
}

void usage(){
	char buf[110];
	sprintf(buf, "Usage:listaParametrosarg1 [arg2..argn]\nEste programa escribe por su salida la lista de argumentos que recibe\n");
	write(1, buf, strlen(buf));
}

int esNumero(char *str) { 
	if (str == NULL) return 0;
	if (*str == '-') ++str; // negatiu?
	for (int i = 0; str[i] != '\0'; i++) {
		if (!(str[i] >= '0' && str[i] <= '9') || (i >= MAX_SIZE)) return 0;
	}
	return 1;
}