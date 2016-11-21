#include <stdio.h>
#include <stdlib.h>
#include "multiply_matrix.h"

int main (int argc, char* argv[])
{

	Matrix m1, m2, m3;
	int i,j;

	m1 = *readMatrix("matrix0");
	m2 = *readMatrix("matrix1");
	
	m3 = *multiplyMatrix(&m1, &m2,1);
	
	for(i = 0; i < m3.rows; ++i)
	{
		for(j = 0; j < m3.rows; ++j)
		{
			printf("[%lf] \t", m3.matrix[i][j]);
		}
		
		printf("\n");
		
	}
}
