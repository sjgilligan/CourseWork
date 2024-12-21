//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
#include <stdio.h>
#include <math.h>
#include "udp_client.h"

#define delta_t 0.1

extern RobotData data;

float x1 = 0;
float x2 = 0;
float z1 = 0;
float z2 = 0;
float Vx = 0;
float Vz = 0;
float dtheta = 0;
double velocity_magnitude;

void get_velocity(void *pvParameters){
	while(1){
	
		x1 = data.x;
		float theta1 = data.theta;
		z1 = data.z;
		
		vTaskDelay(100 / portTICK_PERIOD_MS);
		
		float theta2 = data.theta;
		x2 = data.x;
		z2 = data.z;
		
		Vx = (x2 - x1) / delta_t;
		Vz = (z2 - z1) / delta_t;
		dtheta = (theta2 - theta1) / delta_t;
		double velocity_magnitude = sqrt(Vx * Vx + Vz * Vz);
		
		//printf("Vx: %f, Vz: %f, M: %f\n", Vx, Vz, velocity_magnitude); 
	
	}
}