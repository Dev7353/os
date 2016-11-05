#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int status, pid, w;
	
	char *bin = NULL;
	char *flags[] = {"/bin/ls", "-ls"};
	
	bin = "/bin/ls";
	
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
		execv(bin, flags);
		
	}
	else
	{
		/*parentprocess is running*/	
		w = wait(&status);
		printf("Status: %d\n", status);
	}
}
