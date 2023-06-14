#pragma once

#include "math.h"
#include "../utils.hpp"
#include "stdio.h"

struct Alpha_betaData
{
public:
    double alpha;
    double beta;
};

struct Alpha_beta
{
public:
    Alpha_betaData alpha_beta_prueba;
    Alpha_beta() {}
    void funcion(double voltage_u, double voltage_v, double voltage_w)
    {
        alpha_beta_prueba.alpha = (2 / 3.0) * voltage_u - (1 / 3.0) * voltage_v - (1 / 3.0) * voltage_w;
        alpha_beta_prueba.beta = (sqrt((double)3) / 3.0) * (voltage_v - voltage_w);
    }
};

Alpha_betaData alpha_beta_calculator(double voltage_u, double voltage_v, double voltage_w);