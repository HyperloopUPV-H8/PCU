#pragma once

#include "dutys.hpp"

Dutys dutys;

DutysStructData duty_calculator(double T1, double T2, double T0, unsigned int sector)
{
    dutys.funcion(T1, T2, T0, sector);
    return dutys.dutys_prueba;
}
