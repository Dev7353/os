#include "multiply_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct args
{
	Matrix *a; 
	Matrix *b;
	Matrix *result;
	int start, stop, threads;
			
} args;

void *calc(void *ar);

Matrix *readMatrix(const char filename[])
{
	FILE *fp;

	fp =  fopen(filename, "r");
	if(fp == NULL)
		exit(1);

	Matrix *m = (Matrix*) malloc(sizeof(Matrix));
	if(m == NULL)
		perror("malloc");	
	
	int ch, cols = 0;

	do
	{
		ch = fgetc(fp);	
		if(ch == '\n')
			++cols;
	
	} while(ch != EOF);

	m->rows = cols;
	m->columns = cols;

	m->matrix = (double**) malloc(m->rows * sizeof(double*));
	if(m->matrix == NULL)
		perror("malloc");
	for(int i = 0; i < m->columns;++i)
	{
		m->matrix[i] = (double*) malloc(m->columns * sizeof(double));
		if(m->matrix[i] == NULL)
			perror("malloc");
	}

	fp =  fopen(filename, "r"); //reset file to the HEAD

	for(int i = 0; i < m->rows; ++i)
	{
			for(int j = 0; j < m->columns; ++j)
			{
				fscanf(fp, "%lf", &m->matrix[i][j]);
			}
	}	
	
	fclose(fp);

	return m;
}

Matrix *multiplyMatrix(Matrix *a, Matrix *b, int threads)
{
	
	Matrix *result = (Matrix*) malloc(sizeof(Matrix));
	if(result == NULL)
		perror("malloc");	
	result->rows = a->rows;
	result->columns = a->columns;

	result->matrix = (double**) malloc(result->rows * sizeof(double*));
	if(result->matrix == NULL)
		perror("malloc");
	for(int i = 0; i < result->columns;++i)
	{
		result->matrix[i] = (double*) calloc(result->columns , sizeof(double));
		if(result->matrix[i] == NULL)
			perror("malloc");
	}
	
	args ar;
	ar.a = a;
	ar.b = b;
	ar.result = result;
	ar.threads = threads;
	
	
	pthread_t *thread = (pthread_t*) malloc(sizeof(pthread_t) * threads);
	if(thread == NULL)
		perror("malloc");
		
	for(int i = 0; i < threads; ++i)
	{
		ar.start = i*(ar.a->rows/threads);
		ar.stop = (ar.a->rows/threads)*(i+1);
		printf("From %d to %d\n", ar.start, ar.stop);
		pthread_create(&thread[i], NULL, calc, &ar);
	}


	for(int i = 0; i < threads; ++i)
	{
		printf("Join Thread no. %d\n", i);
		pthread_join(thread[i], NULL);
	}
	

	return result;
}

double multiplyRowColumn(Matrix *a, int row, Matrix *b, int column)
{
	double result = 0;
	for(int k = 0; k < a->columns; ++k)
	{
		result += a->matrix[row][k] * b->matrix[k][column]; 			
	}
	
	return result;
}

void* calc(void *ar)
{
	args *r = (args*) ar;	
	int debug = 0;

		for(int i = r->start; i < r->stop; ++i)
		{
			for(int j = 0;j < r->a->rows; ++j)
			{
				r->result->matrix[i][j] = multiplyRowColumn(r->a, i, r->b, j);	
				
				++debug;
			}

		}
		
	printf("%d Calculations\n", debug);
	pthread_exit(0);
			
}

