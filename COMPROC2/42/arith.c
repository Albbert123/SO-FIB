#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_and_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage()
{
    char msg[256];

    sprintf(msg, "Usage: arith n, 0 < n < 3\n");
    write(2, msg, strlen(msg));
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int n, value, ret;
    char *ptr;

    if (argc != 2)  usage();

    //n = atoi(argv[1]);
    n = (int)strtol(argv[1], &ptr, 10);

    if ((*ptr != '\0') || (n < 1) || (n > 2)) usage();

    while((ret = read(0, &value, sizeof(value))) > 0) {
	switch(n) {
	    case 1: value = value - 1;
		    break;
	    case 2: value = 1 << value;
		    break;
	}
	if (write(1, &value, ret) < 0) error_and_exit("write");
    }
    if (ret < 0) error_and_exit("read");

    return 0;
}
