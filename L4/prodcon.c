#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "buffer.h"
#include "prodcon-api.h"

//struct for thread arguments

typedef struct{
	int start;
	int stop;
	int delay;
} thread_args_t;

//Function prototypes
void* consumer(void* args);
void* producer(void* args);
char* convert(char* string);

//mutex and cv
pthread_cond_t cv;
pthread_mutex_t mutex;

int sp[2];

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
	
	int delay = 2000; // number of millisecs for the passvie sleep
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
	
	pthread_t consumers[consumerThreads];
	pthread_t producers[producerThreads];
	thread_args_t argsProducer[producerThreads];
	//thread_args_t argsConsumer[consumerThreads];
	initBuffer(&buffer, bufferRows, colsPerRows);
	
	for(int i = 0; i < producerThreads; ++i)
	{
		argsProducer[i].start = i * (inputBuffer.head / producerThreads);
		argsProducer[i].stop = (i+1) * (inputBuffer.head / producerThreads);
		argsProducer[i].delay = delay;
		pthread_create(&producers[i], NULL, (void *(*)(void *))producer, &argsProducer[i]);
	}
	
	for(int i = 0; i < consumerThreads; ++i)
	{
		//printf("%d %d\n", argsConsumer[i].start, argsConsumer[i].stop);
		pthread_create(&consumers[i], NULL, (void *(*)(void *))consumer, NULL);
	}
	
	for(int i = 0; i < producerThreads; ++i)
	{
		pthread_join(producers[i], NULL);
	}
	
	for(int i = 0; i < consumerThreads; ++i)
	{
		pthread_join(consumers[i], NULL);
	}
	
	destroyBuffer(&buffer);
	destroyBuffer(&inputBuffer);

	return 0;
}

void* consumer(void* args)
{
	//thread_args_t* arg = (thread_args_t*) args;
	pthread_mutex_lock(&mutex);
	bool sleeped = false;
	while(sleeped == false)
	{
		printf("Go sleep\n");
		pthread_cond_wait(&cv, &mutex);
		sleeped = true;
	}
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	int start = sp[0];
	int stop = sp[1];
	for(int i = start; i  < stop; ++i)
	{
		//verbose
		printf("threadid consumes %s\n", buffer.queue[i]);
		//wait busy or passive
		printf("threadid reports (time) %s\n", convert(buffer.queue[i])); //needs id
	}
	
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

char* convert(char* string)
{
	for(int i = 0; ; ++i)
	{
		string[i] = toupper(string[i]);
		if(string[i] == '\0')
			break;
	}
	return string;
}



void* producer(void* args)
{
	thread_args_t* arg = (thread_args_t*) args;
	pthread_mutex_lock(&mutex);
	//sleep(arg->delay/1000);
	for(int i = arg->start; i  < arg->stop; ++i)
	{
		add(&buffer, inputBuffer.queue[i]);
		printf("threadid produces %s\n", inputBuffer.queue[i]);
	}
	
	pthread_cond_broadcast(&cv);
	sp[0] = arg->start;
	sp[1] = arg->stop;
	
	pthread_mutex_unlock(&mutex); 
	
	pthread_exit(0);
}
