#include <stdio.h>

int main(void)
{
	void freefn(void* element)
	{
	
	}

	const char *words[]={"foo","bla","misc"};
	genStack stringStack;

	GenStackNew (&stringStack, sizeof(char*), freefn);
	for (int i=0; i<3;i++)
	{
	  char *copy = strdup(words[i]);
	  GenStackPush(&stringStack, &copy);
	}

	char *oneWord;
	for (int i=0; i<3; i++)
	{
	  GenStackPop(&stringStack,&oneWord);
	  printf("%s\n",oneWord);
	  free(oneWord);
	}
	
	GenStackDispose(&stringStack);
}
