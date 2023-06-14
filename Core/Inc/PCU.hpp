
#pragma once

#include "Control/Zeroing.hpp"
#include "Control/svpwm.hpp"
#include "lwip/udp.h"
#include "ST-LIB.hpp"
#include "lwip.h"

#include <algorithm>
#include <cmath>
#include <array>

float sign(float x){
	if(x < 0)
		return -1.0;
	return 1.0;
}

float unnan(float x){
	if(isnan(x))
		return 0.0;
	return x;
}

namespace Traction{

	enum PPUState{
		PPU_ON,
		PPU_OFF
	};

	uint16_t PPU_INDEX = 0;
	template <	Pin& state_led_pin		, Pin& buffer_enable_pin,
				Pin& board_t1_pin		, Pin& board_t2_pin	, Pin& board_t3_pin,
				Pin& motor_t1_pin		, Pin& motor_t2_pin	,
				Pin& current_1_pin		, Pin& current_2_pin, Pin& current_3_pin,
				Pin& pwm_u_pin			, Pin& pwm_v_pin	, Pin& pwm_w_pin,
				Pin& pwm_u_n_pin		, Pin& pwm_v_n_pin	, Pin& pwm_w_n_pin,
				Pin& battery_voltage_pin, Pin& battery_current_pin >
	class PPU {

	public:
		PPUState	state;

		float			board_temperatures[3];
		float			max_board_temperature;

		float			motor_temperatures[4];
		float			max_motor_temperature;

		float			phase_currents[6];
		Zeroing<100>	current_zeroing[3];

		float			battery_voltage, battery_current;
		float			phase_dutys[3] = {0,0,0};

		LinearSensor<float> board_temperature_sensors[3] = {
				LinearSensor<float>( board_t1_pin, 1.0, 0.0, &board_temperatures[0]),
				LinearSensor<float>( board_t2_pin, 1.0, 0.0, &board_temperatures[1]),
				LinearSensor<float>( board_t3_pin, 1.0, 0.0, &board_temperatures[2])
		};

		LinearSensor<float> motor_temperature_sensors[2] = {
				LinearSensor<float>(motor_t1_pin, 1.0, 0.0, &motor_temperatures[0] ),
				LinearSensor<float>(motor_t2_pin, 1.0, 0.0, &motor_temperatures[1] ),
		};

		LinearSensor<float> phase_current_sensors[3] = {
				LinearSensor<float>(current_1_pin, 361.5, -619, &phase_currents[0] ),
				LinearSensor<float>(current_2_pin, 361.5, -619, &phase_currents[1] ),
				LinearSensor<float>(current_3_pin, 361.5, -619, &phase_currents[2] )
		};

		LinearSensor<float> battery_voltage_sensor{battery_voltage_pin, 1.0, 0.0, &battery_voltage};
		LinearSensor<float> battery_current_sensor{battery_current_pin, 1.0, 0.0, &battery_current};

		DigitalOutput state_led{state_led_pin};
		DigitalOutput buffer_enable{buffer_enable_pin};

		DualPWM phase_pwms[3] = {
				{ pwm_u_pin, pwm_u_n_pin },
				{ pwm_v_pin, pwm_v_n_pin },
				{ pwm_w_pin, pwm_w_n_pin },
		};


		StackPacket<24, float, float, float> currents_a {
			603 + PPU_INDEX * 50,
			&phase_currents[0],
			&phase_currents[1],
			&phase_currents[2]
		};

		StackPacket<24, float, float, float> currents_b {
			610 + PPU_INDEX * 50,
			&phase_currents[3],
			&phase_currents[4],
			&phase_currents[5]
		};

		StackPacket<16,float, float> temperatures {
			604 + PPU_INDEX * 50,
			&max_board_temperature,
			&max_motor_temperature
		};

		StackPacket<16, float, float> battery {
			605 + PPU_INDEX * 50,
			&battery_voltage,
			&battery_current
		};

		StackPacket<24, float, float, float> dutys {
			608 + PPU_INDEX * 50,
			&phase_dutys[0], &phase_dutys[1], &phase_dutys[2]
		};


	private:
		void turn_on_adcs(){
			for(int i = 0; i < 3; i++)
				ADC::turn_on(board_temperature_sensors[i].get_id());
			for(int i = 0; i < 2; i++)
				ADC::turn_on(motor_temperature_sensors[i].get_id());
			for(int i = 0; i < 3; i++)
				ADC::turn_on(phase_current_sensors[i].get_id());

			ADC::turn_on(battery_voltage_sensor.get_id());
			ADC::turn_on(battery_current_sensor.get_id());
		}

		void setup_actuators(){
			for(int i = 0; i < 3; i++){
				phase_pwms[i].turn_on();
				phase_pwms[i].set_duty_cycle(0);
				phase_pwms[i].set_frequency(20000);
			}
		}

	public:

		void add_protections() {
			add_protection(&motor_temperatures[0], Boundary<float, ABOVE>(80));
			add_protection(&motor_temperatures[1], Boundary<float, ABOVE>(80));
			add_protection(&motor_temperatures[2], Boundary<float, ABOVE>(80));
			add_protection(&motor_temperatures[3], Boundary<float, ABOVE>(80));

			add_protection(&phase_currents[0], Boundary<float, ABOVE>(40));
			add_protection(&phase_currents[1], Boundary<float, ABOVE>(40));
			add_protection(&phase_currents[2], Boundary<float, ABOVE>(40));
			add_protection(&phase_currents[3], Boundary<float, ABOVE>(40));
			add_protection(&phase_currents[4], Boundary<float, ABOVE>(40));
			add_protection(&phase_currents[5], Boundary<float, ABOVE>(40));
		}

