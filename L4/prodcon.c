#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "buffer.h"
#include "prodcon-api.h"
#include <assert.h>
#include <time.h>
#include <math.h>

//struct for thread arguments

typedef struct{
	double start;
	double stop;
	int delay;
	int upper,lower;
	int id;
} thread_args_t;

//Function prototypes
void* consumer(void* args);
void* producer(void* args);
char* convert(char* string);
void* observe(void* args);

void refreshConsumers();
int nextConsumer();
bool operationsLeft();
bool consumersAreDone();

//mutex and cv
pthread_cond_t cv;
pthread_mutex_t mutex;

//global variables
Buffer buffer, inputBuffer;
time_t sec;
bool verbose = false;
int consumerThreads = 1;
pthread_cond_t *vars;
bool* accessConsumer; 
char* output = NULL;
int operations = 0;


int
main (int argc, char **argv)
{
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cv, NULL);	

	int c; //argument
	char* input = NULL;
	
	int bufferRows = 20; //default
	int colsPerRows = 20; //default
	
	int delay = 3000; // number of millisecs for the passvie sleep
	int upperBorder = 1; //default in sec
	int lowerBorder = 2; //default in sec
	int busyLoopFactor = 0; // default	
	
	int producerThreads = 1;
	
	srandom((unsigned int) sec);
	
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
		  
		  
	vars = (pthread_cond_t*) malloc(sizeof(pthread_cond_t) * consumerThreads);
	for(int i = 0; i < consumerThreads; ++i)
	{
		pthread_cond_init(&vars[i], NULL);
	}
	
	accessConsumer = (bool*) malloc(sizeof(bool) * consumerThreads);
	assert(accessConsumer != NULL);
	refreshConsumers();
	
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
	
	operations = inputBuffer.tail;
	pthread_t consumers[consumerThreads];
	pthread_t producers[producerThreads];
	pthread_t observer;
	thread_args_t argsProducer[producerThreads];
	thread_args_t argsConsumer[consumerThreads];
	
	initBuffer(&buffer, bufferRows, colsPerRows);
	
	printf("Observer started\n");
	pthread_create(&observer, NULL, observe, NULL);
	
	printf("%d Consumer startet\n", consumerThreads);
	for(int i = 0; i < consumerThreads; ++i)
	{
		argsConsumer[i].id = i;
		argsConsumer[i].upper = upperBorder;
		argsConsumer[i].lower = lowerBorder;
		pthread_create(&consumers[i], NULL, (void *(*)(void *))consumer, &argsConsumer[i]);
	}
	
	printf("%d Producer startet\n", producerThreads);
	for(int i = 0; i < producerThreads; ++i)
	{
		argsProducer[i].start = round(i * (inputBuffer.tail / producerThreads));
		argsProducer[i].stop = round((i+1) * (inputBuffer.tail / producerThreads));
		argsProducer[i].delay = delay;
		argsProducer[i].id = i;
		pthread_create(&producers[i], NULL, (void *(*)(void *))producer, &argsProducer[i]);
	}
	
	//joins
	
	for(int i = 0; i < producerThreads; ++i)
	{
		pthread_join(producers[i], NULL);
	}
	
	for(int i = 0; i < consumerThreads; ++i)
	{
		pthread_join(consumers[i], NULL);
	}
	
	pthread_join(observer, NULL);
	
	destroyBuffer(&buffer);
	destroyBuffer(&inputBuffer);
	
	
	//free pointers
	
	free(vars);
	return 0;
}

void* consumer(void* args)
{
	thread_args_t* arg = (thread_args_t*) args;
	while(true)
	{
		if(operationsLeft() == false)
		{
			printf("Theres no work left\n");
			pthread_cond_signal(&cv);
			break;
		}
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&vars[arg->id], &mutex);
		
		if(operationsLeft() == false)
		{
			printf("C%d im out\n", arg->id);
			pthread_mutex_unlock(&mutex);
			break;
		}
		
		--operations;
		accessConsumer[arg->id] = true;
	
		char* c = pop(&buffer);
		printf("C%d consumes %s\n",arg->id, c);
		
		pthread_mutex_unlock(&mutex);
		
		time(&sec);
		int s = (random () % arg->upper) + arg->lower;
		sleep (s);
		printf("C%d reports (time) %s\n",arg->id, convert(c)); //needs id
		
	}
	pthread_exit(0);
}

void* producer(void* args)
{
	thread_args_t* arg = (thread_args_t*) args;
	for(int i = (int) arg->start; i  < (int)arg->stop; ++i)
	{
		pthread_mutex_lock(&mutex);
		//read from input
		char* input = pop(&inputBuffer);
		add(&buffer, input);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cv);
		printf("P%d produces %s\n", arg->id, input);
		free(input);
		
		sleep(arg->delay/1000);
	}
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

void* observe(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cv, &mutex);
		pthread_mutex_unlock(&mutex);
		for(int i = 0; i < consumerThreads; ++i)
			printf("%d ", accessConsumer[i]);
			
		printf("\n");
			
		//release next consumer thread
		if(consumersAreDone() == true && operationsLeft() == true)
		{
			refreshConsumers();
		} 
				
		if(operationsLeft() == false)
			break;
		pthread_cond_signal(&vars[nextConsumer()]);
	}

	for(int i = 0; i < consumerThreads; ++i)
	{
		pthread_cond_signal(&vars[i]);
	}
	printf("Observe is done\n");
	pthread_exit(0);	
}

bool consumersAreDone()
{
	for(int i = 0; i < consumerThreads; ++i)
	{
		if(accessConsumer[i] == false)
			return false;
	}
	
	return true;
}

bool operationsLeft()
{
	if(operations == 0)
		return false;
	
	return true;
}

int nextConsumer()
{
	for(int i = 0; i < consumerThreads; ++i)
	{
		if(accessConsumer[i] == false)
			return i;
	}
	
	return -1;
}

void refreshConsumers()
{
	for(int i = 0; i < consumerThreads; ++i)
	{
		accessConsumer[i] = false;
	}
}
