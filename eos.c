#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "eos.h"
#include "data.h"

//Polytropic EQ of state.
double eosPressure(double rho, double eps, double *j){
    //abs(j)^2
    double j2 = pow(j[1],2) + pow(j[2],2);
    double uRho = eps - j2 / (2 * rho);
    double pressure = uRho * (DATA.gamma - 1);

    if(pressure < 0){
        printf("Error: Negative pressure encountered");
        exit(0);
    }

    return pressure;
}

double eosSoundSpd(double rho, double eps, double *j){
    return sqrt(DATA.gamma * eosPressure(rho, eps, j) / rho);
}
