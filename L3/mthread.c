#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

typedef int boolean;
#define true 1;
#define false 0;

pthread_mutex_t mutex;

typedef struct arg
{
	int number;
	int upper;
	int lower;

} arg;

void *PrintHello (void *threadarg);

/*Parsing based on https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt*/
time_t sec;
int
main (int argc, char *argv[])
{
	
	srandom ((unsigned int) sec);
	// initialize and declare default values
	boolean infos = false;
	int status;
	long t;
	long NumberOfThreads = 4;
	int c;			//argument

	int lowerBorder = 0;
	int upperBorder = 10;

	while ((c = getopt (argc, argv, "hvt:r:R:")) != -1)
	{
		switch (c)
		{
		case 't':
			NumberOfThreads = atoi (optarg);
			if (NumberOfThreads < 0)
			{
				perror ("No negativ threads");
				NumberOfThreads = (-1) * NumberOfThreads;
			}
			break;
		case 'h':
			printf ("Help: \n-v, --verbose \t\t get more informations while running\n-h, --help  \t\tget help\n-t --times \t\tcreate t times threads\n -r \t\t\t under random border \n -R \t\t\t upper random border\n");
			printf ("Current Git HEAD commit number: \n");
			const char *gitversion = "git rev-parse HEAD";
			system (gitversion);
			return 0;
		case 'v':
			infos = true;
			break;
		case 'r':
			lowerBorder = atoi (optarg);
			if (lowerBorder < 0)
			{
				perror ("No negative time");
				lowerBorder = (-1) * atoi (optarg);
			}
			break;
		case 'R':
			upperBorder = atoi (optarg);
			if (upperBorder < 0)
			{
				perror ("No negative time");
				upperBorder = (-1) * atoi (optarg);
			}
			else if (upperBorder <= lowerBorder)
			{
				perror ("Upper border cannot be smaller then lower border");
				upperBorder = lowerBorder;
				upperBorder++;
			}
			break;
		case '?':
			if (optopt == 't')
				fprintf (stderr,
					 "Option -%c requires argument.\n",
					 optopt);
			else if (optopt == 'r')
				fprintf (stderr,
					 "Option -%c requires argument.\n",
					 optopt);
			else if (optopt == 'R')
				fprintf (stderr,
					 "Option -%c requires argument.\n",
					 optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n",
					 optopt);
			else
				fprintf (stderr,
					 "Unknown option character `\\x%x'.\n",
					 optopt);
			return 1;
		default:
			abort ();
		}
	}


	arg args[NumberOfThreads];

	errno = 0;

	pthread_t threads[NumberOfThreads];
	for (t = 0; t < NumberOfThreads; t++)
	{
		args[t].number = t;
		args[t].upper = upperBorder;
		args[t].lower = lowerBorder;
		printf ("Creating thread %ld\n", t);
		if (pthread_create (&threads[t], NULL, PrintHello, &args[t])
		    != 0)
		{
			perror ("creating Thread failed\n");
			if (infos)
				printf ("Errno %d\n", errno);
			exit (1);
		}

	}

	for (t = 0; t < NumberOfThreads; t++)
	{
		printf ("Main joining with thread %ld\n", t);
		if (pthread_join (*(&threads[t]), (void *) (&status)) != 0)
		{
			perror ("join failed\n");
			if (infos)
				printf ("Errno %d\n", errno);
			exit (1);
		}

		printf ("Completed joining with thread %ld status = %d\n", t,
			status);
	}
}

void *
PrintHello (void *threadarg)
{

	arg ar = *((arg *) threadarg);
	printf ("%d: Hello World\n", ar.number);
	time (&sec);
	int s = (random () % ar.upper) + ar.lower;
	sleep (s);

	printf ("%d: Thread is done after sleeping %d[s]\n", ar.number, s);

	return NULL;
}
