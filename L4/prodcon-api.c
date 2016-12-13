#include "prodcon-api.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


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
	while(fgets(string, 32, file))
	{
		if(i == buffer->storage)
			break;
		if(*string != ' ')
		{
			if(buffer->tail == buffer->storage)
				break;
			memcpy(cache, string, buffer->stringLength+1);
			cache[buffer->stringLength] = '\0';
			//printf("insert %s\n", cache);
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

