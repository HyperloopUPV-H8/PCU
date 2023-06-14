#include "main.h"
#include "lwip.h"
#include "lwip/udp.h"
#include "ST-LIB.hpp"
#include "Runes/Runes.hpp"
#include "IMU/IMU.hpp"
#include "PCU.hpp"
#include "PCUStateMachine.hpp"

#include <string.h>

void MX_SPI3_Init();
void MX_GPIO_Ini();
Traction::PCU* pcu;
GeneralStateMachine* general_state_machine;
uint16_t id;
uint8_t adc;
bool do_fault_test = false;

/////////////////////////////////////
//
//			Cyclic actions
//
/////////////////////////////////////

int adc_index = 0;
float adc_value;
void read_sensors() {
	adc_index += 1;
	pcu->read_adcs();
	read_imu_data();
	pcu->accel_x = accel_x;
	pcu->accel_y = accel_y;
	pcu->accel_z = accel_z;
	pcu->velocity = 0;
	pcu->send_all_data();

	if(do_fault_test){
		pcu->left_ppu.motor_temperatures[0] = 300;
		pcu->left_ppu.phase_currents[0] = 700;
	}

	for(int i = 0; i < 3; i++) {
		if(pcu->left_ppu.current_zeroing[0].completed && pcu->left_ppu.phase_currents[i] > 50)
			pcu->stop_control();
	}
}

void stop_control(){
	pcu->stop_control();
}

int svpwm_index = 0;
void exec_svpwm()	{ pcu->exec_svpwm(); svpwm_index++; }
void stop_contro_order() { pcu->stop_control(); }
void test_pwm()		{
	pcu->test_svpmw();
}

void check_transitions(){
	general_state_machine->general_state_machine.check_transitions();
}

void test_fault_callback(){
	do_fault_test = true;
}

/////////////////////////////////////
//
//				Orders
//
/////////////////////////////////////

void toggle_led() {
	pcu->flash_led.toggle();
}

float 	test_duty_target;
uint8_t test_duty_phase;
void  test_duty() {
	pcu->sleep_led.toggle();
	pcu->left_ppu.phase_pwms[test_duty_phase].set_duty_cycle(test_duty_target);
	for(int i = 0; i < 3; i++){
		if(i != test_duty_phase)
			pcu->left_ppu.phase_pwms[i].set_duty_cycle(0);
	}
}

void reset(){
	HAL_NVIC_SystemReset();
}

int main(void) {
	pcu = new Traction::PCU();
	general_state_machine = new GeneralStateMachine(*pcu);

	Pin::inscribe(PC10, ALTERNATIVE);
	Pin::inscribe(PC11, ALTERNATIVE);
	Pin::inscribe(PC12, ALTERNATIVE);
	Pin::inscribe(PD3, OUTPUT);

	STLIB::start("192.168.1.6", "255.255.0.0", "192.168.1.1");
	MX_SPI3_Init();
	pcu -> start();

	start_imu();
	id = read_id();

	[[maybe_unused]]StackOrder toggle_led_order = { 600, toggle_led };
	[[maybe_unused]]StackOrder test_duty_order	= { 601, test_duty, &test_duty_target, &test_duty_phase };
	[[maybe_unused]]StackOrder reset_order		= { 602, reset };
	[[maybe_unused]]StackOrder test_svpwm		= { 607, test_pwm, &pcu->svpwm_frequency, &pcu->svpwm_voltage, &pcu->svpwm_vdc};
	[[maybe_unused]]StackOrder stop_control		= { 609, stop_contro_order};
	[[maybe_unused]]StackOrder test_fault		= { 611, test_fault_callback};

	Time::register_high_precision_alarm(pcu->SVPWM_EXEC_PERIOD * 1000000, exec_svpwm);
	Time::register_high_precision_alarm(1000, read_sensors);
	Time::register_low_precision_alarm(1, ErrorHandlerModel::ErrorHandlerUpdate);
	Time::register_low_precision_alarm(1, ProtectionManager::check_protections);
	Time::register_low_precision_alarm(1, ProtectionManager::check_protections);
	Time::register_low_precision_alarm(1, check_transitions);
	ServerSocket tcp_server(IPV4("192.168.1.6"), 50500);
	general_state_machine->init();
	pcu->add_protections();

	while(1) {
		STLIB::update();
	}

	return 0;
}

void MX_SPI3_Init(void)
{
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 0x0;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void) {
  __disable_irq();
  while (1);
}
