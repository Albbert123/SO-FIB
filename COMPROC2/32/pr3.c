#include <unistd.h>
#include <error.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int fd[2];

void
close_pipes ()
{
  if (close (fd[0]) < 0)
    error (1, errno, "close");
  if (close (fd[1]) < 0)
    error (1, errno, "close");
}

void
spawn (char *exe, char *input, char *output, int redin, int redout)
{
  switch (fork ())
    {
    case -1:
      error (1, errno, "fork");

    case 0:
      if (input != NULL)
        {
          if (close (0) < 0)
            error (1, errno, "close");
          if (open (input, O_RDONLY) < 0)
            error (1, errno, "open");
          if (dup2 (redout, 1) < 0)
            error (1, errno, "dup2");
        }
      else
        {
          if (close (1) < 0)
            error (1, errno, "close");
          if (open (output, O_WRONLY | O_TRUNC | O_CREAT, 0640) < 0)
            error (1, errno, "open");
          if (dup2 (redin, 0) < 0)
            error (1, errno, "dup2");
        }
      close_pipes ();
      execl (exe, exe, NULL);
      error (1, errno, "exec");
    }
}

int
main (int argc, char *argv[])
{
  if (argc != 3)
    error (1, 0, "Wrong parameters");

  if (pipe (fd) < 0)
    error (1, errno, "pipe");

  spawn ("pr1", argv[1], NULL, -1, fd[1]);
  spawn ("pr2", NULL, argv[2], fd[0], -1);

  close_pipes ();

  wait (NULL);
  wait (NULL);
}
