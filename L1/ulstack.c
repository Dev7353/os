# include "ulstack.h"
# include <stdlib.h>
# include <stdio.h>
# define STACK_CAPACITY 4

// Should i set a tag on the master if i'm done?
void ULStackNew(ulstack *s)
{
    s->elems = (unsigned long *) malloc(STACK_CAPACITY * sizeof(unsigned long));

    if(s->elems == NULL)
    {
        fprintf(stderr, "Memory allocation failure!\n");
        exit(1);
    }
    s->logLength = 0;
    s->allocLength = 4;
    
}
void ULStackDispose(ulstack *s)
{
    free(s->elems);
    free(s);
}
void ULStackPush(ulstack *s, unsigned long value)
{
    // if logLenght  == allocLength then extend the elems pointer allocation #realloc
    if(s->logLength == s->allocLength)
    {
        unsigned long *tmp = (unsigned long *) realloc(s->elems, STACK_CAPACITY*2* sizeof(unsigned long));
        if(tmp == NULL)
        {
            fprintf(stderr, "Memory reallocation failure!\n");
            exit(1);
        }
    }
    
    // it is forbidden to push more than 4 elements but it is possible. why? Because the higher indices are not 'controlled'
    s->elems[s->logLength]= value;
    ++s->logLength;
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
