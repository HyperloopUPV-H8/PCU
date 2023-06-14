#pragma once

#include "../utils.hpp"
#include "math.h"

struct TimesStructData
{
public:
    double T1;
    double T2;
    double T0;
};
struct Times
{
public:
    TimesStructData times_prueba;

    void funcion(double VDC, double Vref, double theta, unsigned int sector)
    {
        times_prueba.T1 = (sqrt(3.0) * Ts * fabs(Vref)) / VDC * sin((sector / 3.0) * PI_SVPWM - theta);
        times_prueba.T2 = (sqrt(3.0) * Ts * fabs(Vref)) / VDC * sin(theta - ((sector - 1) / 3.0) * PI_SVPWM);
        times_prueba.T0 = Ts - (times_prueba.T1 + times_prueba.T2);
    }
};

TimesStructData calculate_times(double VDC, double Vref, double theta, unsigned int sector);
