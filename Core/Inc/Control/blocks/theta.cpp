

#include "theta.hpp"

Theta thetas;

ThetaStructData theta_calculator(double Alpha, double Beta)
{
    thetas.funcion(Alpha, Beta);
    return thetas.theta_prueba;
}