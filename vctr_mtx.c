#include <stdio.h> //remove when done
#include <stdlib.h>
#include <tgmath.h>

int *intVectorMalloc(int n){
    return malloc(n * sizeof(int));
}

double *vectorMalloc(int n){
    return malloc(n * sizeof(double));
}

double **matrixMalloc(int m, int n){
    double **matrix = malloc(m * sizeof(double *));
    for (int i=0; i<m; i++){
        matrix[i] = malloc(n * sizeof(double *));
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

double dotProd(double *a, double *b, int n){
    double dp = 0.0;
    for(int i = 0; i < n; i++){
        dp += a[i] * b[i];
    }
    return dp;
}

double norm(double *a, int n){
    return sqrt(dotProd(a, a, n));
}
