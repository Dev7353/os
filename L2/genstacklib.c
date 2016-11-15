#include "genstacklib.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

bool stack_created = false;

void
GenStackNew (genStack * s, int elemSize, void (*freefn) (void *))
{
  errno = 0;
  s->elems = malloc (GenStackInitialAllocationSize * elemSize);
  assert (s->elems != NULL);
  if (errno != 0)
    printf ("ERROR SYSCALL %d\n", errno);

  s->elemSize = elemSize;
  s->logLength = 0;
  s->allocLength = GenStackInitialAllocationSize;
  s->freefn = freefn;

  stack_created = true;
}

void
GenStackDispose (genStack * s)
{
  assert (stack_created == true);

  if (s->freefn != NULL)
    {
      while (GenStackEmpty (s) == false)
	{
	  errno = 0;
	  void *leftOnStack = malloc (s->elemSize);
	  assert (leftOnStack != NULL);

	  if (errno != 0)
	    printf ("ERROR SYSCALL %d\n", errno);

	  GenStackPop (s, leftOnStack);
	  s->freefn (leftOnStack);
	  free (leftOnStack);
	}
    }
  free (s->elems);
  s->elems = NULL;		//just to be sure
}

void
GenStackPush (genStack * s, const void *elemAddr)
{
  assert (stack_created == true);

  if (s->logLength == s->allocLength)
    {
      s->elems = realloc (s->elems, s->allocLength * 2 * s->elemSize);
      s->allocLength *= 2;
      assert (s->elems != NULL);
    }

  char *ptr1 = (char *) s->elems;
  // get current position in stack
  ptr1 += s->logLength * s->elemSize;

  char *ptr2 = (char *) elemAddr;

  memmove (ptr1, ptr2, s->elemSize);

  s->logLength++;
}

void
GenStackPop (genStack * s, void *elemAddr)
{
  assert (stack_created == true);

  s->logLength--;
  char *ptr1 = (char *) s->elems;
  // get current position in stack
  ptr1 += (s->logLength) * s->elemSize;

  char *ptr2 = (char *) elemAddr;

  memmove (ptr2, ptr1, s->elemSize);

}

bool
GenStackEmpty (const genStack * s)
{
  assert (stack_created == true);

  if (s->logLength == 0)
    {
      return true;
    }
  else
    {

      return false;
    }
}
