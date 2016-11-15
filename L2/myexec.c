#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int
main (int argc, char **argv)
{
  int status, pid;

  /*create child and execute program with arguments */

  if (argc == 1)
    {
      perror ("-- No Argument --\n");
      return 1;
    }

  pid = fork ();

  if (pid < 0)
    {
      /*forking failed */
      perror ("-- fork --");
      return 1;
    }
  else if (pid == 0)
    {
      /*chidprocess is running */

      return execvp (argv[1], &argv[1]);

    }
  else
    {
      /*parentprocess is running */
      wait (&status);

      if (WIFEXITED (status))
	{
	  if (status != 0)
	    {
	      printf
		("Exec Fehlgeschlagen. Das Programm %s beendet mit Status %d.\n",
		 argv[1], WEXITSTATUS (status));
	    }
	  else
	    {
	      printf ("Beendet mit Status %d.\n", WEXITSTATUS (status));
	    }
	}
    }

}
