#ifndef FLUX_H
#define FLUX_H

#include "cell.h"
typedef double (*FluxFunc)(Cell *, int);

// Flux Function Array
typedef struct DivFlux{
    double *divFl;
    FluxFunc *fluxFunc;
} DivFlux;

// Initialize DivFlux
DivFlux *initDivFlux(void);

// Flux Functions
double massFlux(Cell *current, int direc);
double momentumFlux(Cell *current, int i, int j);
double momentumFluxX(Cell *current, int j);
double momentumFluxY(Cell *current, int j);
double energyFlux(Cell *current, int direc);

// Get conserved quantity
double getConsQtty(Cell *current, int fluxInd);

void fluxTest(Cell **space);
int fluxTestCell(Cell *current, DivFlux *ktFlux);
#endif
