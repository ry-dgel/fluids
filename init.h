#ifndef INIT_H
#define INIT_H

#include "cell.h"

#define MIN(a,b) ((a < b) ? (a) : (b))
#define MAX(a,b) ((a > b) ? (a) : (b))
// Gaussian functions defined in assignment.
double gaussRho(double r);
double gaussEps(double r, double j2, double rho);

void initialize(Cell **space);
void reCalcDeltaT(Cell **space);

#endif
