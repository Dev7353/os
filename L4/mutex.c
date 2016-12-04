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

/*
 * forward declaration of functions
 */
void print_ident(thread_args_t *args);



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

    // hint:: you may need to use a mutex.
    pthread_mutex_init(&mutex, NULL);

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



/**
 * this function is started as a new thread.
 *
 */
void
print_ident(thread_args_t *args){
    int s ;

    /* say hello to the world. */
    printf("Hello world, I'm thread %d\n",args->ident);


    for(;;){

    /*
     *  sleep for a random time to make thing random.
     */
        s =1+(int) (N * ((double) rand() / (double)(RAND_MAX + 1.0))) ;
    sleep(s);

    /*
     * up date the global counter and start go back to sleep.
     *
     * you need to add some code here so that threads obay thier
     * order in the queue.
     *
     */
    pthread_mutex_lock(&mutex);
    {
        *args->global_counter +=1 ;
        printf("thread %2d  counting  %2d\n",
               args->ident,*args->global_counter);
    }
    }
    /* should never happen */
    fprintf(stderr,"I'm returning.. [%d]\n",args->ident);
    
    pthread_mutex_unlock(&mutex	);
}
