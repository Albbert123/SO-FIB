#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void main(int argc, char *argv[])
{
  char buffer[128];


  sprintf(buffer, " mi pid es %d \n", getpid());
  write(1,buffer, strlen(buffer));
  exit(getpid()%2);
}
