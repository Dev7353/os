#include <stdio.h>
#include "buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void initBuffer(Buffer* buffer, int rows, int cols)
{
	assert(rows > 0 && cols > 0);

	buffer->queue = (char**) malloc(sizeof(char*) * rows);
	for(int i = 0; i < rows; ++i)
	{
		buffer->queue[i] = (char*) malloc(sizeof(char) * cols);
	}
	
	buffer->elements = 0;
	buffer->isEmpty = true;
	buffer->isFull = false;
	buffer->storage = rows;
	buffer->stringLength = cols;
	
}

void add(Buffer* buffer, char* element, int row)
{
	if(strcmp(element, "") != 0) //empty strings will ignored
		buffer->elements++;
	assert(row >= 0);
	assert(buffer->isFull == false);
	memcpy(buffer->queue[row], element, sizeof(char)* buffer->stringLength);
	buffer->isEmpty = false;
	if(buffer->storage == buffer->elements)
		buffer->isFull = true;
}

char* pop(Buffer* buffer, int row)
{
	assert(row >= 0);
	char* popped = (char*) malloc(sizeof(char) * buffer->stringLength);
	strncpy(popped, buffer->queue[row], buffer->stringLength);
	char* empty = "";
	add(buffer, empty, row);
	buffer->elements--;
	
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
