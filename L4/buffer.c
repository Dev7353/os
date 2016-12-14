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
	buffer->tail = 0;
	
}

void add(Buffer* buffer, char* element)
{
	if(*element == '\0')
		return;
	memcpy(buffer->queue[buffer->tail], element, sizeof(char)* buffer->stringLength);
	buffer->isEmpty = false;
	if(buffer->storage == buffer->head)
		buffer->isFull = true;

	++buffer->tail;
}

char* pop(Buffer* buffer)
{
	if(buffer->isEmpty)
		return 0;
	char* popped = (char*) malloc(sizeof(char) * buffer->stringLength);
	memcpy(popped, buffer->queue[buffer->head], buffer->stringLength);
	
	if(buffer->head < buffer->tail)
		++buffer->head;
	
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


