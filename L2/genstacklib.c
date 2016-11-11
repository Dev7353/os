#include "genstacklib.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int stack_created = 0;

void GenStackNew(genStack *s, int elemSize, void(*freefn)(void*))
{
    s->elems = malloc(GenStackInitialAllocationSize * elemSize);
    assert(s->elems != NULL);
    
    s->elemSize = elemSize;
    s->logLength = 0; 
    s->allocLength = GenStackInitialAllocationSize;
    s->freefn = freefn;
    
}
void GenStackDispose(genStack *s)
{
	int i;
	
	for(i = 0; i < s->logLength; ++i)
	{	
	}
    
}
void GenStackPush(genStack *s, const void *elemAddr)
{
    if(s->logLength == s->allocLength)
    {
        s->elems = realloc(s->elems, s->allocLength * 2 *  s->elemSize);
		s->allocLength *= 2;
		assert(s->elems != NULL);
    }
   
    char *ptr1 = (char*) *(&s->elems + s->logLength * s->elemSize);
    char *ptr2 = (char*) elemAddr;
    
	char helper[s->elemSize];
	
	for(int i = 0; i< s->elemSize; ++i)
		helper[i] = ptr2[i];
	
	//umkopieren von helper nach ptr1
	
	ptr1 = helper;
	
	int *erg = (int*) ptr1;
	printf("Debug: Genstacklib.c pointer has value %i\n", *erg);
    s->logLength++;
}
void GenStackPop(genStack *s, void *elemAddr)
{
	
	 
    s->logLength--;
    exit(1);
}
bool GenStackEmpty(const genStack *s)
{
	if(s->logLength == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

