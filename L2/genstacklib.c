#include "genstackstack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int stack_created = 0;

void GenStackNew(genStack *s, int elemSize, void(*freefn)(void*))
{
    s->elems = (unsigned long *) malloc(STACK_CAPACITY * sizeof(unsigned long));
    assert(s->elems != NULL);
    
    s->logLength = 0; 
    s->allocLength = STACK_CAPACITY;
    
}
void GenStackDispose(genStack *s)
{
    assert(s->allocLength != 0);
    free(s->elems);
    s->elems = NULL;
    s->allocLength = 0;
    s->logLength = 0;
    
}
void GenStackPush(genStack *s, const void *elemAddr)
{
    if(s->logLength == s->allocLength)
    {
        s->elems = (unsigned long *) realloc(s->elems, s->allocLength * 2 *  sizeof(unsigned long));
	s->allocLength *= 2;
	assert(s->elems != NULL);
    } 
    s->elems[s->logLength]= value;
    s->logLength++;
    
}
int GenStackPop(genStack *s, const void *elemAddr)
{
    unsigned long value = s->elems[s->logLength-1];
    s->logLength--;
    
    return value;
}
bool GenStackEmpty(const genStack *s)
{
	return true;
}

