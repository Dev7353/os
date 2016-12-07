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
	assert(row >= 0);
	assert(buffer->isFull == false);
	memcpy(buffer->queue[row], element, sizeof(char)* buffer->stringLength);
	buffer->isEmpty = false;
	buffer->elements++;
	if(buffer->storage == buffer->elements)
		buffer->isFull = true;
}

char* pop(Buffer* buffer)
{
	return "Todo";
}

void readStdin(Buffer* buffer)
{
	char* string = (char*) malloc(sizeof(char) * buffer->stringLength);
	int row = 0;
	while(((scanf(" %[^\n]s", string)) != EOF) && buffer->isFull == false)
	{
		add(buffer, string, row);
		++row;	
	}

	free(string);
}

void destroyBuffer(Buffer* buffer)
{
	for(int i = 0; i < buffer->storage; ++i)
	{
		free(buffer->queue[i]);
	}
	free(buffer->queue);
}
