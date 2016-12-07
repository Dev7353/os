#include "prodcon-api.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>


void readStdin(void* b)
{
	Buffer* buffer = (Buffer*) b;
	char* string = (char*) malloc(sizeof(char) * buffer->stringLength);
	int row = 0;
	while(((scanf(" %[^\n]s", string)) != EOF) && buffer->isFull == false)
	{
		add(buffer, string, row);
		++row;	
	}

	free(string);
}

void readFile(void* b)
{
	//todo
}

void printBuffer(void* b)
{
	Buffer* buffer = (Buffer*) b;

	for(int i = 0; i < buffer->elements; ++i)
		printf("%s\n", buffer->queue[i]);
}
