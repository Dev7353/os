#include "multiply_matrix.h"
#include <stdlib.h>
#include <stdio.h>

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
	return NULL;
}

double multiplyRowColumn(Matrix *a, int row, Matrix *b, int column)
{
	return 0;
}

