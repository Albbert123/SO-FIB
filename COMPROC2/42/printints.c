#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_y_exit(char *msg)
{
    perror(msg);
    exit(1);
}

void usage()
{
    char msg[256];

    sprintf(msg, "Usage: showints\n");
    write(2, msg, strlen(msg));
    exit(1);
}

int main(int argc, char *argv[])
{
        int f,value,ret;
	char buffer[5];

        if (argc != 1)  usage();

	ret=read(0,&value,sizeof(value));
	while(ret > 0) {
		sprintf(buffer, "%d ",value);
		if (write(1,buffer,strlen(buffer)) < 0) 
			error_y_exit("write");
		ret=read(0,&value,sizeof(value));
        }
	if(ret < 0) error_y_exit("read");

	write(1,"\n",sizeof(char));

	return 0;
}
