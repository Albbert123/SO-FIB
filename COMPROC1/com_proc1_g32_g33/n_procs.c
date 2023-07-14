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

  if (argc < 2) Usage();

  nump = atoi(argv[1]);
  for (current = 0; current < nump; current++){
    pid = fork();
    if (pid == 0){
      execlp("./quien_soy", "./quien_soy", NULL);
      error("Execlp");
    }else if (pid < 0){
      error("error fork");
    }
  }
  int result;
  while ((pid = waitpid(-1, &result, 0)) > 0){
    if (WIFEXITED(result)){ //termina con un exit?
      sprintf(buffer, "Termina proceso %d con resultado %d\n", pid, WEXITSTATUS(result));
      write(1, buffer, strlen(buffer));
    }
  }

}
