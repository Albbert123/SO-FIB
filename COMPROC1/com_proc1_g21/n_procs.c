#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void Usage()
{
  char buffer[128];
  sprintf(buffer, "Usage: n_procs num\n");
  write(1,buffer, strlen(buffer));
  exit(0);
}

void error(char *txt)
{
  char buffer[128];
  perror(txt);
  exit(0);
}

void main(int argc, char *argv[])
{
  char buffer[128];
  int nump, current, pid;
  char orden[64];

  if (argc < 2) Usage();

  nump = atoi(argv[1]);
  for (current = 0; current < nump; current++){
    pid = fork();
    if (pid == 0){
      sprintf(orden,"%d", current);
      execlp("./quien_soy", "./quien_soy", orden, NULL);
      error("Execlp");
    }else if (pid < 0){
      error("error fork");
    }
    waitpid(-1,NULL,0);
  }

}
