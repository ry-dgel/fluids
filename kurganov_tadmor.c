#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kurganov_tadmor.h"
#include "eos.h"
#include "util.h"
#include "vctr_mtx.h"
#include "data.h"
#include "cell.h"

// Initialize Boundary Cells.
Cell **initBoundaryCells(){
    Cell **cells, *current;
    int dir, lr;

    // "This is a small price to pay." It's gonna be like
    // 200*300*7*20*8 Bytes, that's a pretty big addition to a memory footprint
    // I'll just use macros to define easier to read indexing.
    // for dir: [0, 1, 2, 3] = [Left, Down, Right, Up]
    // for lr: 0 is from inside left, 1 is from right.
    cells = cell2dAlloc(4, 2);
    for(dir = 0; dir < 4; dir++){
        for(lr = 0; lr < 2; lr++){
            current = &(cells[dir][lr]);
            current -> j = vectorMalloc(2);
            current -> jPrev = vectorMalloc(2);
        }
    }
    return cells;
}

void testBoundaryCells(Cell **space, Cell **bdryCells){
    FILE *f = fopen("Zifkin_CA_6_Results/Zifkin_CA_6_bdry.dat", "w");

    Cell *current;
    double *pos = vectorMalloc(2);
    double avg = 0;

    int rkStep = 1;

    int nx = DATA.Nmax[XLIM];
    int ny = DATA.Nmax[YLIM];

    for(int ix = 1; ix <= nx; ix++){
        for(int iy = 1; iy <= ny; iy++){
            current = &(space[ix][iy]);
            makeBoundaryCells(current, bdryCells, rkStep);
            cellGetPos(current, pos);
            for (int dir = 0; dir < 4; dir++){
                for (int lr = 0; lr < 2; lr++){
                    avg += bdryCells[dir][lr].rhoPrev;
                }
            }
            avg /= 8;
            fprintf(f, "%f, %f, %f\n", pos[XIND], pos[YIND], avg);
        }
        fprintf(f, "\n");
    }
}

void makeBoundaryCells(Cell *current, Cell **bdryCells, int rkStep){
    for(int dir = 0; dir < 3; dir++){
        for(int lr = 0; lr < 2; lr++){
            boundaryCell(current, &(bdryCells[dir][lr]), dir, lr, rkStep);
        }
    }
}

void boundaryCell(Cell *current, Cell *bdryCell, int dir, int lr, int rkStep){
    double f, fp1, fm1, f0;
    Cell *plsNbr, *minNbr, *curCell;

    // Due to my choice of indexing, even indices are along x axis,
    // odd indices are along y axis.
    int direc = dir % 2;
    
    curCell = getCurrentCell(current, dir, lr);
    plsNbr = curCell -> nbrPls[direc];
    minNbr = curCell -> nbrMin[direc];

    if(rkStep == 1){
        //mass density
        f0  = curCell -> rho;
        fp1 = plsNbr  -> rho;
        fm1 = minNbr  -> rho;
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> rhoPrev = f;

        f0  = curCell -> j[XIND];
        fp1 = plsNbr  -> j[XIND];
        fm1 = minNbr  -> j[XIND];
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> jPrev[XIND] = f;

        f0  = curCell -> j[YIND];
        fp1 = plsNbr  -> j[YIND];
        fm1 = minNbr  -> j[YIND];
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> jPrev[YIND] = f;

        f0  = curCell -> eps;
        fp1 = plsNbr  -> eps;
        fm1 = minNbr  -> eps;
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> epsPrev = f;

        bdryCell -> presPrev = eosPressure(bdryCell -> rhoPrev, 
                                           bdryCell -> epsPrev, 
                                           bdryCell -> jPrev);
    }
    else if(rkStep == 2){
        //mass density
        f0  = curCell -> rhoPrev;
        fp1 = plsNbr  -> rhoPrev;
        fm1 = minNbr  -> rhoPrev;
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> rho = f;

        f0  = curCell -> jPrev[XIND];
        fp1 = plsNbr  -> jPrev[XIND];
        fm1 = minNbr  -> jPrev[XIND];
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> j[XIND] = f;

        f0  = curCell -> jPrev[YIND];
        fp1 = plsNbr  -> jPrev[YIND];
        fm1 = minNbr  -> jPrev[YIND];
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> j[YIND] = f;

        f0  = curCell -> epsPrev;
        fp1 = plsNbr  -> epsPrev;
        fm1 = minNbr  -> epsPrev;
        f = f0 - (2*lr-1) * minmodA(fp1, f0, fm1)/2;
        bdryCell -> eps = f;

        bdryCell -> pres = eosPressure(bdryCell -> rho, bdryCell -> eps, 
                                       bdryCell -> j);
    }
    else{
        printf("Invalid RK Step given.\n");
        exit(0);
    }
}

Cell *getCurrentCell(Cell *current, int dir, int lr){
    // Either left or down.
    if(dir < 2){ 
        // From right, which is from within, so current.
        if(lr == 1){
            return current;
        } else {
            // Left or Down are minus directions, if dir is even. go along x.
            return current -> nbrMin[dir % 2];
        }
    }
    // Either right or up.
    if(dir > 1){
        // From left, which is from within so current.
        if(lr == 0){
            return current;
        } else {
            // Right or Up are plus directions, if dir is even, go along x.
            return current -> nbrPls[dir % 2];
        }
    }
    else{
        printf("Invalid Dir or LR value entered.");
        exit(0);
    }
}

double minmodA(double up, double u, double um){
    double tf = DATA.tf;

    double diffup = (up - u) * tf;                                 
    double diffdn = (u - um) * tf;                                 
    double diffmd = (up - um) * 0.5;                                            
                                                                                
    if(up > u && u > um){                                                       
        return MIN(MIN(diffup, diffdn), diffmd);                                
    } else if(um > u && u > up){                                                
        return MAX(MAX(diffup, diffdn), diffmd);                                
    } else {                                                                    
        return 0.0;                                                             
    } 
}
