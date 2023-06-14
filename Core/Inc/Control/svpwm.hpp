#pragma once

#include "blocks/voltages.hpp"
#include "blocks/alpha_beta.hpp"
#include "blocks/theta.hpp"
#include "blocks/times.hpp"
#include "blocks/dutys.hpp"


struct Svpwm
{ 
    double duty_U;
    double duty_V;
    double duty_W;
};

Svpwm calculate_svpwm(double VDC, double voltage, double frequency, double time)
{
    VoltageStructData voltage_obj = voltage_calculator(frequency,voltage,time,1);
    Alpha_betaData alpha_beta_obj = alpha_beta_calculator(voltage_obj.phase_u,voltage_obj.phase_v,voltage_obj.phase_w);
    ThetaStructData theta_obj = theta_calculator(alpha_beta_obj.alpha,alpha_beta_obj.beta);
    TimesStructData times_obj = calculate_times(VDC, theta_obj.vref, theta_obj.theta, theta_obj.sector);
    DutysStructData dutys_obj = duty_calculator(times_obj.T1,times_obj.T2,times_obj.T0,theta_obj.sector);
    Svpwm result;
    result.duty_U=dutys_obj.dutyU;
    result.duty_V=dutys_obj.dutyV;
    result.duty_W=dutys_obj.dutyW;
    return result;
}


Svpwm calculate (double voltage, double frequency, double time);
double calculo(double entrada);


