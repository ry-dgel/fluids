#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "vctr_mtx.h"
#include "util.h"
#include "flux.h"
#include "eos.h"
#include "data.h"

DivFlux *initDivFlux(void){
    DivFlux *fl;

    fl = malloc(sizeof(DivFlux));
    fl -> divFl = malloc(4 * sizeof(double));
    fl -> fluxFunc = malloc(4 * sizeof(FluxFunc));

    fl -> fluxFunc[0] = massFlux;
    fl -> fluxFunc[1] = momentumFluxX;
    fl -> fluxFunc[2] = momentumFluxY;
    fl -> fluxFunc[3] = energyFlux;

    return fl;
}

double massFlux(Cell *current, int dir){
    return current -> j[dir];
}

double momentumFlux(Cell *current, int i, int j){
    double *J       = current -> j;
    double rho      = current -> rho;
    double pressure = current -> pres;

    return J[i] * J[j] / rho + pressure * KRON(i,j);
}

double momentumFluxX(Cell *current, int j){
    return momentumFlux(current, XIND, j);
}

double momentumFluxY(Cell *current, int j){
    return momentumFlux(current, YIND, j);
}

double energyFlux(Cell *current, int dir){
    double *J       = current -> j;
    double rho      = current -> rho;
    double pressure = current -> pres;
    double epsilon  = current -> eps;

    return (J[dir] / rho) * (epsilon + pressure);
}

double getConsQtty(Cell *current, int fluxInd){
    switch(fluxInd){
        case 0:
            return current -> rho;
        case 1:
            return current -> j[XIND];
        case 2:
            return current -> j[YIND];
        case 3:
            return current -> eps;
        default:
            printf("No conserved quantity");
            exit(0);
    }
}

void fluxTest(Cell **space){
   DivFlux *ktFlux = initDivFlux();
   int ix, iy, ixmax, iymax;
   Cell *current;

   ixmax = DATA.Nmax[XLIM];
   iymax = DATA.Nmax[YLIM];

   fprintf(stderr, "Flux Test: Starting...\n");
   for(ix = 1; ix <= ixmax; ix++){
       for(iy = 1; iy <= iymax; iy++){
            current = &(space[ix][iy]);
            if (!(fluxTestCell(current, ktFlux))){
                printf("Error occured at %d, %d\n", ix, iy);
                exit(0);
            }
       }
    }
   fprintf(stderr, "Flux Test: Ends.\n");
}

int fluxTestCell(Cell *current, DivFlux *ktFlux){
    double f, g, sum, pressure;
    int dir;
    FluxFunc func;

    pressure = eosPressure(current -> rho, current -> eps, current -> j);

    // mass flux test
    sum = .0;
    func = ktFlux -> fluxFunc[0];
    for(dir = XIND; dir <= YIND; dir++){
        f = func(current, dir);
        sum += f*f;
    }
    sum = sqrt(sum);
    g = norm(current -> j, 2);
    if (fabs(sum - g) > 1.0e-10){
        printf("Error in mass flux.\n");
        printf("Value of g is: %f\t value of sum is: %f\n", g, sum);
        return 0;
    }

    // momentum flux test
    sum = .0;
    for(dir = XIND; dir <= YIND; dir++){
        func = ktFlux -> fluxFunc[dir + 1];
        f = func(current, dir);
        sum += f;
    }
    g = dotProd(current -> j, current -> j, 2) / current -> rho + 2*pressure;
    if (fabs(sum - g) > 1.0e-10){
        printf("Error in momentum flux.\n"); 
        printf("Value of g is: %f\t value of sum is: %f\n", g, sum);
        return 0;
    }

    // energy flux test
    sum = .0;
    func = ktFlux -> fluxFunc[3];
    for(dir = XIND; dir <= YIND; dir++){
        f = func(current, dir);
        sum += f*f;
    }
    g = pow((current -> eps + pressure), 2) * 
        dotProd(current -> j, current -> j, 2) / 
        pow(current -> rho, 2);
    if (fabs(sum - g) > 1.0e-10){
        printf("Error in energy flux.\n"); 
        printf("Value of g is: %f\t value of sum is: %f\n", g, sum);
        return 0;
    }

    // Success!
    return 1;
}
