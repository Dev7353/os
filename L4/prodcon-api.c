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
	char* string = (char*) malloc(sizeof(char) * buffer->stringLength);
	file = fopen(filename, "r");
	assert(file != NULL);
	int i = 0;
	while(fgets(string, buffer->stringLength, file))
	{
		if(*string != ' ')
		{
			add(buffer, string);
			++i;
		}
	}
	fclose(file);
	
	free(string);
}

void printBuffer(Buffer* buffer)
{
	for(int i = 0; i < buffer->head; ++i)
		printf("%s\n", buffer->queue[i]);
}

