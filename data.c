#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "data.h"
#include "vctr_mtx.h"

// Read in data from inputFile, store in DATA struct.
int readInData(char *inputFile){

    // Initialize Member Arrays
    DATA.Nmax   = intVectorMalloc(3);
    DATA.Lmax   = vectorMalloc(3);
    DATA.dx     = vectorMalloc(3);
    DATA.lBound = vectorMalloc(3);
    DATA.uBound = vectorMalloc(3);

    // Open input file
    FILE *input = fopen(inputFile, "r");

    // Read in upper and lower bound for t
    fscanf(input, "%lf", &DATA.lBound[0]);
    fscanf(input, "%lf", &DATA.uBound[0]);

    // Read in upper and lower bound for x
    fscanf(input, "%lf", &DATA.lBound[1]);
    fscanf(input, "%lf", &DATA.uBound[1]);

    // Read in upper and lower bound for y
    fscanf(input, "%lf", &DATA.lBound[2]);
    fscanf(input, "%lf", &DATA.uBound[2]);

    // Read in Nmax, calculate Lmax and dx, for t,x,y all in one loop!
    for(int i = 0; i < 3; i++){
        fscanf(input, "%d", &DATA.Nmax[i]);
        DATA.Lmax[i] = DATA.uBound[i] - DATA.lBound[i];
        DATA.dx[i] = DATA.Lmax[i]/DATA.Nmax[i];
    }

    // Read in parameters.
    fscanf(input, "%lf", &DATA.width);
    fscanf(input, "%lf", &DATA.rho0);
    fscanf(input, "%lf", &DATA.eps0);
    fscanf(input, "%lf", &DATA.gamma);

    // Get file number, file name must be input#.dat
    int fNum = inputFile[5] - '0';

    // Close the file
    fclose(input);

    return fNum;
}

// Pretty printing the contents of DATA to InitData#.dat.
void printInitData(int fNum){

    char fname[50];
    sprintf(fname, "Zifkin_CA_5_Results/params%d.dat", fNum);
    FILE *output = fopen(fname, "w");

    fprintf(output, "T bounds:    %f, %f\n", DATA.lBound[0], DATA.uBound[0]);
    fprintf(output, "X bounds:    %f, %f\n", DATA.lBound[1], DATA.uBound[1]);
    fprintf(output, "Y bounds:    %f, %f\n", DATA.lBound[2], DATA.uBound[2]);
    fprintf(output, "T Size:      %f\n", DATA.Lmax[0]);
    fprintf(output, "X Size:      %f\n", DATA.Lmax[1]);
    fprintf(output, "Y Size:      %f\n", DATA.Lmax[2]);
    fprintf(output, "Time Step:   %f\n", DATA.dx[0]);
    fprintf(output, "X Step:      %f\n", DATA.dx[1]);
    fprintf(output, "Y Step:      %f\n", DATA.dx[2]);
    fprintf(output, "T Steps:     %d\n", DATA.Nmax[0]);
    fprintf(output, "X Steps:     %d\n", DATA.Nmax[1]);
    fprintf(output, "Y Steps:     %d\n", DATA.Nmax[2]);
    fprintf(output, "Gaus. Width: %f\n", DATA.width);
    fprintf(output, "Rho Initial: %f\n", DATA.eps0);
    fprintf(output, "Epsilon 0:   %f\n", DATA.rho0);
    fprintf(output, "Gamma:       %f\n", DATA.gamma);

    fclose(output);
}

void checkInputFile(char** argv){
    // Check filename was passed to function.
    if(argv[1] == NULL){
        printf("No input file given, try again!\n");
        exit(0);
    // Check File Exists.
    } else if(fopen(argv[1], "r") == NULL){
            printf("File: %s, does not exist, try again!\n", argv[1]);
            exit(0);
    }
    printf("Running with file: %s\n", argv[1]);
    return;
}
