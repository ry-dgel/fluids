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

    // gaussian width
    double width;
    
    // rho0
    double rho0;

    // epsilon0
    double eps0;

    // gamma
    double gamma;
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
