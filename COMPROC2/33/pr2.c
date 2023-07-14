#include <unistd.h>
#include <error.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "maxdigits.h"

int
main ()
{
  int r, i = 0;
  char c;
  char s[MAXDIGITS + 1]; // + \0

  while ((r = read (0, &c, 1)) > 0)
    {
      if (c != '\n')
        {
          if (i < MAXDIGITS)
            s[i++] = c;
          else
            error (1, 0, "Too long");
        }
      else
        {
          unsigned int n;

          s[i] = '\0';
          i = 0;
          n = atoi (s);
          if (write (1, &n, sizeof (n)) < 0)
            error (1, errno, "write");
        }
    }
  if (r < 0)
    error (1, errno, "read");
}
