#ifndef SCHEDULER_API_H
#define SCHEDULER_API_H

typedef int boolean;
#define true 1
#define false 0

typedef struct
{
	int num_eat;
	int eating_time; /*in sec*/
	int satisfied_time;  /*in sec*/
} animal_t;

void printHelp(); 
 
#endif
