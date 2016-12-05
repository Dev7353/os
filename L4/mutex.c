/**
 * mutex - simple mutex example
 *
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// The number of thread that will be started
#define THREAD_COUNT 10
// sleeping factor
#define N 5

#define true 1
#define false 0

typedef int bool;

int accessMatrix[THREAD_COUNT];

/*
 * argument given to the thread
 *
 */
struct thread_args {
    int ident ;                 // theads identity
    int * global_counter ;      // global counter

    // add argument that you want to pass to threads


} ;
// type def for those who don't like writing struct all the time
typedef struct thread_args  thread_args_t ;

//mutex global declaration
pthread_mutex_t mutex;
//condition variable
pthread_cond_t cv;

/*
 * forward declaration of functions
 */
void print_ident(thread_args_t *args);
int isFull(int idx);
void printAccessMatrix();
bool isFull(int idx);



/*
 *  Main code
 */
int
main(int argc, char * argv[]){

    pthread_t        worker[THREAD_COUNT] ;  // threads
    thread_args_t    args[THREAD_COUNT];     // thread arguments
    int i ;
    int counter = 0 ;                        // global counter
    long *statusp  ;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cv, NULL);
    printf("Mutex program starting. \n");

    /* Set evil seed (initial seed) */
    srand( (unsigned)time( NULL ) );


    /*
     * start up threads
     */
    for(i=0;i<THREAD_COUNT;i++){

    /* Set up argument */
    args[i].ident = i  ;
    args[i].global_counter = &counter;

        // set any other argument you want to pass to the
    // newly created thread

    pthread_create(&worker[i],
               NULL,
               (void *(*)(void *)) print_ident,
               (void *) &args[i]) ;
    }

    /* sleep for 60 secs before cleaning up*/
    sleep(60);

    /*
     * cleaning up after threads
     */
    for(i=0;i<THREAD_COUNT;i++){
    /* just in case the thread returns, make sure we handle it */
    pthread_join(worker[i],(void *)&statusp);
    if(PTHREAD_CANCELED == statusp ){
        printf("thread %d was canceld\n",args[i].ident);
    }else{

           printf("thread %d completed. it's return value isf %ld\n",
           args[i].ident,*statusp);

    }
    }
    /* and we're done */
    return(0);

}

void
print_ident(thread_args_t *args){
	    
	int s ;
	int ctr = 0;

    /* say hello to the world. */
    printf("Hello world, I'm thread %d\n",args->ident);

		
    for(int i = 0;i < THREAD_COUNT; ++i){
	pthread_mutex_lock(&mutex);
	
	while(ctr > 0)
	{
		if(isFull(i) == true)
		{
			ctr = 0;
			pthread_cond_signal(&cv);
		}
		pthread_cond_wait(&cv, &mutex); 
	}	
	s =1+(int) (N * ((double) rand() / (double)(RAND_MAX + 1.0))) ;
	sleep(1); // dont forget to change again!!! 

	*args->global_counter +=1 ;
	++ctr;
	accessMatrix[i] += ctr;
	printf("thread %2d  counting  %2d\n",
		      args->ident,*args->global_counter);
	
	
	pthread_mutex_unlock(&mutex);    
    }
    /* should never happen */
    printAccessMatrix();
    fprintf(stderr,"I'm returning.. [%d]\n",args->ident);
    
}

bool isFull(int idx){
	int sum = 0;

	for(int i = 0; i < THREAD_COUNT; ++i)
		sum += accessMatrix[i];
	
	if(sum == THREAD_COUNT*(idx+1))
		return true;
	
	return false;
}

void printAccessMatrix()
{
	for(int i = 0; i < THREAD_COUNT; ++i)
		printf("|%d", accessMatrix[i]);
		
	printf("\n");

}
