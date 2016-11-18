#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

typedef int boolean;
#define true 1;
#define false 0;

void *PrintHello(void *threadarg);

/*Parsing based on https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt*/

int main(int argc, char* argv[])
{
	boolean infos = false;
	int status;
	long t;
	long NumberOfThreads = 4;
	int c; //argument
	
	pthread_t *thread;
	
	while((c = getopt(argc, argv, "hvt:")) != -1)
	{
		switch(c)
		{
			case 't':
				NumberOfThreads = atoi(optarg);
				break;
			case 'h':
				printf("Help: \n-v, --verbose \t\t get more informations while running\n-h, --help  \t\tget help\n-t --times \t\tcreate t times threads\n\n");
				printf("Current Git HEAD commit number: \n");
				const char *gitversion = "git rev-parse HEAD";
				system(gitversion);
				return 0;
			case 'v':
				infos = true;
				break;
			case '?':
				if(optopt == 't')
					fprintf(stderr, "Option -%c requires argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
				return 1;
			default:
				abort();
		}
	}
	
	errno = 0;
	
	thread = (pthread_t*) malloc(NumberOfThreads * sizeof(pthread_t));
	if(thread == NULL)
	{
		perror("malloc fail");
		if(infos)
			printf("Errno %d\n", errno);
		exit(1);

	}
	
	for(t = 0; t < NumberOfThreads; t++)
	{
		printf("Creating thread %ld\n", t);
		if(pthread_create(&thread[t], NULL, PrintHello, &t) != 0)
		{
			perror("creating Thread failed\n");
			if(infos)
				printf("Errno %d\n", errno);
			exit(1);
		} 
			
	}
	
	for(t = 0; t < NumberOfThreads; t++)
	{
		printf("Main joining with thread %ld\n", t);
		if(pthread_join(*(&thread[t]), (void*)(&status)) != 0)
		{
			perror("join failed\n");
			if(infos)
				printf("Errno %d\n", errno);
			exit(1);
		}

                printf("Completed joining with thread %ld status = %d\n", t,
                       status);
	}
	
	free(thread);
}

void *PrintHello(void *threadarg)
{
	time_t sec;
	printf("%ld: Hello World\n", *((long *) threadarg));
	time(&sec);
	srand((unsigned int) sec);
	int s = (rand()%10)+1;
	sleep(s);
	
	printf("%ld: Thread is done after sleeping %d[s]\n", *((long *) threadarg), s); /*For now 0 is a dummy value*/
	
	return NULL;
}
