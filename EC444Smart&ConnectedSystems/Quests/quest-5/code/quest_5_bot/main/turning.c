//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
#include "turning.h"

bool turn(float setpoint, float theta){
	//setpoint = setpoint % 360.0;
	if(setpoint < 0){
		setpoint += 360.0;
	}

    float error = setpoint - theta;

	if(error < -5){
		turn_in_place(512, 512, 0);
		
		//movement(0, 512, 1);
		return false;
	}
	if(error > 5){
		turn_in_place(512, 512, 1);
		//movement(512, 0, 1);
		return false;
	}
    return true;
}