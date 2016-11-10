#include "genstacklib.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int stack_created = 0;

void GenStackNew(genStack *s, int elemSize, void(*freefn)(void*))
{
    s->elems = malloc(GenStackInitialAllocationSize * elemSize	);
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
    
    
    char *ptr1 = (char*) *(&s->elems + (s->logLength * s->elemSize));
    char *ptr2 = (char*) elemAddr;
    
    printf("INhalt von elemAddr: %i\n", (int) *ptr2);
    char *ptr;
    ptr = (char*) malloc(sizeof(int));
    assert(ptr != NULL);
    ptr[0] = ptr2[0];
    ptr[1] = ptr2[1];
    ptr[2] = ptr2[2];
    
    ptr1 = ptr;
    
    int *h = (int*) ptr;
    printf(">> %i\n", *h);
        
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

