#ifndef EVOLVE_H
#define EVOLVE_H

void evolve(Cell **space);

void advanceRK(Cell **space, Cell **bdryCells, DivFlux *ktFlux, int rkStep);

#endif
