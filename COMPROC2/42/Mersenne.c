#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>

#define SIZE_BUFF 4

void error_and_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage()
{
    char msg[256];

    sprintf(msg, "Usage: Mersenne\n");
    write(2, msg, strlen(msg));
    exit(EXIT_FAILURE);
}

int main(int argc,char *argv[])
{
    int pid, fdf1, fdf2, ret, value;
    int fdp1[2], values[SIZE_BUFF];
    char buffer[5];

    if (argc != 1) usage();

    if((mknod("ff1", S_IFIFO|S_IRUSR|S_IWUSR, 0) < 0) && (errno != EEXIST))
        	error_and_exit("mknod");
    if((mknod("ff2", S_IFIFO|S_IRUSR|S_IWUSR, 0) < 0) && (errno != EEXIST))
                error_and_exit("mknod");

    if(pipe(fdp1) < 0) error_and_exit("pipe");

    pid = fork();
    switch (pid) {
      case 0:
          if(dup2(fdp1[1], 1) < 0) error_and_exit("dup2 fdp1[1]");
          if(close(0) < 0) error_and_exit("close 0");
          if(open("ff1", O_RDONLY) < 0) error_and_exit("open ff1");
          if(close(fdp1[0]) < 0) error_and_exit("close fdp1[0]");
          if(close(fdp1[1]) < 0) error_and_exit("close fdp1[1]");
          if(execlp("./arith", "arith", "2", (char*)NULL) < 0) error_and_exit("execlp");
      case -1: error_and_exit("fork");
    }

    pid = fork();
    switch (pid) {
      case 0: 
	  if(dup2(fdp1[0], 0) < 0) error_and_exit("dup2 fdp1[0]");
          if(close(1) < 0) error_and_exit("close 1");
          if(open("ff2", O_WRONLY) < 0) error_and_exit("open ff2");
	  if(close(fdp1[0]) < 0) error_and_exit("close fdp1[0]");
          if(close(fdp1[1]) < 0) error_and_exit("close fdp1[1]");
          if(execlp("./arith", "arith", "1", (char*)NULL) < 0) error_and_exit("execlp");
      case -1: error_and_exit("fork");
    }

    if(close(fdp1[0]) < 0) error_and_exit("close fdp1[0]");
    if(close(fdp1[1]) < 0) error_and_exit("close fdp1[1]");

    if((fdf1 = open("ff1", O_WRONLY)) < 0) error_and_exit("open ff1");

    while((ret = read(0, values, sizeof(values))) > 0) {
        if (write(fdf1, values, ret) < 0) error_and_exit("write fdf1");
    }
    if(ret < 0) error_and_exit("read 0");
    if(close(fdf1) < 0) error_and_exit("close fdf1");

    if((fdf2 = open("ff2", O_RDONLY)) < 0) error_and_exit("open ff2");
    while((ret = read(fdf2, &value, sizeof(value))) > 0) {
        sprintf(buffer, "%d ", value);
        if (write(1, buffer, strlen(buffer)) < 0) error_and_exit("write 1");
    }
    if(ret < 0) error_and_exit("read fdf2");
    if(close(fdf2) < 0) error_and_exit("close fdf2");

    write(1,"\n",sizeof(char));

    while ((ret = waitpid(-1, NULL, 0)) > 0);
    if (ret < 0 && errno != ECHILD) error_and_exit("waitpid");

    return 0;
}
