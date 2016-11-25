#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include "multiply_matrix.h"

#define BILLION 1000000000
#define MILLIONTH 0.000001
#define false 0
#define true 1

long diff(struct timespec t1, struct timespec t2);

int main (int argc, char* argv[])
{
	char* filea;
	char* fileb;
	int multithreading = false;
	int threads = 1;
	int c;	

	while ((c = getopt (argc, argv, "a:b:pt:")) != -1)
	{
		switch (c)
	      {
		case 'a':
			filea = optarg;
			break;
		case 'b':
			fileb = optarg;
			break;
		case 'p':
			multithreading = true;
			break;
		case 't':
			threads = atoi(optarg);
			break;
		case '?':
			if (optopt == 'a')
		  		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			if (optopt == 'b')
		  		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			if (optopt == 't')
		  		fprintf (stderr, "Option -%c requires an argument.\n", optopt);

			else if (isprint (optopt))
		  		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
		 		 fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
        		return 1;
		default:
			abort();
	      }

	}

	printf("FILE A \t\t %s\n", filea);
	printf("FILE B \t\t %s\n", fileb);
	char* mt;
	if(multithreading == true)
		mt = "an";
	else
		mt = "aus";
	printf("MULTITHREADING \t %s\n", mt);
	printf("THREADS \t %d\n", threads);

	Matrix *m1, *m2, *m3;
	int i,j;
	struct timespec t1, t2;

	m1 = readMatrix(filea);
	m2 = readMatrix(fileb);
	
	clock_gettime(CLOCK_MONOTONIC, &t1);
	m3 = multiplyMatrix(m1, m2, threads);
	clock_gettime(CLOCK_MONOTONIC, &t2);

	printf("Time : %ld [msec]\n", diff(t1, t2));	

	FILE *fp;
	fp = fopen("result", "w");
	char *number = (char*) malloc(sizeof(long double));
	char tab = '\t';
	char newline = '\n';

	// copy the whole data from file  into matrix
	for(i = 0; i < m3->rows; ++i)
	{
		for(j = 0; j < m3->rows; ++j)
		{
			sprintf(number, "%lf", m3->matrix[i][j]);
			fputs(number, fp);
			fputc(tab, fp);

		}
		fputc(newline, fp);
		
	}
	fclose(fp);
	
	// debug information 
	int empty = 0;
	for(i = 0; i < m3->rows; ++i)
	{
		for(j = 0; j < m3->rows; ++j)
		{
			if(m3->matrix[i][j] == 0)
				++empty;
		}
	}
	
	printf("%d empty cells\n", empty);
	
	//free allocated memory
	for(int i = 0; i < m3->rows; ++i)
	{
		free(m1->matrix[i]);
		free(m2->matrix[i]);
		free(m3->matrix[i]);
	}
	
	free(m1->matrix);
	free(m2->matrix);
	free(m3->matrix);
	
	free(m1);
	free(m2);
	free(m3);
	free(number);
}

long diff(struct timespec t1, struct timespec t2)
{
	long t1nsec = t1.tv_sec * BILLION + t1.tv_nsec;
	long t2nsec = t2.tv_sec * BILLION + t2.tv_nsec;
	
	return ((t2nsec - t1nsec) * MILLIONTH);
}
