#include <stdio.h>
#include "genstacklib.h"

int main(void)
{
	int val; 
	genStack IntegerStack;

	GenStackNew(&IntegerStack, sizeof(int), NULL); 

	for (val = 0; val < 6; val++)
	  GenStackPush(&IntegerStack, &val);
	  /*Dereferenzierung u zu chekcn ob die werte tatsächlich gepusht wurden*/
		int* ptr;
		ptr = (int*) &IntegerStack.elems;
		printf("Round %d, Push %i\n", val, ptr[val]);

	while(!GenStackEmpty(&IntegerStack)){ 
	  GenStackPop(&IntegerStack, &val); 
	  printf("Popped: %d\n",val);
	}

	GenStackDispose(&IntegerStack); 
}
