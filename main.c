#include <stdio.h>
#include <tgmath.h>
#include "cell.h"
#include "data.h"
#include "init.h"

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

    // Put stuff in space
    initialize(space);
    printSpace(space, "Zifkin_CA_5_initial", fNum);
    
    // Ensure CFL condition
    reCalcDeltaT(space);

    //evolve(space)
    //printSpace(space, "final", fNum)
    
    // Is there space?
    testSpace(space);
    
    return 1;
}
