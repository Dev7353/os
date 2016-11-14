#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "genstacklib.h"

void freefn (void *element);

typedef struct
{
	char *name;
	char *description;
	int num;


} Person;

int main (void)
{
	genStack structStack;
	Person person;
	char *pool[] = {"Tony", "Thor", "Stephen"};
	char *desc[] = {"Talks to Jarvis and flies in a metal suite",
			"He's a damn god. Close enough.",
			"Mr.Doctor"};
	
	GenStackNew(&structStack, sizeof(Person), freefn);
	
	for(int i = 0; i < 3; ++i)
	{
		if(i == 0)
		{
			person.name = strdup(pool[i]);
			person.description = strdup(desc[i]);
			person.num = i;
		}
		else if(i == 1)
		{
			person.name = strdup(pool[i]);
			person.description = strdup(desc[i]);
			person.num = i;
		}
		else
		{
			person.name = strdup(pool[i]);
			person.description = strdup(desc[i]);
			person.num = i;
		}
		
		GenStackPush(&structStack, &person);
		printf("Push %s on Stack\n", person.name);
	}
	
	Person *popped = (Person*) malloc(sizeof(Person));
	for(int i = 0; i < 3; ++i)
	{
		GenStackPop(&structStack, popped);
		printf("Popped Number %i: %s: %s\n",popped->num, popped->name, popped->description);
		free(popped->name);
		free(popped->description);
		
	}
	free(popped);

	GenStackDispose(&structStack);
	
	return 0;
}

void freefn (void *element)
{
	Person *helper = (Person*) element;
	printf("Offcourse i don't forget about you: %s: %s\n", helper->name, helper->description);
	free (helper->name);
	free (helper->description);
}
