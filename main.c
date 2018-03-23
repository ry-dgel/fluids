#include <stdio.h>
#include <tgmath.h>
#include "cell.h"
#include "data.h"
#include "init.h"
#include "flux.h"
#include "evolve.h"

int main(int argc, char **argv){
    // This is Space
    Cell **space;

    // Watch out for id10t errors
    checkInputFile(argv);
    
    // Read in data from file, and retrieve file number.
    int fNum = readInData(argv[1]);

    // Output to InitData#.dat
    printInitData(fNum);
    
    // Let there be space
    space = initSpace();

    // Is there space?
    testSpace(space);

    // Put stuff in space
    initialize(space);
    printSpace(space, "Zifkin_CA_6_initial", fNum);
    
    // Ensure CFL condition
    reCalcDeltaT(space);
    
    fluxTest(space);

    evolve(space);

    // printSpace(space, "final", fNum);
    
    return 1;
}
