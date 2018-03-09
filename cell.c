#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "cell.h"
#include "data.h"
#include "vctr_mtx.h"
Cell **cell2dAlloc(int nx, int ny){ // Make top level pointer, x-axis vector.
    Cell **cells = malloc(nx * sizeof(Cell *));
    for (int i=0; i<nx; i++){
        // Make y-axis vector pointers.
        cells[i] = malloc(ny * sizeof(Cell));
    }
    return cells;
}

void cell2dFree(Cell **cells, int nx){
    // Free all second tier, y-axis pointers.
    for(int i = 0; i < nx; i++){
        free(cells[i]);
    }
    // Free top level, x-axis pointer.
    free(cells);
}

Cell **initSpace(void){
    Cell **space;
    Cell *current;

    // Make space bigger for ghosts
    int nx = DATA.Nmax[XLIM] + 2;
    int ny = DATA.Nmax[YLIM] + 2;

    space = cell2dAlloc(nx, ny);

    for(int ix = 0; ix < nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            current = &(space)[ix][iy];

            // I refuse to allocate unecessary memory for easy notation
            current -> j = vectorMalloc(2);
            current -> jPrev = vectorMalloc(2);

            current -> pos = intVectorMalloc(2);
            current -> pos[XIND] = ix;
            current -> pos[YIND] = iy;

            current -> nbrPls = (Cell **) malloc(2 * sizeof(Cell *));
            current -> nbrMin = (Cell **) malloc(2 * sizeof(Cell *));
        }
    }
    linkSpace(space);
    return space;
}

void linkSpace(Cell **space){
    Cell *current;
    int nx = DATA.Nmax[XLIM]+2;
    int ny = DATA.Nmax[YLIM]+2;

    for(int ix = 0; ix < nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            current = &(space)[ix][iy];

            if(ix == (nx - 1)){
                current -> nbrPls[XIND] = current;
            } else {
                current -> nbrPls[XIND] = &(space)[ix+1][iy];
            }
            if(ix == 0){
                current -> nbrMin[XIND] = current;
            } else {
                current -> nbrMin[XIND] = &(space)[ix-1][iy];
            }
            if(iy == (ny - 1)){
                current -> nbrPls[YIND] = current;
            } else {
                current -> nbrPls[YIND] = &(space)[ix][iy+1];
            }
            if(iy == 0){
                current -> nbrMin[YIND] = current;
            } else {
                current -> nbrMin[YIND] = &(space)[ix][iy-1];
            }
        }
    }
}

Cell *cellGetNeighbour(Cell *current, int direction){
    switch(direction){
        case 0:
            return current;
        case 1:
            return current -> nbrPls[XIND];
        case -1:
            return current -> nbrMin[XIND];
        case 2:
            return current -> nbrPls[YIND];
        case -2:
            return current -> nbrMin[YIND];
        default:
            printf("AHH! That's not a direction!");
            exit(0);
    }
}

void cellGetPos(Cell *current, double *pos){
    pos[XIND] = DATA.lBound[XLIM] + DATA.dx[XLIM] * (current->pos[XIND] - 0.5);
    pos[YIND] = DATA.lBound[YLIM] + DATA.dx[YLIM] * (current->pos[YIND] - 0.5);
}

