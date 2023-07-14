#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 8

int
esNumero (char *str)
{
  if (str == NULL)
    return 0;
  for (int i = 0; str[i] != '\0'; i++)
    {
      if (!(str[i] >= '0' && str[i] <= '9') || (i >= MAX_SIZE))
	return 0;
    }
  return 1;
}

int
main (int argc, char *argv[])
{
  char buf[80];
  for (int i = 1; i < argc; i++)
    {				//i=1 para que no se vea el ./numero
      int n = esNumero (argv[i]);
      if (n == 1)
	sprintf (buf, "%s Es un numero\n", argv[i]);
      else
	sprintf (buf, "%s No es un numero\n", argv[i]);
      write (1, buf, strlen (buf));
    }
  return 0;
}
