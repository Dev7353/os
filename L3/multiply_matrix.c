#include "multiply_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void calcValue(Matrix *a, Matrix *b, Matrix *result);

Matrix *readMatrix(const char filename[])
{
	FILE *fp;

	fp =  fopen(filename, "r");
	if(fp == NULL)
		exit(1);

	Matrix *m = (Matrix*) malloc(sizeof(Matrix));
	if(m == NULL)
		perror("malloc");	
	
	fscanf(fp, "%d", &m->rows);
	fscanf(fp, "%d", &m->columns);

	m->matrix = (double**) malloc(m->rows * sizeof(double*));
	if(m->matrix == NULL)
		perror("malloc");
	for(int i = 0; i < m->columns;++i)
	{
		m->matrix[i] = (double*) malloc(m->columns * sizeof(double));
		if(m->matrix[i] == NULL)
			perror("malloc");
	}

	for(int i = 0; i < m->rows; ++i)
	{
			for(int j = 0; j < m->columns; ++j)
			{
				fscanf(fp, "%lf", &m->matrix[i][j]);
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
	

	return m;
}

Matrix *multiplyMatrix(Matrix *a, Matrix *b, int threads)
{
	int i,j,k;
	
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
		result->matrix[i] = (double*) malloc(result->columns * sizeof(double));
		if(result->matrix[i] == NULL)
			perror("malloc");
	}
	
	//initialize result matrix with 0
	for(i = 0; i < a->rows; ++i)
	{
		for(j = 0; j < a->rows; ++j)
		{
			result->matrix[i][j] = 0;
		}
		
	}
	
	for(i = 0; i < thread; ++i)
	{
		pthread thread;

		pthread_create(&thread, NULL, calcValue, );
		pthread();

	}


	return result;
}

double multiplyRowColumn(Matrix *a, int row, Matrix *b, int column)
{
	return 0;
}

void calcValue(Matrix *a, Matrix *b, Matrix *result)
{

	for(i = 0; i < thread; ++i)
    {
        for(j = 0; j < a->rows; ++j)
        {
            for(k = 0; k < a->rows; ++k)
            {
                res->matrix[i][j] += a->matrix[i][k] * b->matrix[k][j]; 
            }
        }
               


    }


}

