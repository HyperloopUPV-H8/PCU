#include "times.hpp"

Times times;

TimesStructData calculate_times(double VDC, double Vref, double theta, unsigned int sector)
{
    times.funcion(VDC, Vref, theta, sector);
    return times.times_prueba;
}
