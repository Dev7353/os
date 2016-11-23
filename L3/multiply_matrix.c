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
	int slice;
			
} args;

void *calc(void *ar);
int idx = 0;

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
				//printf("DEBUG >>> %f\n", m->matrix[i][j]);
			}
	}	

	/*
	printf("[DEBUG]");
	for(int i = 0; i < m->rows; ++i)
	{
			for(int j = 0; j < m->columns; ++j)
			{
				printf("%lf\t", m->matrix[i][j]);
			}
			printf("\n");

	}*/
	
	fclose(fp);

	return m;
}

Matrix *multiplyMatrix(Matrix *a, Matrix *b, int threads)
{
	int i,j, status;
	
	Matrix *result = (Matrix*) malloc(sizeof(Matrix));
	if(result == NULL)
		perror("malloc");	
	result->rows = a->rows;
	result->columns = a->columns;

	result->matrix = (double**) malloc(result->rows * sizeof(double*));
	if(result->matrix == NULL)
		perror("malloc");
	for(i = 0; i < result->columns;++i)
	{
		result->matrix[i] = (double*) calloc(result->columns , sizeof(double));
		if(result->matrix[i] == NULL)
			perror("malloc");
	}
	
	args ar;
	ar.a = a;
	ar.b = b;
	ar.result = result;
	ar.slice = (a->rows * a->rows) / threads;	
	
	pthread_t *thread = (pthread_t*) malloc(sizeof(pthread_t) * threads);
	if(thread == NULL)
		perror("malloc");
	
	for(i = 0; i < threads; ++i)
	{
		pthread_create(&thread[i], NULL, calc, &ar);
	}

	for(i = 0; i < threads; ++i)
	{
		
		pthread_join(thread[i], (void*) &status);
	}
	
	idx = 0; //reset index
	
	return result;
}

double multiplyRowColumn(Matrix *a, int row, Matrix *b, int column)
{
	return 0;
}

void* calc(void *ar)
{
	args *r = (args*) ar;
	int i, j, k;
	
	//printf("DEBUG slice size is %d\n", slice);

	while(idx < r->slice)
	{
		for(i = 0;i < r->a->rows; ++i)
		{
			for(j = 0;j < r->a->rows; ++j)
			{
				r->result->matrix[i][j] = 0;	
				for(k = 0; k < r->a->rows; ++k)
				{
					r->result->matrix[i][j] += r->a->matrix[i][k] * r->b->matrix[k][j]; 
				}
			}
		
			++idx;
		}
	}
	return NULL;
			
}

