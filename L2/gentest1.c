#include <stdio.h>
#include "genstacklib.h"

int main(void)
{
	int val;
	//int* ptr; 
	genStack IntegerStack;
	int elems = 6;

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
	
	printf("Debug: _________________________________________\n");
	printf("Debug: Stack size of %i\n", IntegerStack.logLength);
	printf("Debug: _________________________________________\n");


	while(GenStackEmpty(&IntegerStack) == false){ 
	  GenStackPop(&IntegerStack, &val); 
	  printf("Debug: Client Popp das Ding wech!: %d\n",val);
	}

	GenStackDispose(&IntegerStack); 
}
