#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "data.h"
#include "cell.h"
#include "init.h"
#include "eos.h"
#include "vctr_mtx.h"

double gaussRho(double r){
    return DATA.rho0 / 4 * exp(-pow(r, 2) / 2 / pow(DATA.width, 2)) + DATA.rho0;
}

double gaussEps(double r, double j2, double rho){
    double offset = DATA.eps0 + j2 / (2 * rho);
    return DATA.eps0 / 4 * exp(-pow(r, 2) / 2 / pow(DATA.width,2)) + offset;
}

void initialize(Cell **space){
    Cell *current;
    double *cntr = vectorMalloc(2); 
    double *pos = vectorMalloc(2);
    double j2, r;
   
    int nx = DATA.Nmax[XLIM];
    int ny = DATA.Nmax[YLIM];
    
    // Get position of center cell.
    cntr[XIND] = (DATA.uBound[XLIM] - DATA.lBound[XLIM])/2;
    cntr[YIND] = (DATA.uBound[YLIM] - DATA.lBound[YLIM])/2;

    for(int ix = 1; ix <= nx; ix++){
        for(int iy = 1; iy <= ny; iy++){
            current = &space[ix][iy];

            cellGetPos(current, pos);
            r = hypot(pos[XIND] - cntr[XIND], pos[YIND] - cntr[YIND]);
            current -> rho = gaussRho(r);

            current -> j[XIND] = -0.1 * (pos[XIND] - cntr[XIND]);
            current -> j[YIND] =  0.1 * (pos[YIND] - cntr[YIND]);

            j2 = pow(current -> j[XIND], 2) + pow(current -> j[YIND], 2);
            current -> eps = gaussEps(r, j2, current->rho);

            current -> pres = eosPressure(current -> rho, 
                                          current -> eps,
                                          current -> j);
        }
    }
    updateGhosts(space);
}

void reCalcDeltaT(Cell **space){
    Cell *current;
    double csMax = 0.0;
    double cs;

    // Get cs max
    for(int ix = 1; ix <= DATA.Nmax[XLIM]; ix++){
        for(int iy = 1; iy <= DATA.Nmax[YLIM]; iy++){
           current = &(space[ix][iy]);
           cs = eosSoundSpd(current->rho, current->eps, current->j);
           if(cs > csMax){
               csMax = cs;
           }
        }
    }
 
    fprintf(stderr, "csMax = %e\n", csMax);
    printf("Old dt: %f\n", DATA.dx[0]);
    if(DATA.dx[0] > MIN(DATA.dx[1], DATA.dx[2]) / (8 * csMax)){
       DATA.dx[0] = MIN(DATA.dx[1], DATA.dx[2]) / (8 * csMax); 
    }
    printf("New dt: %f\n", DATA.dx[0]);

    printf("Old Nmax: %d\n", DATA.Nmax[0]);
    DATA.Nmax[0] = (int) DATA.Lmax[0] / DATA.dx[0];
    printf("New Nmax: %d\n", DATA.Nmax[0]);
}
