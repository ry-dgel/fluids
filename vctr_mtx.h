#ifndef VCTR_MTX_H
#define VCTR_MTX_H

// allocate a vector of ints
int *intVectorMalloc(int n);

// allocate a vector of doubles
double *vectorMalloc(int n);

// allocate a matrix of size m*n of doubles
double **matrixMalloc(int m, int n);

// free a matrix
void mtxFree(double **matrix, int n);

// copy vector a to vector b
void copyVector(double *a, double *b, int n);

// dot product
double dotProd(double *a, double *b, int n);

// Norm
double norm(double *a, int n);
#endif
