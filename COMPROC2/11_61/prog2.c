#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
	execlp("wc", "wc", "-c", NULL);
	perror("Error en execlp");

	return -1;
}
