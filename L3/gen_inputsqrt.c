#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#define MAX 2147483647		// 2^31 -1
#define MAX_LENGTH 9 //BILLION
#define MIN 0			// ni divsion trough 0

int randInteger (int min, int max);

int
main (int argc, char *argv[])
{
	assert (argc == 2);
	srand (time (NULL));
	FILE *fp;
	int amount, i;
	char *number;
	char newline;

	newline = '\n';
	number = (char *) malloc (sizeof (int)*MAX_LENGTH);
	fp = fopen ("input", "w");
	amount = atoi (argv[argc - 1]);
	for (i = 0; i < amount; ++i)
	{
		sprintf (number, "%d", randInteger (MIN, MAX));
		fputs (number, fp);
		fputc (newline, fp);
	}
	fclose (fp);
	free (number);
	return 0;
}

int
randInteger (int min, int max)
{
	return (random () % max) + min;
}
