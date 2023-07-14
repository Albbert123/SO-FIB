//reprogramar la gestión del signal SIGINT y que muestre un mensaje por salida estándar informando de que se ha recibido el signal.
//Modifica el programa principal para que después del read se muestre un mensaje en salida estándar indicando el resultado de 
//la operación: read correcto, read con error (diferente de interrupción por signal), o read interrumpido por signal

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

int recibido;

void error_y_exit(char* txt){
    char buf[60];
    perror(txt);
    exit(1);
}

void trat_s(int s) {
    recibido = 1;
}

int
main ()
{
  char buffer[256];
  char c;
  int ret;
  recibido = 0;

  struct sigaction sa;

  sa.sa_handler = trat_s;
  
  sigfillset(&sa.sa_mask);

  if (sigaction (SIGINT, &sa, NULL) < 0) error_y_exit ("sigaction");


  ret = read (0, &c, sizeof (char));

  if(recibido==1) {
    sprintf(buffer,"signal recibido\n");
    write(1,buffer,strlen(buffer));
  }

  else {
    if(ret>0) sprintf(buffer, "Se ha hecho el read\n");
  else if(ret<0) sprintf(buffer,"%s\n",strerror(errno));
  else sprintf(buffer, "Error read\n");
    write(1,buffer,strlen(buffer));
    write (1, &c, sizeof (char));
  }
}