void testSpace(Cell **space){

    fprintf(stderr, "Start testing the space connections...\n");
    FILE *output = fopen("Zifkin_CA_5_Results/Zifkin_CA_5.dat", "w");

    Cell *current, *nbr;
    int sgnj;
    int absj;
    int nx = DATA.Nmax[XLIM]+2;
    int ny = DATA.Nmax[YLIM]+2;

    // For in order printing of neighbours to file.
    int js[5] = {2, -2, 1,-1, 0};
    int j;

    for(int ix = 0; ix < nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            current = &(space[ix][iy]);
            for(int i = 0; i < 5; i++){
                // Get direction to check in
                j = js[i];

                // Compute sgn and abs
                sgnj = intSgn(j);
                absj = abs(j);

                // Get neighbour in j direction
                nbr  = cellGetNeighbour(current, j);
                
                // Check if non current pointer is actually current.
                if(nbr != current || j == 0){
                    if(absj > 0 &&
                       nbr->pos[absj-1] != current->pos[absj-1] + sgnj){
                        printf("Mismatch found in neighgour positions\n");
                        exit(0);
                    }
                    // Wether or not to print.
                    if(!(ix % 10 + iy % 10)){
                        switch(j){
                            case -2:
                                fprintf(output, "Down Neighbour:  %d, %d\n",
                                        nbr->pos[XIND], nbr->pos[YIND]);
                                break;
                            case 2:
                                fprintf(output, "Up Neighbour:    %d, %d\n",
                                        nbr->pos[XIND], nbr->pos[YIND]);
                                break;
                            case -1:
                                fprintf(output, "Left Neighbour:  %d, %d\n",
                                        nbr->pos[XIND], nbr->pos[YIND]);
                                break;
                            case 1:
                                fprintf(output, "Right Neighbour: %d, %d\n",
                                        nbr->pos[XIND], nbr->pos[YIND]);
                                break;
                            case 0:
                                fprintf(output, "Current:         %d, %d\n\n",
                                        nbr->pos[XIND], nbr->pos[YIND]);
                                break;
                        }
                    }
                } else if(ix != 0 && iy != 0 && ix != nx - 1 && iy != ny - 1 ){
                    // Check if NULL pointer doesn't belong to a ghost,
                    // in which case halt and catch fire.
                    printf("Non ghost pointing to self at ix = %d, iy = %d\n",
                           ix, iy);
                    exit(0);
                }
            }
        }
    }
    fclose(output);
    fprintf(stderr, "Done testing the space connections.\n");
}

int intSgn(int x){
    // if x > 0, returns True  - False = 1
    // if x < 0, returns False - True = -1
    // if x = 0, returns False - False = 0
    return (x > 0) - (x < 0);
}

void printSpace(Cell **space, char *fName, int fNum){
    char buf[50];
    sprintf(buf, "Zifkin_CA_5_Results/%s%d.dat", fName, fNum);
    FILE *file = fopen(buf, "w");
    Cell *current;
    double *pos = vectorMalloc(2);
    
    // Only loop over "real" cells.
    for(int ix=1; ix <= DATA.Nmax[XLIM]; ix++){
        for(int iy=1; iy <= DATA.Nmax[YLIM]; iy++){
            current = &(space[ix][iy]);
            cellGetPos(current, pos);
            fprintf(file, "%f, %f, %f, %f\n", pos[XIND], pos[YIND],
                                              current -> rho, current -> eps);
        }
    }
    fclose(file);
    free(pos);
}

// Travel around the perimeter, updating the ghosts.
void updateGhosts(Cell **space){
    int ix = 0;
    int iy = 0;
    Cell *current;

    // Go along bottom row. (iy = 0)
    for(; ix <= DATA.Nmax[XLIM] + 1; ix++){
        current = &(space[ix][iy]);
        cellCopy(current->nbrPls[YIND], current);
    }
    // Apparantly it gets incremented even when the loop doesn't run??
    ix--;

    // Advance y so as not to repeat corner case.
    iy++;
    // Go along right column. (ix = xmax + 1)
    for(; iy <= DATA.Nmax[YLIM] + 1; iy++){
        current = &(space[ix][iy]);
        cellCopy(current->nbrMin[XIND], current);
    }
    iy--;

    ix--;
    // Go along top row. (iy = ymax + 1)
    for(; ix >= 0; ix--){
        current = &(space[ix][iy]);
        cellCopy(current->nbrMin[YIND], current);
    }
    ix++;

    iy--;
    // Go along left column. (ix = 0). 0,0 already done.
    for(; iy > 0; iy--){
        current = &(space[ix][iy]);
        cellCopy(current->nbrPls[XIND], current);
    }
}

void cellCopy(Cell *src, Cell *targ){
    targ -> rho     = src -> rho;
    targ -> eps     = src -> eps;
    targ -> pres    = src -> pres;
    targ -> j[XIND] = src -> j[XIND];
    targ -> j[YIND] = src -> j[YIND];

    targ -> rhoPrev     = src -> rhoPrev;
    targ -> epsPrev     = src -> epsPrev;
    targ -> presPrev    = src -> presPrev;
    targ -> jPrev[XIND] = src -> jPrev[XIND];
    targ -> jPrev[YIND] = src -> jPrev[YIND];
}
