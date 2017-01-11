#ifndef SCHEDULER_API_H
#define SCHEDULER_API_H

#define ANSI_COLOR_RED 		"\x1b[31m"
#define ANSI_COLOR_BLUE 	"\x1b[34m"
#define ANSI_COLOR_GREEN 	"\x1b[32m"
#define ANSI_COLOR_YELLOW  	"\x1b[33m"
#define ANSI_COLOR_RESET   	"\x1b[0m"

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
#define GROUPS 3

void printHelp(); 
int nextBowle(char* status, int bowles); 
#endif
