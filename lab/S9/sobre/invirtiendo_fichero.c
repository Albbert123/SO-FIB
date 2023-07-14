#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void error(char* msg) {
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[]) {
	char buff[32];
    char final[32];

    int f1 = open(argv[1], O_RDONLY);
    int f2 = open(strcat(argv[1] , ".inv"), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    int fi = lseek(f1, -1, SEEK_END); // fi size, f1 final
    lseek(f2, 0, SEEK_SET); // f2 inicio

    for(int i=0; i<=fi; ++i)
    {
        read(f1, buff, 1);
        lseek(f1, -2, SEEK_CUR); 
        write(f2, buff, 1);      
    }
    close(f1);
    close(f2);
}