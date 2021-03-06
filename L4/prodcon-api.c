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

void
readStdin (Buffer * buffer)
{
	char *string = (char *) malloc (sizeof (char) * buffer->stringLength);
	char *cache =
		(char *) malloc (sizeof (char) * buffer->stringLength + 1);
	int i = 0;
	while (((scanf (" %[^\n]s", string)) != EOF)
	       && buffer->isFull == false)
	{
		memcpy (cache, string, buffer->stringLength + 1);
		cache[buffer->stringLength] = '\0';
		add (buffer, cache);
		++i;
	}

	free (string);
}

void
readFile (Buffer * buffer, char *filename)
{
	FILE *file;
	char *string = (char *) malloc (sizeof (char) * 32);
	assert (string != NULL);
	char *cache;
	cache = (char *) malloc (sizeof (char) * buffer->stringLength + 1);
	assert (cache != NULL);
	file = fopen (filename, "r");
	assert (file != NULL);
	int i = 0;
	while (fgets (string, 1024, file))
	{
		if (i == buffer->storage)
			break;
		if (*string != ' ')
		{
			if (buffer->tail == buffer->storage)
				break;
			memcpy (cache, string, buffer->stringLength + 1);
			cache[buffer->stringLength] = '\0';

			add (buffer, cache);
			++i;
		}
	}
	fclose (file);

	free (cache);
	free (string);
}

void
printBuffer (Buffer * buffer)
{
	for (int i = 0; i < buffer->tail; ++i)
		printf ("|%s|\n", buffer->queue[i]);
}

void
printIds (int threads)
{
	printf ("(");
	for (int i = 0; i < threads; ++i)
		printf ("%d, ", i);

	printf (")\n");
}

int
nextThread (int threads, int access[])
{
	for (int i = 0; i < threads; ++i)
	{
		if (access[i] == false)
			return i;
	}

	return -1;
}

bool
threadsAreDone (int threads, int access[])
{
	for (int i = 0; i < threads; ++i)
	{
		if (access[i] == false)
			return false;
	}

	return true;
}

void
refreshThreads (int threads, int access[])
{
	for (int i = 0; i < threads; ++i)
	{
		access[i] = false;
	}
}

void
printHelp()
{
printf ("Help\n");
	printf("-h --help \t Help\n");
	printf("-i --input \t Input filename or path to file\n");
	printf("-o --output \t Output filename or path to file\n");
	printf("-L --lines \t Number of Lines which stored in the buffer\n");
	printf("-C --columns \t Size of Strings\n");
	printf("-c --consumer \t Number of Consumers\n");
	printf("-p --producer \t Number of producers\n");
	printf("-t --time \t Delay of Producers\n");
	printf("-r --lower \t Lower Border to generate Random Number\n");
	printf("-r --upper \t Upper Border to generate Random Number\n");
	printf("-a --busy \t Activates busyloop in addition of -r and -R\n");
	printf("============================================================\n");
	printf ("Current Git HEAD commit number: \n");
	const char *gitversion = "git rev-parse HEAD";
	system (gitversion);
}
