#include <stdio.h>
#include "buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int next(Buffer* buffer); // returns next avaiable space
int head(Buffer* buffer); // returns head (fifo)

void initBuffer(Buffer* buffer, int rows, int cols)
{
	assert(rows > 0 && cols > 0);

	buffer->queue = (char**) malloc(sizeof(char*) * rows);
	for(int i = 0; i < rows; ++i)
	{
		buffer->queue[i] = (char*) malloc(sizeof(char) * cols);
	}
	
	buffer->isEmpty = true;
	buffer->isFull = false;
	buffer->storage = rows;
	buffer->stringLength = cols;
	buffer->head = 0;
	
}

void add(Buffer* buffer, char* element)
{
	assert(buffer->isFull == false);
	
	memcpy(buffer->queue[buffer->head], element, sizeof(char)* buffer->stringLength);
	buffer->isEmpty = false;
	if(buffer->storage == buffer->head-1)
		buffer->isFull = true;

		++buffer->head;
}

char* pop(Buffer* buffer)
{
	if(buffer->isEmpty)
		return 0;
	printf("HEAD with %s %d\n", buffer->queue[buffer->head],  buffer->head);
	char* popped = (char*) malloc(sizeof(char) * buffer->stringLength);
	memcpy(popped, buffer->queue[buffer->head], buffer->stringLength);
	memcpy(buffer->queue[buffer->head], "\0", buffer->stringLength);
	
	if(buffer->head > 0)
		--buffer->head;
	
	return popped;
}

void destroyBuffer(Buffer* buffer)
{
	for(int i = 0; i < buffer->storage; ++i)
	{
		free(buffer->queue[i]);
	}
	free(buffer->queue);
}


