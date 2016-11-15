#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "ini.h"
#include <sys/wait.h>
#include <assert.h>

#define DEFAULTCONFIG "forksqrt.cfg"

typedef struct
{
  char *start;
  char *loops;
  char *tolerance;
  char *numbers;
} configuration;

static int
handler (void *sqrt2, const char *section, const char *name,
	 const char *value)
{
  configuration *pconfig = (configuration *) sqrt2;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  if (MATCH ("sqrt2", "start"))
    {
      pconfig->start = strdup (value);
    }
  else if (MATCH ("sqrt2", "loops"))
    {
      pconfig->loops = strdup (value);
    }
  else if (MATCH ("sqrt2", "tolerance"))
    {
      pconfig->tolerance = strdup (value);
    }
  else if (MATCH ("sqrt2", "numbers"))
    {
      pconfig->numbers = strdup (value);
    }
  else
    {
      return 0;			/* unknown section/name, error */
    }
  return 1;
}

int
main (int argc, char *argv[])
{
  configuration config;
  pid_t pid;
  int writeToChild[2];		// First pipe that sends the data to the child
  int readFromChild[2];		// Second pipe that reads the results from the child
  char readbuffer[1000] = "";
  char *buffer;
  int errno = 0;
  int status;

  char *configfile = NULL;
  int flag = 0;

  int c;
  configfile = DEFAULTCONFIG;
  while ((c = getopt (argc, argv, "hdc:")) != -1)
    {
      switch (c)
	{
	case 'h':
	  printf ("-h help \n-d debug \n-c config\n");
	  return 0;
	case 'd':
	  flag = 1;
	  break;
	case 'c':
	  if (optarg == NULL)
	    break;
	  else
	    configfile = optarg;
	  break;
	}

    }

  if (ini_parse (configfile, handler, &config) < 0)
    {
      perror ("Can't load config'\n");
      return 1;
    }

  if(pipe (writeToChild) != 0)
    {
      printf("Failed Syscall: %d\n", errno);
      perror("Pipe not successful.\n");
    }
  if(pipe (readFromChild) != 0)
    {
      printf("Failed Syscall: %d\n", errno);
      perror("Pipe not successful.\n");
    }
  pid = fork ();
  if (pid < 0)
    {
    printf("Failed Syscall: %d\n", errno);
    perror("Fork not successful. \n");
    }
  if (pid == 0)
    {
      char fd0[20], fd1[20];
      snprintf (fd0, 20, "%d", writeToChild[0]);
      snprintf (fd1, 20, "%d", readFromChild[1]);
      char *python[] = { "python3", "forksqrt.py", fd0, fd1, NULL };
      execvp ("python3", python);
    }
  if(close (writeToChild[0]) != 0)
    {printf("Failed Syscall: %d\n", errno);
        perror("Close not successful.\n");}
  if(close (readFromChild[1]) != 0)
    {printf("Failed Syscall: %d\n", errno);
        perror("Close not successful.\n");}

  buffer = (char *) malloc (sizeof (config) + 4);
  assert(buffer != NULL);

  strcpy (buffer, config.start);
  strcat (buffer, "|");
  strcat (buffer, config.loops);
  strcat (buffer, "|");
  strcat (buffer, config.tolerance);
  strcat (buffer, "|");
  strcat (buffer, config.numbers);

  if (flag == 1)
    {
      strcat (buffer, "|");
      strcat (buffer, "True");
    }
  else
    {
      strcat (buffer, "|");
      strcat (buffer, "False");

    }

  if(write (writeToChild[1], buffer, strlen (buffer)) == -1)
    {
      printf("Failed Syscall: %d\n", errno);
        perror("Write not successful.\n");}
  if(read (readFromChild[0], readbuffer, sizeof (readbuffer)) == -1)
    {printf("Failed Syscall: %d\n", errno);
        perror("Read not successful.\n");}
  printf ("Results: %s\n", readbuffer);
  wait (&status);
  if (WIFEXITED (status))
  {
    if (status != 0)
      {
        printf
    ("Wait not successful. Exitcode: %d. Failed Syscall: %d.\n", WEXITSTATUS (status), errno);
      }
  }

  free (buffer);
  free (config.start);
  free (config.loops);
  free (config.tolerance);
  free (config.numbers);
  return 0;
}
