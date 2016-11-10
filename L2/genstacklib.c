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
        s->elems = (void *) realloc(s->elems, s->allocLength * 2 *  s->elemSize);
		s->allocLength *= 2;
		assert(s->elems != NULL);
    }
    
    /*printf("Adress from genstack: %p\n", (void*)elemAddr);
    printf("Adress from genstack: %08x\n", (int)elemAddr);*/
    
    
    char *ptr1 = (char*) *(&s->elems + s->logLength * s->elemSize);
    char *ptr2 = (char*) elemAddr;
    
    for(int i = 0; i  < s->elemSize; ++i)
    {
			ptr1[i] = ptr2[i];
	}

    
    //printf("ptr1 %i\n", (int) *ptr1);
    //printf("ptr2 %i\n", (int) *ptr2);
    
    s->logLength++;
}
void GenStackPop(genStack *s, void *elemAddr)
{
	elemAddr = &(*(&s->elems + (s->logLength * s->elemSize - 1)));
	*(&s->elems + (s->logLength * s->elemSize - 1)) = NULL;
    s->logLength--;
    
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

