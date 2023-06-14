#pragma once
#include "ST-LIB_HIGH/Control/ControlBlock.hpp"

template<size_t N>
class Zeroing : public ControlBlock<double,double>{
public:
    int index = 0;
    double mean = 0.0;
    bool completed = false;

public:
    Zeroing():ControlBlock<double,double>(0.0){}
    void execute() override {
    	if(index != N){
            mean += input_value/N;
            index ++;
    	} else {
    		completed = true;
    		output_value = input_value - mean;
    	}
    }

    void reset(){
        mean = 0.0;
        index = 0;
    }
};
