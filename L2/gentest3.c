#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "genstacklib.h"

void freefn (void *element);

typedef struct
{
	char *name;
	int age;
} Person;

int main (void)
{
	genStack structStack;
	Person person;
	char *pool[] = {"Tony", "Thor", "Stephen"};
	
	GenStackNew(&structStack, sizeof(Person), freefn);
	
	for(int i = 0; i < 3; ++i)
	{
		if(i == 0)
		{
			person.name = strdup(pool[i]);
			person.age = i;
		}
		else if(i == 1)
		{
			person.name = strdup(pool[i]);
			person.age = i;
		}
		else
		{
			person.name = strdup(pool[i]);
			person.age = i;
		}
		
		GenStackPush(&structStack, &person);
		printf("Push Mr.%s on Stack\n", person.name);
	}
	
	Person popped;
	for(int i = 0; i < 3; ++i)
	{
		GenStackPop(&structStack, &popped);
		printf("Byebye %i Mr.%s\n",popped.age, popped.name);
		free(&popped);
	}

	GenStackDispose(&structStack);
	
	return 0;
}

void freefn (void *element)
{
	
	free (((Person *) (element))->name);
}
