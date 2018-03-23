#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "data.h"
#include "util.h"
#include "eos.h"
#include "cell.h"
#include "flux.h"
#include "kurganov_tadmor.h"

void evolve(Cell **space){
   int it, itmax, rkStep;
   Cell **bdryCells;
   DivFlux *ktFlux;

   itmax = DATA.Nmax[0];

   ktFlux = initDivFlux();
   bdryCells = initBoundaryCells();

   testBoundaryCells(space, bdryCells);

   return;
}
