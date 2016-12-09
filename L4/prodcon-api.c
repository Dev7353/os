#include "prodcon-api.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void readStdin(void* b)
{
	Buffer* buffer = (Buffer*) b;
	char* string = (char*) malloc(sizeof(char) * buffer->stringLength);
	int i = 0;
	while(((scanf(" %[^\n]s", string)) != EOF) && buffer->isFull == false)
	{
		add(buffer, string);
		++i;	
	}

	free(string);
}

void readFile(void* b, char* filename)
{
	Buffer* buffer = (Buffer*) b;
	FILE* file;
	char* string = (char*) malloc(sizeof(char) * buffer->stringLength);
	file = fopen(filename, "r");
	assert(file != NULL);
	int i = 0;
	while(fgets(string, buffer->stringLength, file))
	{
		add(buffer, string);
		++i;
	}
	fclose(file);
	
	free(string);
}

void printBuffer(void* b)
{
	Buffer* buffer = (Buffer*) b;

	for(int i = 0; i < buffer->storage; ++i)
		printf("%s", buffer->queue[i]);
}
