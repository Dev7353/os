#include <stdio.h>
#include <"genstacklib.h">

int main(void)
{
	int val; 
	genStack IntegerStack;

	GenStackNew(&IntegerStack, sizeof(int), Null); 

	for (val = 0; val < 6; val++)
	  GenStackPush(&IntegerStack, &val);

	while(!GenStackEmpty(&IntegerStack)){ 
	  GenStackPop(&IntegerStack, &val); 
	  printf("Popped: %d\n",val);
	}

	GenStackDispose(&IntegerStack); 
}
