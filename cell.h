#ifndef CELL_H
#define CELL_H

// Macros for indexing v, pos, nbrs arrays
#define XIND 0
#define YIND 1
// Macros for indexing limits of DATA
#define XLIM 1
#define YLIM 2

// cell data struct
typedef struct cell{
    // 5 Hydrodynamic Variables
    double rho;
    double eps;
    double pres;
    double *v;

    // Previous Values
    double rhoPrev;
    double epsPrev;
    double presPrev;
    double *vPrev;

    // Node position
    int *pos;

    // Neighbour Pointers
    struct cell **nbrPls;
    struct cell **nbrMin;
} Cell;

// matrix allocation of cells
Cell **cell2dAlloc(int nx, int ny);

// free 2d allocation
void cell2dFree(Cell **cells, int nx);

// init space
Cell **initSpace(void);

// link cells
void linkSpace(Cell **Space);

// test links
void testSpace(Cell **Space);

// get neighbours
//  1 is plus x direction,   2 is plus y
// -1 is minus x direction, -2 is minus y
// 0 return self
Cell *cellGetNeighbour(Cell *current, int dir);

// get the position of a cell
void cellGetPos(Cell *current, double *pos);

int intSgn(int x);
#endif
