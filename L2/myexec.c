#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int status, pid, w, i, j;
	char *prefix = "/bin/";
	char *bin = argv[1];
	char **arguments;
	
	/*create a path to binary from argument*/
	int n = strlen(prefix) + strlen(bin);
	char *executeable = NULL;
	
	executeable = (char*) malloc(n+1 * sizeof(char));
	
	if(executeable == NULL)
	{
			return 1;
	}
	
	strcat(strcpy(executeable, prefix), bin);
	

	/*create list of arguments from the agurments*/
	
	arguments = NULL;
	arguments = (char**) malloc((argc-1) * sizeof(char*));
	if(arguments == NULL)
	{
			return 1;
	}
	
	j = 0;
	
	for(i = 1; i < argc; ++i)
	{
		/*printf("Lenght of %s arg %lu\n", argv[i], strlen(argv[i]));*/
		arguments[j] = (char*) malloc(strlen(argv[i])+1 * sizeof(char));
		if(arguments[j] == NULL)
		{
			return 1;
		}
		
		strcpy(arguments[j], argv[i]);
		++j;
	}

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

		execv(executeable, arguments);
	}
	else
	{
		/*parentprocess is running*/	
		w = wait(&status);
		printf("Status: %d\n", status);
	}
	
	/*free memory*/
	
	for(i = 0; i < argc-1; ++i)
	{
		free(arguments[i]);
	}
	
	free(arguments);
	free(executeable);
}
