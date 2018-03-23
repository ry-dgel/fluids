#ifndef KURG_TAD_H
#define KURG_TAD_H

#include "cell.h"
#include "flux.h"

#define LL 0][0
#define LR 0][1
#define RL 1][0
#define RR 1][1
#define DD 2][0
#define DU 2][1
#define UD 3][0
#define UU 3][1

void testBoundaryCells(Cell **space, Cell **bdryCells);

Cell **initBoundaryCells();

void makeBoundaryCells(Cell *current, Cell **bdryCells, int rkStep);

void boundaryCell(Cell *current, Cell *bdryCell, int dir, int lr, int rkStep);

Cell *getCurrentCell(Cell *current, int dir, int lr);

void makeKTFlux(Cell **bdryCells, DivFlux *ktFlux);

Cell *getBoundaryCell(Cell **bdryCells, int dir, int lr);

void firstPass(Cell *current, DivFlux *ktFlux);

void secondPass(Cell *current, DivFlux *ktFlux);

double minmodA(double up1, double u, double um1);
#endif
