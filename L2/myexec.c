#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int status, pid, i, j;
	/*Only support for executables in /bin/*/
	char *prefix = "/bin/";
	char *bin = argv[1];
	char **arguments;
	
	/*create a path to binary from argument*/
	int n = strlen(prefix) + strlen(bin);
	char *executeable = NULL;
	
	executeable = (char*) malloc(n+1 * sizeof(char));
	
	if(executeable == NULL)
	{
			perror("Memory allocation failed.");
			return 1;
	}
	
	strcat(strcpy(executeable, prefix), bin);
	

	/*create list of arguments from the agurments*/
	
	arguments = NULL;
	arguments = (char**) malloc((argc) * sizeof(char*));
	if(arguments == NULL)
	{
			perror("Memory allocation failed.");
			return 1;
	}
	
	j = 0;
	
	for(i = 1; i < argc; ++i)
	{
		arguments[j] = (char*) malloc(strlen(argv[i])+1 * sizeof(char));
		if(arguments[j] == NULL)
		{
			perror("Memory allocation failed.");
			return 1;
		}
		
		strcpy(arguments[j], argv[i]);
		++j;
	}
	
	/*last argument needs null pointer for termination. See [man execv]*/
	arguments[argc-1] = (void*) NULL;
	
	/*create child and execute program with arguments*/

	if(argc == 1)
	{
			perror("-- No Argument --\n");
			return 1;
	}
	
	pid = fork();
		
	if(pid < 0)
	{
		/*forking failed*/
		perror("-- fork --");
		return 1;
	}
	else if(pid == 0)
	{
		/*chidprocess is running*/

		return execv(executeable, arguments);
		
	}
	else
	{
		/*parentprocess is running*/	
		wait(&status);	
		
		if (WIFEXITED(status))
		{
			if(status != 0)
			{
					printf("Exec Fehlgeschlagen. Das Programm %s beendet mit Status %d.\n", argv[1], WEXITSTATUS(status));
			}
			else
			{
					printf("Beendet mit Status %d.\n", WEXITSTATUS(status));
			}
		}
	}
	
	/*free memory*/
	
	for(i = 0; i < argc-1; ++i)
	{
		free(arguments[i]);
	}
	
	free(arguments);
	free(executeable);
}
