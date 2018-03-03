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
    double *j;

    // Previous Values
    double rhoPrev;
    double epsPrev;
    double presPrev;
    double *jPrev;

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

// return 1 for positive x, -1 for negative x, 0 for x = 0
int intSgn(int x);

// print out values of rho and eps to a file with position.
void printSpace(Cell **space, char *fName, int fNum);

// Copy data to ghost cell from their nearest neighbour
void updateGhosts(Cell **space);

// Copy data from one cell to an other, do not touch neighbour positions.
void cellCopy(Cell *src, Cell *targ);
#endif
