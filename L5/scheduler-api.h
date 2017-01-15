#ifndef SCHEDULER_API_H
#define SCHEDULER_API_H
#define ANSI_COLOR_RED 		"\x1b[31m"
#define ANSI_COLOR_BLUE 	"\x1b[34m"
#define ANSI_COLOR_GREEN 	"\x1b[32m"
#define ANSI_COLOR_YELLOW  	"\x1b[33m"
#define ANSI_COLOR_RESET   	"\x1b[0m"

#include <pthread.h>
#include <stdio.h>
typedef int boolean;
#define true 1
#define false 0

typedef struct 
{
	pthread_cond_t** container; //wrapper for cvs for each animal
	int** priority; // prioritiy for each animal 
	int* group_priority; // priority for each animal group
	int* threads_per_group;
	
}prio_queue_t;

typedef struct
{
	int num_eat;
	int eating_time; /*in sec*/
	int satisfied_time;  /*in sec*/
	char* animal_type;
	int id;
} animal_t;

typedef struct
{
	int bowles;
	char* status;
	int num_eaten;
	int* eating_times_per_group;
}food_area;

#define CAT "cat"
#define DOG "dog"
#define MOUSE "mouse"
#define GROUPS 3

//functions
void printHelp(); 
int nextBowle(char* status, int bowles); 
pthread_cond_t* nextAnimal();
int nextGroup();
boolean groupIsDone(int animal);
void calcGroupPriorities(int current_group);
boolean checkIfEmpty(int animal);
void printStatistics();
void initializeSynchronize();

//global variables
FILE* fp;
boolean file;
char* filename;
food_area area;
int** threadDone;
int** synchronize;
double** waiting_times;
double* waiting_times_group;
int isReady[GROUPS];
pthread_cond_t* cond_cats;
pthread_cond_t* cond_dogs; 
pthread_cond_t* cond_mice;

prio_queue_t prio;
pthread_mutex_t mutex;
boolean verbose ;
#endif
