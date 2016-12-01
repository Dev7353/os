#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static pthread_mutex_t mutex;
static pthread_cond_t cond_consumed = PTHREAD_COND_INITIALIZER;
static pthread_cond_t cond_produced = PTHREAD_COND_INITIALIZER;

static void *producer( void *arg )
{
        unsigned long count=0;

        while( 1 ) {
                printf("producer: wait...\n");
                pthread_mutex_lock( &mutex ); // enter critical section
                pthread_cond_wait( &cond_consumed, &mutex );
                printf("producer: PRODUCE %ld...\n", count++);
                pthread_cond_signal( &cond_produced );
                pthread_mutex_unlock( &mutex ); // leave critical section
        }
        return NULL;
}

static void *consumer( void *arg )
{
        unsigned long count=0; 

        sleep( 1 );
        pthread_cond_signal( &cond_consumed );
        while( 1 ) {
                printf("consumer: wait...\n");
                pthread_mutex_lock( &mutex );
                pthread_cond_wait( &cond_produced, &mutex );
                printf("consumer: CONSUME %ld...\n", count++);
                pthread_cond_signal( &cond_consumed );
                pthread_mutex_unlock( &mutex );
        }
        return NULL;
}

int main( int argc, char **argv, char **envp )
{
        pthread_t p1, p2;

        if (pthread_mutex_init( &mutex, NULL )) {
                perror("pthread_mutex_init");
                return -1;
        }

        pthread_create( &p2, NULL, consumer, NULL );
        pthread_create( &p1, NULL, producer, NULL );

        pthread_join( p1, NULL );
        pthread_join( p2, NULL );

        pthread_mutex_destroy( &mutex );
        return 0;
}
