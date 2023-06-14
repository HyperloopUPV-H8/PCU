#pragma once

#include "math.h"
#include "../utils.hpp"
#include "stdio.h"
#include "Control/Blocks/Integrator.hpp"

struct VoltageStructData
{
public:
    double phase_u, phase_v, phase_w;
};

struct Voltages
{
public:
	Integrator<IntegratorType::Trapezoidal> angle_integrator = {1.0/5000, 1};

    VoltageStructData voltaje_prueba;
    Voltages() {}

    void funcion(double frequency, double target_phase_voltage, double time, double direction)
    {

        //Hay que hacer integral del angulo, solo importa como incrementa, asi que no es necesario analizarlo
        angle_integrator.input_value = 2*PI_SVPWM * fabs(frequency);;
        angle_integrator.execute();
        double angulo = angle_integrator.output_value;

        voltaje_prueba.phase_u = target_phase_voltage * sin(angulo);
        voltaje_prueba.phase_v = target_phase_voltage * sin(angulo - PHASE_SVPWM);
        voltaje_prueba.phase_w = target_phase_voltage * sin(angulo + PHASE_SVPWM);
        if (direction == -1)
        {
            double swap = voltaje_prueba.phase_w;
            voltaje_prueba.phase_w = voltaje_prueba.phase_v;
            voltaje_prueba.phase_v = swap;
        }
    }
};

VoltageStructData voltage_calculator(double frequency, double target_phase_voltage, double time, double direction);
