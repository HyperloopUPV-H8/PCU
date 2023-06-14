#include "alpha_beta.hpp"

Alpha_beta alpha_beta;

Alpha_betaData alpha_beta_calculator(double voltage_u, double voltage_v, double voltage_w)
{
    alpha_beta.funcion(voltage_u, voltage_v, voltage_w);
    return alpha_beta.alpha_beta_prueba;
}
