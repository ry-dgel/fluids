#include <stdio.h>
#include <tgmath.h>
#include "cell.h"
#include "data.h"

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

    return 1;
}
