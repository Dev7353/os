#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "genstacklib.h"

#define POPVALUE 2
void freefn (void *element);

int
main (void)
{

  const char *words[] = { "foo", "bla", "misc" };
  genStack stringStack;

  GenStackNew (&stringStack, sizeof (char *), freefn);

  for (int i = 0; i < 3; i++)
    {
      char *copy = strdup (words[i]);
      GenStackPush (&stringStack, &copy);
    }

  char *oneWord;
  for (int i = 0; i < POPVALUE; i++)	// pop 2 values the third is handled in dispose method
    {
      GenStackPop (&stringStack, &oneWord);
      printf ("%s\n", oneWord);
      free (oneWord);
    }

  GenStackDispose (&stringStack);
}

void
freefn (void *element)
{
  assert (element != NULL);

  char **helper = (char **) element;
  printf ("%s\n", *helper);

  free (*helper);
}
