#pragma once

#include "../utils.hpp"

struct DutysStructData
{
public:
    double dutyU;
    double dutyV;
    double dutyW;
};

struct Dutys
{
public:
    DutysStructData dutys_prueba;
    void funcion(double T1, double T2, double T0, unsigned int sector)
    {
        switch (sector)
        {
        case 1:
            dutys_prueba.dutyU = T1 + T2 + T0 / 2;
            dutys_prueba.dutyV = T2 + T0 / 2;
            dutys_prueba.dutyW = T0 / 2;
            break;
        case 2:
            dutys_prueba.dutyU = T1 + T0 / 2;
            dutys_prueba.dutyV = T1 + T2 + T0 / 2;
            dutys_prueba.dutyW = T0 / 2;
            break;
        case 3:
            dutys_prueba.dutyU = T0 / 2;
            dutys_prueba.dutyV = T1 + T2 + T0 / 2;
            dutys_prueba.dutyW = T2 + T0 / 2;
            break;
        case 4:
            dutys_prueba.dutyU = T0 / 2;
            dutys_prueba.dutyV = T1 + T0 / 2;
            dutys_prueba.dutyW = T1 + T2 + T0 / 2;
            break;
        case 5:
            dutys_prueba.dutyU = T2 + T0 / 2;
            dutys_prueba.dutyV = T0 / 2;
            dutys_prueba.dutyW = T1 + T2 + T0 / 2;
            break;
        case 6:
            dutys_prueba.dutyU = T1 + T2 + T0 / 2;
            dutys_prueba.dutyV = T0 / 2;
            dutys_prueba.dutyW = T1 + T0 / 2;
            break;

        default:
            dutys_prueba.dutyU = 0;
            dutys_prueba.dutyV = 0;
            dutys_prueba.dutyW = 0;
            break;
        }
        dutys_prueba.dutyU = dutys_prueba.dutyU / Ts;
        dutys_prueba.dutyV = dutys_prueba.dutyV / Ts;
        dutys_prueba.dutyW = dutys_prueba.dutyW / Ts;
    }
};

DutysStructData duty_calculator(double T1, double T2, double T0, unsigned int sector);