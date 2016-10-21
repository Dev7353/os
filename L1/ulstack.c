# include "ulstack.h"
# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

int stack_created = 0;

void ULStackNew(ulstack *s)
{
    s->elems = (unsigned long *) malloc(STACK_CAPACITY * sizeof(unsigned long));
    assert(s->elems != NULL);
    
    s->logLength = 0; 
    s->allocLength = STACK_CAPACITY;
    
}
void ULStackDispose(ulstack *s)
{
    assert(s->allocLength != 0);
    free(s->elems);
    s->elems = NULL;
    s->allocLength = 0;
    s->logLength = 0;
    
}
void ULStackPush(ulstack *s, unsigned long value)
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
unsigned long ULStackPop(ulstack *s)
{
    unsigned long value = s->elems[s->logLength-1];
    s->logLength--;
    
    return value;
}
unsigned int GetULStackNumberOfElements(ulstack *s)
{
    return s->logLength;
}
