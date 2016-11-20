#ifndef MULTIPLY_MATRIX_H
#define MULTIPLY_MATRIX_H

struct Matrix {
    int rows;
    int columns;
    double **matrix;
};

typedef struct Matrix Matrix;

/*
 * Liest Matrix aus Datei "filename" aus und gibt diese als Struktur
 * (dynamisch alloziert) zurueck.
 *
 * Return: Pointer auf eingelesene Matrix
 */
Matrix *readMatrix(const char filename[]);

/*
 * Multipliziert Matrix a und b.
 *
 * Parameter threads: Anzahl der Threads die parallel die
 * Multiplikation durchfuehren sollen.
 *
 * Return: Pointer auf Ergebnismatrix
 */
Matrix *multiplyMatrix(Matrix *a, Matrix *b, int threads);

/*
 * Berechnet das Skalarprodukt der jeweiligen Zeile und Spalte der
 * uebergebenen Matrizen.
 */
double multiplyRowColumn(Matrix *a, int row, Matrix *b, int column);

#endif
