#pragma once

#include "IMU_Antialias_freq.hpp"
#include "IMU_registers.hpp"
#include "main.h"
#include <deque>
#include <cmath>

extern TIM_HandleTypeDef htim1;
extern SPI_HandleTypeDef hspi3;

using namespace std;

double  accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, temp;

double PI_ = 3.1415926535;
double accel_sensitivity = 16384;
double gyro_sensitivity = 16.4;

uint8_t txData[1] = { 0 };
uint8_t rxData[1] = { 0 };


/***************************************
 *
 * 			HELPER-COMMON
 *
 ***************************************/


uint8_t read_register(uint8_t register_address){
	txData[0] = register_address | 0b10000000;
	HAL_GPIO_WritePin(SPI3_SS_GPIO_Port, SPI3_SS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, txData, 1, 2000);
	rxData[0] = 0;
	HAL_SPI_Receive(&hspi3, rxData, 1, 2000);
	HAL_GPIO_WritePin(SPI3_SS_GPIO_Port, SPI3_SS_Pin, GPIO_PIN_SET);
	return rxData[0];
}

bool write_register(uint8_t register_address, uint8_t content){
	HAL_GPIO_WritePin(SPI3_SS_GPIO_Port, SPI3_SS_Pin, GPIO_PIN_RESET);

	txData[0] = register_address;
	HAL_SPI_Transmit(&hspi3, txData, 1, 2000);
	txData[0] = content;
	HAL_SPI_Transmit(&hspi3, txData, 1, 2000);

	HAL_GPIO_WritePin(SPI3_SS_GPIO_Port, SPI3_SS_Pin, GPIO_PIN_SET);

	return read_register(register_address) == content;
}

uint8_t read_pwm_mgmt0_fields(){
	return read_register(PWR_MGMT0);
}

void write_sensors_configuration(	TEMPERATURE_OPERACION_MODES temp_dis,
								 	 RC_OPERATION_MODE idle,
									ACCELERATION_OPERATION_MODES accel_mode	){
	SensorConfigRegister reg;
	reg.value = 0;
	reg.fields.TEMP_DIS		= temp_dis;
	reg.fields.ACCEL_MODE	= accel_mode;
	reg.fields.GYRO_MODE	= 0;
	reg.fields.IDLE 		= idle;
	write_register(PWR_MGMT0, reg.value);
}

SensorConfigRegister read_sensor_configuration() {
	SensorConfigRegister reg;
	reg.value = read_register(PWR_MGMT0);
	return reg;
}

/***************************************
 *
 * 			HELPER-ACCELERATION
 *
 ***************************************/

AccelerationConfigRegister read_acceleration_config(){
	AccelerationConfigRegister reg;
	reg.value = read_register(ACCEL_CONFIG0);
	return reg;
}

void write_acceleration_config(ACCELERATION_FULL_SCALE sf, ACCELERATION_ODR odr){
	AccelerationConfigRegister reg;
	reg.fields.ACCEL_FS_SEL = sf;
	reg.fields.ACCEL_ODR = odr;
	reg.fields.__RESERVED_2 = 0;
	accel_sensitivity = ACCLERATION_FS_SENSITIVITY[(uint8_t) sf];
	write_register(ACCEL_CONFIG0, reg.value);
}

/***************************************
 *
 * 				API
 *
 ***************************************/

uint8_t read_id(){
	return read_register(0x75);
}

void soft_reset(){
	DeviceConfigRegister reg;
	reg.value = 0;
	reg.fields.SOFT_RESET_CONFIG = 1;
	write_register(DEVICE_CONFIG, reg.value);
	HAL_Delay(5);
}




void turn_on_sensors(){
	write_sensors_configuration(TEMPERATURE_ON, RC_ALLWAYS_ON, ACCELERATION_LOW_NOISE);
	HAL_Delay(5);
}

void turn_off_sensors(){
	write_sensors_configuration(TEMPERATURE_OFF, RC_ALLWAYS_ON, ACCELERATION_OFF);
	HAL_Delay(5);
}


void config_accel_antialias(uint16_t freq){
	turn_off_sensors();
	AntialiasConfig config = ANTIALIAS_FREQ_TO_FILTER_CONFIG[freq];
	AccelConfigStatic2Register config_static2;
	AccelConfigStatic3Register config_static3;
	AccelConfigStatic4Register config_static4;


	config_static2.fields.ACCEL_AAF_DIS  = FILTER_ON;
	config_static2.fields.ACCEL_AAF_DELT = config.AAF_DELT;
	config_static2.fields._RESERVED = 0;

	config_static3.fields.ACCEL_AAF_DELTSQR = config.AAF_DELTSQR & 0b0000000011111111;

	config_static4.fields.ACCEL_AAF_BITSHIFT = config.AAF_BITSHIFT;
	config_static4.fields.ACCEL_AAF_DELTSQR  = (config.AAF_DELTSQR & 0b0000111100000000) >> 8;

	write_register(ACCEL_CONFIG_STATIC2, config_static2.value);
	write_register(ACCEL_CONFIG_STATIC3, config_static3.value);
	write_register(ACCEL_CONFIG_STATIC4, config_static4.value);
	turn_on_sensors();
}



double read_temp(){
	int16_t raw = (read_register(TEMP_DATA1) << 8) | read_register(TEMP_DATA0);
	return (raw / 132.48) + 25;
}


double read_accel_x(){
	int16_t raw = (int16_t) (read_register(ACCEL_DATA_X1) << 8) | read_register(ACCEL_DATA_X0);
	return  raw / accel_sensitivity;
}

double read_accel_y(){
	int16_t raw = (int16_t) (read_register(ACCEL_DATA_Y1) << 8) | read_register(ACCEL_DATA_Y0);
	return  raw / accel_sensitivity;
}

double read_accel_z(){
	int16_t raw = (int16_t) (read_register(ACCEL_DATA_Z1) << 8) | read_register(ACCEL_DATA_Z0);
	return  raw / accel_sensitivity;
}



void start_imu(){
	soft_reset();
	HAL_Delay(10);

	write_acceleration_config(ACCELERATION_8G, ACCELERATION_50HZ);
	config_accel_antialias(ANTIALIAS_FREQ_536_HZ);
}

void read_imu_data(){
	accel_x = read_accel_x();
	accel_y = read_accel_y();
	accel_z = read_accel_z();
}
