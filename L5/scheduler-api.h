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
	char* animal_type;
	int id;
} animal_t;

typedef struct
{
	int bowles;
	char* status;
	int num_eaten;
}food_area;

#define CAT "cat"
#define DOG "dog"
#define MOUSE "mouse"

void printHelp(); 
int nextBowle(char* status); 
#endif
