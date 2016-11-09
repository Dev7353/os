#include "genstacklib.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int stack_created = 0;

void GenStackNew(genStack *s, int elemSize, void(*freefn)(void*))
{
    s->elems = (void *) malloc(GenStackInitialAllocationSize * elemSize);
    assert(s->elems != NULL);
    
    s->logLength = 0; 
    s->allocLength = GenStackInitialAllocationSize;
    
}
void GenStackDispose(genStack *s)
{

    
}
void GenStackPush(genStack *s, const void *elemAddr)
{

    
}
void GenStackPop(genStack *s, void *elemAddr)
{
	
}
bool GenStackEmpty(const genStack *s)
{
	return true;
}

