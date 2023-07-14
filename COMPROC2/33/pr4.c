#include <unistd.h>
#include <error.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void Usage() {
  char buff[256];
  sprintf(buff, "./pr4 archivo1 archivo2\n");
  write(1, buff, strlen(buff));
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 3) Usage();

  if ((mknod("pipe", S_IFIFO | 0666, 0) < 0) || errno == EEXIST) {
    perror("mknod");
    execlp("rm", "rm", "pipe", NULL);
  }

  int pid, pid2;

  pid = fork();
  if (pid == 0) {
    int fd = open(argv[1], O_RDONLY);
    int p1 = open("pipe", O_WRONLY);
    dup2(fd, 0); //entrada std leemos del archivo 1
    dup2(p1, 1); //salida estandar es entrada de pipe

    //cerramos los canales que no usamos
    close(fd);
    close(p1);

    execlp("./pr1", "./pr1", NULL);
  }

  pid2 = fork();

  if (pid2 == 0) {
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0640);
    int p0 = open("pipe", O_RDONLY);
    dup2(fd2, 1); //salida std escribimos en archivo 2
    dup2(p0, 0); //entrada estandar es salida de pipe

    //cerramos los canales que no usamos
    close(fd2);
    close(p0);

    execlp("./pr2", "./pr2", NULL);
  }

  while((waitpid(-1, NULL, 0)) > 0);

  exit(0);
}