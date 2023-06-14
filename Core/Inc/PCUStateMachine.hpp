#pragma once
#include "PCU.hpp"

class GeneralStateMachine{
public:
	Traction::PCU& pcu;
	StateMachine general_state_machine;
	bool tcp_timeout = false;
	enum States{ OPERATIONAL, FAULT };

	GeneralStateMachine(Traction::PCU& pcu) : pcu(pcu) { }

	void init(){
		general_state_machine = { OPERATIONAL };
		general_state_machine.add_state(FAULT);
		ProtectionManager::link_state_machine(general_state_machine, FAULT);
		ProtectionManager::set_id(Boards::ID::PCU);
		add_on_enter_actions();
		add_on_exit_actions();
	}

	void add_on_enter_actions(){
		general_state_machine.add_enter_action([&](){
			 pcu.fault_led.turn_on();
			 pcu.stop_control();
		}, FAULT);

		general_state_machine.add_enter_action([&](){
			pcu.operational_led.turn_on();
		}, OPERATIONAL);
	}

	void add_on_exit_actions(){
		general_state_machine.add_exit_action([&](){
			pcu.fault_led.turn_off();
		}, FAULT);

		general_state_machine.add_exit_action([&](){
			pcu.operational_led.turn_off();
		}, OPERATIONAL);
	}
};
