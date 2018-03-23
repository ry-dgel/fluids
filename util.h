#ifndef UTIL_H
#define UTIL_H

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define KRON(a,b) ((a) == (b) ? 1 : 0)

// Macros for indexing v, pos, nbrs arrays
#define XIND 0
#define YIND 1
// Macros for indexing limits of DATA
#define XLIM 1
#define YLIM 2

#endif
