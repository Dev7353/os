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
	
	buffer->elements = 0;
	buffer->isEmpty = true;
	buffer->isFull = false;
	buffer->storage = rows;
	buffer->stringLength = cols;
	
}

void add(Buffer* buffer, char* element)
{
	assert(buffer->isFull == false);
	int n = next(buffer);
	printf	("%d\n", n);
	assert(n > -1);
	
	memcpy(buffer->queue[n], element, sizeof(char)* buffer->stringLength);
	buffer->isEmpty = false;
	if(buffer->storage == buffer->elements)
		buffer->isFull = true;
	
	buffer->elements++;
}

char* pop(Buffer* buffer)
{
	
	int h = head(buffer);
	assert(h != -1);
	char* popped = (char*) malloc(sizeof(char) * buffer->stringLength);
	memcpy(popped, buffer->queue[h], buffer->stringLength);
	char* empty = "";
	add(buffer, empty);
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

int next(Buffer* buffer)
{
		if(buffer->elements == 0)
			return 0;
			
		for(int i = 0; i < buffer->elements; ++i)
		{
				printf("DEBUG queue %s\n", buffer->queue[i]);
				if(*buffer->queue[i+1] == '\n')
					printf("frei\n");
		}
		return -1;
}

int head(Buffer* buffer)
{
	if(buffer->elements == 0)
			return 0;
			
		for(int i = 0; i < buffer->elements; ++i)
		{
				if(*buffer->queue[i] != '\n')
					return i;
		}
		return -1;
}
