#include <stdlib.h>

int *intVectorMalloc(int n){
    return (int *) malloc(n*sizeof(int));
}

double *vectorMalloc(int n){
    return (double *) malloc(n*sizeof(double));
}

double **matrixMalloc(int m, int n){
    double **matrix = (double **) malloc(m * sizeof(double *));
    for (int i=0; i<m; i++){
        matrix[i] = (double *) malloc(n * sizeof(double *));
    }
    return matrix;
}

void mtxFree(double **matrix, int n){
    for(int i = 0; i < n; i++){
        free(matrix[n]);
    }
    free(matrix);
}

void copyVector(double *a, double *b, int n){
    for(int i = 0; i < n; i++){
        b[i] = a[i];
    }
}
