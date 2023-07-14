#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void Usage()
{
  char buffer[128];
  sprintf(buffer, "Usage: quien soy num\n");
  write(1,buffer, strlen(buffer));
  exit(0);
}

void main(int argc, char *argv[])
{
  char buffer[128];

  if (argc < 2) Usage();

  sprintf(buffer, "Mi hermano es %s ,  mi pid es %d y mi padre %d\n", argv[1], getpid(), getppid());
  write(1,buffer, strlen(buffer));
  exit(getppid());
}
