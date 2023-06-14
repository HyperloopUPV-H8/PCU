#include "voltages.hpp"

Voltages voltages;

VoltageStructData voltage_calculator(double frequency, double target_phase_voltage, double time, double direction)
{
    voltages.funcion(frequency, target_phase_voltage, time, direction);
    return voltages.voltaje_prueba;
}
