#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_SIZE 8

void usage();
// Mostra missatge de l'utilització del programa

unsigned int char2int(char c);
// Converteix caràcter a número (int)

int mi_atoi(char *s);
// Converteix cadena de caràcters a número (int)

int esNumero(char *str);
// Comprova que la cadena de caràcters que entra és un número