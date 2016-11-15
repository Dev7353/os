#include <stdio.h>
#include "genstacklib.h"

#define ELEMENTS 6

int main(void)
{
	int val;
	genStack IntegerStack;
	int elems = ELEMENTS;

	GenStackNew(&IntegerStack, sizeof(int), NULL); 

	for (val = 0; val < elems; ++val)
	{
	  GenStackPush(&IntegerStack, &val);

	} 
	
	int *h =(int*) IntegerStack.elems;
	
	for(int i = 0; i < elems; ++i)
	{	
		printf("Debug: Client has the value %i \n", *(h+i));

	}	
	
	while(GenStackEmpty(&IntegerStack) == false){ 
	  GenStackPop(&IntegerStack, &val); 
	  printf("Debug: Client Popp das Ding wech!: %d\n",val);
	}

	GenStackDispose(&IntegerStack); 
}
