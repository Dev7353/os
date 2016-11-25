#include "multiply_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

typedef struct args
{
	Matrix *a;
	Matrix *b;
	Matrix *result;
	int start, stop, threads;

} args;

void *calc (void *ar);

Matrix *
readMatrix (const char filename[])
{
	FILE *fp;

	fp = fopen (filename, "r");
	if (fp == NULL)
		exit (1);

	Matrix *m = (Matrix *) malloc (sizeof (Matrix));
	if (m == NULL)
		perror ("malloc");

	int ch, cols = 0;

	do
	{
		ch = fgetc (fp);
		if (ch == '\n')
			++cols;

	}
	while (ch != EOF);

	m->rows = cols;
	m->columns = cols;

	// allocate for double pinter to pointer and then a pointer for that pointer
	m->matrix = (double **) malloc (m->rows * sizeof (double *));
	if (m->matrix == NULL)
		perror ("malloc");
	for (int i = 0; i < m->columns; ++i)
	{
		m->matrix[i] =
			(double *) malloc (m->columns * sizeof (double));
		if (m->matrix[i] == NULL)
			perror ("malloc");
	}
	fclose (fp);
	fp = fopen (filename, "r");	//reset file to the HEAD otherwise valgrind screams

	for (int i = 0; i < m->rows; ++i)
	{
		for (int j = 0; j < m->columns; ++j)
		{
			fscanf (fp, "%lf", &m->matrix[i][j]);
		}
	}

	fclose (fp);

	return m;
}

Matrix *
multiplyMatrix (Matrix * a, Matrix * b, int threads)
{
	Matrix *result = (Matrix *) malloc (sizeof (Matrix));
	if (result == NULL)
		perror ("malloc");

	result->rows = a->rows;
	result->columns = a->columns;

	if (result->rows != result->columns)
		perror ("The matrix multiplication support only nxn matrices\n");

	result->matrix =
		(double **) malloc (result->rows * sizeof (double *));
	if (result->matrix == NULL)
		perror ("malloc");

	for (int i = 0; i < result->columns; ++i)
	{
		result->matrix[i] =
			(double *) calloc (result->columns, sizeof (double));
		if (result->matrix[i] == NULL)
			perror ("malloc");
	}

	args ar[threads];

	for (int i = 0; i < threads; ++i)
	{
		ar[i].a = a;
		ar[i].b = b;
		ar[i].result = result;
		ar[i].threads = threads;
	}


	pthread_t thread[threads];
	errno = 0;
	for (int i = 0; i < threads; ++i)
	{
		ar[i].start = i * (ar[i].a->rows / threads);
		ar[i].stop = (ar[i].a->rows / threads) * (i + 1);
		if (pthread_create (&thread[i], NULL, calc, &ar[i]) != 0)
		{
			printf ("ERRNO %d\n", errno);
			perror ("Thread creation failed\n");
		}
	}


	for (int i = 0; i < threads; ++i)
	{
		if (pthread_join (thread[i], NULL) != 0)
		{
			printf ("ERRNO %d\n", errno);
		}
	}
	return result;
}

double
multiplyRowColumn (Matrix * a, int row, Matrix * b, int column)
{
	double result = 0;
	for (int k = 0; k < a->columns; ++k)
	{
		result += a->matrix[row][k] * b->matrix[k][column];
	}

	return result;
}

void *
calc (void *ar)
{
	args *r = (args *) ar;
	for (int i = r->start; i < r->stop; ++i)
	{
		for (int j = 0; j < r->a->rows; ++j)
		{

			r->result->matrix[i][j] =
				multiplyRowColumn (r->a, i, r->b, j);

		}

	}
	return NULL;
}
