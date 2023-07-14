#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>



// Encripta p1 p2 k1 k2

void main(int argc,char *argv[])
{

  int fd1,fd2, ret;
  int pid1, pid2;
  int k1 = atoi(argv[3]);
  int k2 = atoi(argv[4]);
  char c;
  if (argc != 5){
    printf("usage: encripta p1 p2 k1 k2\n");
    exit(0);
  }
  // Creo argv[1]
  ret = mknod(argv[1],S_IFIFO|S_IRUSR|S_IWUSR,0);
  if ((ret < 0) && (errno != EEXIST)){
    perror("creando pipe1");
    exit(0);
  }
  printf("Pipe 1 ready\n");
  ret = mknod(argv[2],S_IFIFO|S_IRUSR|S_IWUSR,0);
  if ((ret < 0) && (errno != EEXIST)){
    perror("creando pipe2");
    exit(0);
  }
  printf("Pipes creadas\n");
  pid1 = fork();
  if (pid1 == 0){
    // Lee de argv[1] y escribe en fichero mensaje_recibido
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0){
      perror("Open 1:lectura");
      exit(0);
    }
    fd2 = open("mensaje_recibido", O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    if (fd2 < 0){
      perror("Open 2");
      exit(0);
    }

    while(read(fd1,&c,sizeof(char))>0){
      c = c - k1;
      write(fd2,&c,sizeof(char));
    }
    exit(0);

  }

  pid2 = fork();
  if (pid2 == 0){
    // Lee de stdin y escribe en pipe argv[2]
    fd1 = open(argv[2], O_WRONLY);
    if (fd1 < 0){
      perror("P2 Open 1: escritura");
      exit(0);
    }

    while(read(0,&c,sizeof(char))>0){
      c = c + k2;
      write(fd1, &c, sizeof(char));
    }
    exit(0);
    
  }
  while(waitpid(-1,NULL,0)>0);

  unlink(argv[1]);
  unlink(argv[2]);

}

