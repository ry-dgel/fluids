#ifndef DATA_H
#define DATA_H

typedef struct Data {
    // Number of real cells
    int *Nmax;

    // lower bounds
    double *lBound;

    // upper bounds
    double *uBound;

    // size of space
    double *Lmax;

    // step sizes
    double *dx;
} Data;

// Read in data, into DATA.
int readInData(char* inputFile);

// Print out data, from DATA.
void printInitData(int fNum);

// Check for input file
void checkInputFile(char** argv);

// Make DATA available everywhere that includes this header.
Data DATA;
#endif
