# include "ulstack.h"
# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

int stack_created = 0;

void ULStackNew(ulstack *s)
{
    s->elems = (unsigned long *) malloc(STACK_CAPACITY * sizeof(unsigned long));

    if(s->elems == NULL)
    {
        assert(s->elems != NULL);
    }
    s->logLength = 0; 
    s->allocLength = STACK_CAPACITY;
    
}
void ULStackDispose(ulstack *s)
{
    assert(s->allocLength != 0);
    free(s->elems);
    s->allocLength = 0;
    s->logLength = 0;
    
}
void ULStackPush(ulstack *s, unsigned long value)
{
    if(s->logLength == s->allocLength)
    {
        s->elems = (unsigned long *)realloc(s->elems, STACK_CAPACITY*2* sizeof(unsigned long));
        assert(s->elems != NULL);
    }
    
    s->elems[s->logLength]= value;
    ++s->logLength;
    
    assert(s->elems[s->logLength-1] == value); // test bevor logLength was incremented.
}
unsigned long ULStackPop(ulstack *s)
{
    unsigned long value = s->elems[s->logLength-1];
    --s->logLength;
    
    return value;
}
unsigned int GetULStackNumberOfElements(ulstack *s)
{
    return s->logLength;
}