		void stop_actuadtors() {
			for(int i = 0; i < 3; i++)
				phase_dutys[i] = 0;
			apply_control();
		}

		void start() {
			turn_on_adcs();
			setup_actuators();
		}

		void apply_control(){
			for(int i = 0;i  < 3; i++)
				phase_pwms[i].set_duty_cycle(phase_dutys[i]);

		}

		void read(){
			for(int i = 0; i < 3; i++)
				board_temperature_sensors[i].read();
			for(int i = 0; i < 2; i++)
				motor_temperature_sensors[i].read();
			for(int i = 0; i < 3; i++){
				phase_current_sensors[i].read();
				current_zeroing[i].input_value = phase_currents[i];
				current_zeroing[i].execute();
				phase_currents[i] = current_zeroing[i].output_value;
			}

			battery_voltage_sensor.read();
			battery_current_sensor.read();

			max_motor_temperature = std::max(motor_temperatures[0], motor_temperatures[0]);
			max_board_temperature = std::max(	std::max(board_temperatures[0], board_temperatures[1]),
												board_temperatures[1]);
		}

		void send_all_data(DatagramSocket &ds){
			ds.send(currents_a);
			ds.send(temperatures);
			ds.send(battery);
			ds.send(dutys);
			ds.send(currents_b);
		}

		PPU() {
			PPU_INDEX += 1;
		}

	};


	class Saturator{
	private:
		double max, min;
	public:
		Saturator(double max, double min) : max(max), min(min){ }

		double saturate(double x){
			if(x < min)
				return min;
			if(x > max)
				return max;
			return x;
		}
	};

	class PCU{
	public:

		// PPUs
		PPU<PD10, PF4,				// State led, Buffer enable
			PF5, PF6, PF7,			// Board temperatures
			PC0, PC2,				// Motor temperatures
			PA0, PA4, PA5,			// Phase currents_a
			PE9, PE11, PE13,		// PWMs
			PE8, PE10, PE12,		// PWMs negated
			PF11, PF13 > left_ppu;	// Battery voltage, battery current

		// Control params
		double	SVPWM_EXEC_PERIOD	= 1.0 / 5000;
		double	svpwm_voltage 		= 50;
		double 	svpwm_frequency 	= 5;
		double	svpwm_time 			= 0;
		float	svpwm_vdc			= 75;
		bool 	is_svpwm_on 		= false;
		Saturator duty_saturator	= { 100, 0 };
		Svpwm svpwm;

		// Actuators
		DigitalOutput	operational_led	{PG8};
		DigitalOutput	fault_led		{PG7};
		DigitalOutput	can_led			{PG6};
		DigitalOutput	flash_led		{PG5};
		DigitalOutput	sleep_led		{PG4};
		DatagramSocket	socket;

		// IMU
		int reading_index = 0;
		float accel_x = 0, accel_y = 0, accel_z = 0, acceleration = 0, velocity = 0;
		Zeroing<100> accel_zeroing;

		// ORDERS AND DATA
		Integrator<IntegratorType::Trapezoidal> velocity_integrator{0.001 , 1};
		StackPacket<40, float, float, float, float, float> accelerations {
			606,
			&accel_x, &accel_y, &accel_z, &acceleration, &velocity
		};


		void add_protections(){
			left_ppu.add_protections();
		}

		PCU() { }

		void exec_svpwm(){
			if(is_svpwm_on){
				Svpwm result = calculate_svpwm(svpwm_vdc, svpwm_voltage, svpwm_frequency, svpwm_time);
				left_ppu.phase_dutys[0] = duty_saturator.saturate(result.duty_U * 100.0);
				left_ppu.phase_dutys[1] = duty_saturator.saturate(result.duty_V * 100.0);
				left_ppu.phase_dutys[2] = duty_saturator.saturate(result.duty_W * 100.0);
				left_ppu.apply_control();
				svpwm_time += SVPWM_EXEC_PERIOD;
			}
		}

		void test_svpmw(){
			is_svpwm_on = true;
		}

		void stop_control(){
			fault_led.turn_on();
			is_svpwm_on = false;
			left_ppu.stop_actuadtors();
		}

		void read_adcs() {
			left_ppu.read();
			reading_index += 1;
			if(reading_index > 1000){
				acceleration = accel_x;
				accel_zeroing.input(acceleration);
				accel_zeroing.execute();
				acceleration = accel_zeroing.output_value;
				acceleration	*= 9.81;

				velocity_integrator.input(acceleration);
				velocity_integrator.execute();
				velocity		= velocity_integrator.output_value;
			}
		}

		void turn_all_leds() {
			operational_led.turn_on();
		}

		void start() {
			turn_all_leds();
			left_ppu.start();
			socket = DatagramSocket(IPV4("192.168.1.6"), 50400, IPV4("192.168.0.9"), 50400);
			socket.reconnect();
		}

		void send_all_data(){
			left_ppu.send_all_data(socket);
			socket.send(accelerations);
		}

	};

}
