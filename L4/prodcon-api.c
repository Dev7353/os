#include "prodcon-api.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool verbose = false;	

int consumerThreads = 1;
int operations = 0;

int producerThreads = 1;
int additions = 0;
int turn = 0;
bool complete = false;
int busyLoopFactor = 0;

void readStdin(Buffer* buffer)
{
	char* string = (char*) malloc(sizeof(char) * buffer->stringLength);
	int i = 0;
	while(((scanf(" %[^\n]s", string)) != EOF) && buffer->isFull == false)
	{
		add(buffer, string);
		++i;	
	}

	free(string);
}

void readFile(Buffer* buffer, char* filename)
{
	FILE* file;
	char* string = (char*) malloc(sizeof(char) * 32);
	assert(string != NULL);
	char *cache;
	cache = (char*) malloc(sizeof(char) * buffer->stringLength+1);
	assert(cache != NULL);
	file = fopen(filename, "r");
	assert(file != NULL);
	int i = 0;
	while(fgets(string, 1024, file))
	{
		if(i == buffer->storage)
			break;
		if(*string != ' ')
		{
			if(buffer->tail == buffer->storage)
				break;
			memcpy(cache, string, buffer->stringLength+1);
			cache[buffer->stringLength] = '\0';
			
			add(buffer, cache);
			++i;
		}
	}
	fclose(file);
	
	free(cache);
	free(string);
}

void printBuffer(Buffer* buffer)
{
	for(int i = 0; i < buffer->tail; ++i)
		printf("|%s|\n", buffer->queue[i]);
}

void printIds(int threads)
{
	printf("(");
	for(int i = 0; i < threads; ++i)
		printf("%d, ", i);
	
	printf(")\n");
}

int nextThread(int threads, int access[])
{
	for(int i = 0; i < threads; ++i)
	{
		if(access[i] == false)
			return i;
	}
	
	return -1;
}

bool threadsAreDone(int threads, int access[])
{
	for(int i = 0; i < threads; ++i)
	{
		if(access[i] == false)
			return false;
	}
	
	return true;
}
void refreshThreads(int threads, int access[])
{
	for(int i = 0; i < threads; ++i)
	{
		access[i] = false;
	}
}



