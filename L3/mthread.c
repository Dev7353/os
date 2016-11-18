#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <ctype.h>

void *PrintHello(void *threadarg);

/*Parsing based on https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt*/

int main(int argc, char* argv[])
{
	int status;
	long t;
	long NumberOfThreads = 4;
	int c; //argument
	
	pthread_t *thread;
	
	while((c = getopt(argc, argv, "t:")) != -1)
	{
		switch(c)
		{
			case 't':
				NumberOfThreads = atoi(optarg);
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
	
	thread = (pthread_t*) malloc(NumberOfThreads * sizeof(pthread_t));
	
	
	for(t = 0; t < NumberOfThreads; t++)
	{
		printf("Creating thread %ld\n", t);
		assert(pthread_create(&thread[t], NULL, PrintHello, &t) == 0);
		
	}
	
	for(t = 0; t < NumberOfThreads; t++)
	{
		printf("Main joining with thread %ld\n", t);
		assert(pthread_join(*(&thread[t]), (void*)(&status)) == 0);

                printf("Completed joining with thread %ld status = %d\n", t,
                       status);
	}
	
	free(thread);
}

void *PrintHello(void *threadarg)
{
	printf("%ld: Hello World\n", *((long *) threadarg));
	
	sleep((int) (rand()%10)+1);
	
	printf("%ld: Thread is done after sleeping %d[s]\n", *((long *) threadarg), 0); /*For now 0 is a dummy value*/
	
	return NULL;
}
