#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>




void main(int argc,char *argv[])
{
  int pid1, pid2, pid3;
  int fd;
  int p1[2], p2[2];

  pipe(p1);
  pid1 = fork();
  if (pid1 == 0){
    dup2(p1[1], 1);
    close(p1[0]);close(p1[1]);
    execlp("ps","ps","-a", NULL);
    perror("fallo ps");
    exit(0);
  }
  pipe(p2);
  pid2 = fork();
  if (pid2 == 0){
    dup2(p1[0],0);
    dup2(p2[1],1);
    close(p1[0]);close(p1[1]);
    close(p2[0]);close(p2[1]);
    execlp("grep","grep", argv[1], NULL);
    perror("fallo grep");
    exit(0);
  }
  close(p1[0]);close(p1[1]);close(p2[1]);
  pid3 = fork();
  if (pid3 == 0){
    fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if (fd < 0){
      perror("open");exit(0);
    }
    dup2(fd,1);
    char txt[128];
    sprintf(txt,"El numero de procesos tipo %s es ", argv[1]);
    write(1,txt, strlen(txt));
    dup2(p2[0],0);
    close(p2[0]);
    execlp("wc","wc","-l",NULL);
    perror("fallo wc");exit(0);
  }
  close(p2[0]);
  while(waitpid(-1,NULL,0)>0);

}

