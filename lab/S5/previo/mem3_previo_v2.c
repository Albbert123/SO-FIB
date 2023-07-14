#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>

#define REGION_SIZE		4096

int *p;

void error_segmen(){
    char buf[60];
    sprintf(buf,"Error de segmentacion\n");
    write(1,buf,strlen(buf));
    exit(1);
}

void error(char*txt) {
    char buf[100];
    perror(txt);
    exit(1);
}

int
main (int argc, char *argv[])
{
  char buff[256];

  struct sigaction sa;
  sigset_t mask;

  //blok
  sigemptyset(&mask);
  sigaddset(&mask,SIGSEGV);
  sigprocmask(SIG_BLOCK,&mask,NULL);

  //repr
  sa.sa_flags=0;
  sigfillset(&sa.sa_mask);
  sa.sa_handler = error_segmen;
  if(sigaction(SIGSEGV,&sa,NULL)<0) error("sigaction");

  sigfillset(&mask);
  sigdelset(&mask,SIGSEGV);
  sigdelset(&mask,SIGINT);

  sprintf (buff, "Addresses:\n");
  write (1, buff, strlen (buff));
  sprintf (buff, "\tp: %p\n", &p);
  write (1, buff, strlen (buff));

  *p = 5;

  sigsuspend(&mask);

  sprintf (buff, "\tp Address: %p, p value: %p, *p: %d\n", &p, p, *p);
  write (1, buff, strlen (buff));

  exit (0);

}
