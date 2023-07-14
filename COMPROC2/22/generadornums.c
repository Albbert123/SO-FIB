#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i;
    for (i=1;i<=10;i++){
        write(1,&i,sizeof(int));
    }

}
