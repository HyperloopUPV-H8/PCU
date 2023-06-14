#pragma once

#include "../utils.hpp"

struct ThetaStructData
{
public:
    double vref;
    double theta;
    unsigned int sector;
};

struct Theta
{
public:
    ThetaStructData theta_prueba;
    void funcion(double Alpha, double Beta)
    {
        theta_prueba.vref = sqrt(Alpha * Alpha + Beta * Beta);
        theta_prueba.theta = atan2(Beta, Alpha);
        if (theta_prueba.theta > 0 && theta_prueba.theta < 60 / 180.0 * PI_SVPWM)
            theta_prueba.sector = 1;
        else if (theta_prueba.theta > 60 / 180.0 * PI_SVPWM && theta_prueba.theta < 120 / 180.0 * PI_SVPWM)
            theta_prueba.sector = 2;
        else if (theta_prueba.theta > 120 / 180.0 * PI_SVPWM && theta_prueba.theta < 180 / 180.0 * PI_SVPWM)
            theta_prueba.sector = 3;
        else if (theta_prueba.theta > -180 / 180.0 * PI_SVPWM && theta_prueba.theta < -120 / 180.0 * PI_SVPWM)
            theta_prueba.sector = 4;
        else if (theta_prueba.theta > -120 / 180.0 * PI_SVPWM && theta_prueba.theta < -60 / 180.0 * PI_SVPWM)
            theta_prueba.sector = 5;
        else if (theta_prueba.theta > -60 / 180.0 * PI_SVPWM && theta_prueba.theta < 0 / 180.0 * PI_SVPWM)
            theta_prueba.sector = 6;
        else
            theta_prueba.sector = 0;
    }
};

ThetaStructData theta_calculator(double Alpha, double Beta);