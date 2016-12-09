#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "buffer.h"
#include "prodcon-api.h"

//struct for thread arguments

typedef struct{
	int start;
	int stop;
} thread_args_t;

//Function prototypes
void* consumer(void* args);
void* producer(void* args);

//mutex and cv
pthread_cond_t cv;
pthread_mutex_t mutex;

//global buffers
Buffer buffer, inputBuffer;

int
main (int argc, char **argv)
{

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cv, NULL);

	bool verbose = false;
	int c; //argument
	char* input = NULL;
	char* output;
	
	int bufferRows = 20; //default
	int colsPerRows = 20; //default
	
	int consumerThreads = 1;
	int producerThreads = 1;
	
	int delay = 0; // number of millisecs for the passvie sleep
	int upperBorder = 10; //default in sec
	int lowerBorder = 0; //default in sec
	int busyLoopFactor = 0; // default	

	while ((c = getopt (argc, argv, "vhi:o:L:C:c:p:t:r:R:a:")) != -1)
		switch (c)
		  {
		  case 'v':
		  verbose = true;
			  break;
		  case 'h':
			printf("HELP\n"); // todo:
			printf ("Current Git HEAD commit number: \n");
			const char *gitversion = "git rev-parse HEAD";
			system (gitversion);
			  break;
		  case 'i':
			  input = optarg;
			  break;
		  case 'o':
			  output = optarg;
			  break;
		  case 'L':
			  bufferRows =  atoi(optarg);
			  break;
		  case 'C':
			  colsPerRows = atoi(optarg);
			  break;
		  case 'c':
			  consumerThreads = atoi(optarg);
			  break;
		  case 'p':
		      producerThreads =  atoi(optarg);
			  break;
		  case 't':
			  delay = atoi(optarg);
			  break;
		  case 'r':
			  lowerBorder = atoi(optarg);
			  break;
		  case 'R':
			  upperBorder = atoi(optarg);
			  break;
		  case 'a':
			  busyLoopFactor = atoi(optarg);
			  break;
		  case '?':
			  if (optopt == 'i')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'o')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'L')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'C')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'c')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'p')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 't')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'r')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'R')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);
			  else if (optopt == 'a')
				  fprintf (stderr,
					   "Option -%c requires an argument.\n",
					   optopt);

			  else if (isprint (optopt))
				  fprintf (stderr, "Unknown option `-%c'.\n",
					   optopt);
			  else
				  fprintf (stderr,
					   "Unknown option character `\\x%x'.\n",
					   optopt);
			  return 1;
		  default:
			  abort ();
		  }
		  
	initBuffer(&inputBuffer, bufferRows, colsPerRows);
	if(input != NULL)
	{
		printf("[READ FROM FILE]\n"); 
		readFile(&inputBuffer, input);
	}
	else
	{
		printf("[READ FROM STDIN]\n"); 
		readStdin(&inputBuffer);
	}
	
	//pthread_t consumer[consumerThreads];
	pthread_t producers[producerThreads];
	thread_args_t argsProducer[producerThreads];
	thread_args_t argsConsumer[consumerThreads];
	initBuffer(&buffer, bufferRows, colsPerRows);
	
	for(int i = 0; i < producerThreads; ++i)
	{
		argsProducer[i].start = i * (inputBuffer.head / producerThreads);
		argsProducer[i].stop = (i+1) * (inputBuffer.head / producerThreads);
		pthread_create(&producers[i], NULL, (void *(*)(void *))producer, &argsProducer[i]);
	}
	
	for(int i = 0; i < consumerThreads; ++i)
	{
		argsProducer[i].start = i * (inputBuffer.head / producerThreads);
		argsProducer[i].stop = (i+1) * (inputBuffer.head / producerThreads);
		pthread_create(consumer[i], NULL, consumer, NULL);
	}
	
	for(int i = 0; i < consumerThreads; ++i)
	{
		pthread_join(consumer[i], NULL);
	}
	
	for(int i = 0; i < producerThreads; ++i)
	{
		pthread_join(producers[i], NULL);
	}
	
	printBuffer(&buffer);
	destroyBuffer(&buffer);
	destroyBuffer(&inputBuffer);

	return 0;
}

void* consumer(void* args)
{
	pthread_exit(0);
}


void* producer(void* args)
{
	thread_args_t* arg = (thread_args_t*) args;
	pthread_mutex_lock(&mutex);
	for(int i = arg->start; i  < arg->stop; ++i)
		add(&buffer, inputBuffer.queue[i]);

	
	pthread_mutex_unlock(&mutex); 
	
	pthread_exit(0);
}
