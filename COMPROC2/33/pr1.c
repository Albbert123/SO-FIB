#include <unistd.h>
#include <error.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "maxdigits.h"

int
main ()
{
  int r;
  unsigned int n;

  while ((r = read (0, &n, sizeof (n))) == sizeof (n))
    {
      char s[MAXDIGITS + 2]; // + \n + \0

      sprintf (s, "%u\n", n);
      if (write (1, s, strlen (s)) < 0)
        error (1, errno, "write");

    }
  if (r < 0)
    error (1, errno, "read");
}
