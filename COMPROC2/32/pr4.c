#include <unistd.h>
#include <error.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#define NP "NamedPipe"
void
spawn_np (char *exe, char *input, char *output)
{
  switch (fork ())
    {
    case -1:
      error (1, errno, "fork");

    case 0:
      if (close (0) < 0)
        error (1, errno, "close");
      if (close (1) < 0)
        error (1, errno, "close");

      if (input != NULL)
        {
          if (open (input, O_RDONLY) < 0)
            error (1, errno, "open");
          if (open (NP, O_WRONLY) < 0)
            error (1, errno, "open");
        }
      else
        {
          if (open (NP, O_RDONLY) < 0)
            error (1, errno, "open");
          if (open (output, O_WRONLY | O_TRUNC | O_CREAT, 0640) < 0)
            error (1, errno, "open");
        }
      execl (exe, exe, NULL);
      error (1, errno, "exec");
    }
}

int
main (int argc, char *argv[])
{
  if (argc != 3)
    error (1, 0, "Wrong parameters");

  if (mkfifo (NP, 0600) < 0)
    {
      if (errno != EEXIST)
        error (1, errno, "mkfifo");
    }

  spawn_np ("pr1", argv[1], NULL);
  spawn_np ("pr2", NULL, argv[2]);

  wait (NULL);
  wait (NULL);
}
