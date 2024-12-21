//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
#include "butterfly_move.h"
#include "udp_client.h"
#include "turning.h"
#include "range.h"

int id = 7;

#define ANGLE_TOL 5.0

extern RobotData data;
extern bool detection;
extern float activity_time; 

void straight(float des_theta){
	if(data.theta > des_theta + ANGLE_TOL){
		printf("bank RIGHT\n");
		movement(400, 512, 1); //if left bank right
				
	}else if(data.theta < des_theta - ANGLE_TOL){	
		printf("bank LEFT\n");
		movement(512, 400, 1); //if right bank left
	}else{
		printf("MOVING FORWARD\n");
		movement(512, 512, 1);
	}
}

void check_status(){
	printf("Status: %s\n", data.status);
	while(activity_time< 10){
		printf("WASD Control %f\n", activity_time);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	// if(strstr(data.status, "inactive") != NULL){
	// 	//movement(0,0,1);
	// 	vTaskDelay(1000 / portTICK_PERIOD_MS);
	// 	int i = 0;
	// 	while(strstr(data.status, "inactive") != NULL){
	// 		// Back up if out of bounds
	// 		printf("BACKING UP into bounds\n");
	// 		movement(512,512,0);
	// 		vTaskDelay(500 / portTICK_PERIOD_MS);
	// 		i++;
	// 		if(i > 4){
	// 			break;
	// 		}
	// 	}
	// 	movement(0,0,1);
	// 	while(strstr(data.status, "inactive") != NULL){
	// 		printf("Wating for status to change...pick me up\n");
	// 		vTaskDelay(1000 / portTICK_PERIOD_MS);
	// 	}
	// }
	
}

void avoid_object(){
	printf("Object detected, calling avoid function...\n"); 
	movement(0,0,1);
	uint16_t delay = 5000;
	if(data.id == 7){
		delay = 1000;
	}
	while(detection){
		movement(0,0,1);
		printf("Waiting for object to clear\n"); 
		vTaskDelay(500 / portTICK_PERIOD_MS); 

	}
	vTaskDelay(delay / portTICK_PERIOD_MS); 
	//movement(512,512,0);
	if(data.id){
		vTaskDelay(delay / portTICK_PERIOD_MS); 
	}
	else
		vTaskDelay(700 / portTICK_PERIOD_MS);
	
	movement(0,0,1);
	vTaskDelay(700 / portTICK_PERIOD_MS);
}

float previous_error = 0;
float integral = 0;

float Kp = 4;
float Ki = 0.2;
float Kd = 0;
float output = 0;

void reset_pid(){
	previous_error = 0;
	integral = 0;
	output = 0;
	movement(0, 0, 1);
}

void pid(float des_theta){

  float error = des_theta - data.theta;
  integral = integral + error;
  float derivative = (error - previous_error);
  output = Kp * error + Ki * integral + Kd * derivative;
  if(output > 512){
	output = 512;
  }
  previous_error = error;
  uint16_t duty_left = 512 + output;
  uint16_t duty_right = 512 - output;
  // Postive error means bank right, negative error means bank left
  movement(duty_left, duty_right, 1);
  printf("Error: %f\tIntegral %f\tderivative %f\t Output %f\n", error, integral, derivative, output);
  printf("Duty Left: %d\tDuty Right: %d\n", duty_left, duty_right);
}




void butterfly_move_task(void *pvParameters)
{
	if(id == 7){
	Kp = 3;
	Ki = 0.1;
    while(1)
    {
		movement(0, 0, 1);
		reset_pid();
		check_status();
		vTaskDelay(3000 / portTICK_PERIOD_MS);

		if(detection){ 
				avoid_object();
		}

        bool waypoint1 = false;
        while(waypoint1 == false)
        {
            printf("Waypoint 1: Angle: %f\n",data.theta);
            
            if(data.x > 745.0){
                waypoint1 = true;
			}
			float delta_x = 745.0-data.x;
			float delta_z = -497-data.z;
			float des_theta = atan(delta_x/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			if(delta_x > 0){
				des_theta = des_theta + 179.9;
			}
			if(delta_z < 0){
				des_theta = des_theta + 45;
			}
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
			//straight(270.0);
			
			if(detection){ 
				avoid_object();
			}
			check_status();
            vTaskDelay(50 / portTICK_PERIOD_MS); 
        }
        printf("Waypoint 1\n");
		reset_pid();
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

        bool turn1 = false;
        while(!turn1)
        {
			printf("Angle: %f\n",data.theta);
            turn1 = turn(124.0,data.theta);
            vTaskDelay(50 / portTICK_PERIOD_MS);
				if(detection){ 
				avoid_object();
			}
			check_status();

        }
		
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
		bool waypoint2 = false;
        while(waypoint2 == false)
        {
            printf("Waypoint 2: Angle: %f\n",data.theta);
            
            if(data.x < -728.0){
                waypoint2 = true;
			}

			float delta_x = -728-data.x;
			float delta_z = 730-data.z;
			float des_theta = atan(fabsf(delta_x)/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			if(delta_x > 0){
				des_theta = des_theta + 179.9;
			}else if(delta_x < 0){
				des_theta = 180 - des_theta;
			}
			if(delta_z < 0){
				des_theta = des_theta + 45;
			}
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
						
			if(detection){ 
				avoid_object();
			}
			check_status();
			
            vTaskDelay(50 / portTICK_PERIOD_MS); 
        }
        printf("Waypoint 2\n");
		reset_pid();
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

		
		bool turn2 = false;
        while(!turn2)
        {
			printf("Angle: %f\n",data.theta);
            turn2 = turn(270.0,data.theta);
            vTaskDelay(50 / portTICK_PERIOD_MS);  
        }
		
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
		bool waypoint3 = false;
        while(waypoint3 == false)
        {
            printf("Waypoint 3: Angle: %f\n",data.theta);
            
            if(data.x > 765.0){
                waypoint3 = true;
			}

			float delta_x = 765-data.x;
			float delta_z = 748-data.z;
			float des_theta = atan(fabsf(delta_x)/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			if(delta_x > 0){
				des_theta = des_theta + 179.9;
			}else if(delta_x < 0){
				des_theta = 180 - des_theta;
			}
			if(delta_z < 0){
				des_theta = des_theta + 45;
			}
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
						
			if(detection){ 
				avoid_object();
			}
			check_status();
			
            vTaskDelay(50 / portTICK_PERIOD_MS); 
        }
        printf("Waypoint 3\n");
		
		reset_pid();
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
		bool turn3 = false;
        while(!turn3)
        {
			printf("Angle: %f\n",data.theta);
            turn3 = turn(51.0,data.theta);
            vTaskDelay(50 / portTICK_PERIOD_MS);  
        }
		
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
		bool waypoint4 = false;
        while(waypoint4 == false)
        {
            printf("Waypoint 4: Angle: %f\n",data.theta);
            
            if(data.x < -728.0){
                waypoint4 = true;
			}

			float delta_x = -728-data.x;
			float delta_z = -455-data.z;
			float des_theta = atan(fabsf(delta_x)/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
						
			if(detection){ 
				avoid_object();
			}
			check_status();
			
            vTaskDelay(50 / portTICK_PERIOD_MS); 
        }
        printf("Waypoint 4\n");
		
		reset_pid();

		while(1){
			movement(0, 0, 1);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
        
        vTaskDelay(500 / portTICK_PERIOD_MS);       
    }
	}else if(id == 8){ ///////////////////////////////////////////////////////////
	Kp = 4;
	Ki = 0.2;
	bool turn0 = false;
		if(detection){ 
				avoid_object();
		}
        while(!turn0)
        {
			printf("Angle: %f\n",data.theta);
            turn0 = turn(270,data.theta);
            vTaskDelay(50 / portTICK_PERIOD_MS);
				if(detection){ 
				avoid_object();
			}
			check_status();

        }
	
		movement(0, 0, 1);
		reset_pid();
		check_status();
		vTaskDelay(3000 / portTICK_PERIOD_MS);
        bool waypoint1 = false;
        while(waypoint1 == false)
        {
            printf("Waypoint 1: Angle: %f\n",data.theta);
            
            if(data.x > 760.0){
                waypoint1 = true;
			}
			float delta_x = 760.0-data.x;
			float delta_z = 748-data.z;
			float des_theta = atan(delta_x/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			if(delta_x > 0){
				des_theta = des_theta + 179.9;
			}
			if(delta_z < 0){
				des_theta = des_theta + 45;
			}
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
			//straight(270.0);
			
			if(detection){ 
				avoid_object();
			}
			check_status();
            vTaskDelay(50 / portTICK_PERIOD_MS); 
        }
        printf("Waypoint 1\n");
		reset_pid();
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

        bool turn1 = false;
        while(!turn1)
        {
			printf("Angle: %f\n",data.theta);
            turn1 = turn(51.0,data.theta);
            vTaskDelay(50 / portTICK_PERIOD_MS);
				if(detection){ 
				avoid_object();
			}
			check_status();

        }
		
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
		bool waypoint2 = false;
        while(waypoint2 == false)
        {
            printf("Waypoint 2: Angle: %f\n",data.theta);
            
            if(data.x < -725.0){
                waypoint2 = true;
			}

			float delta_x = -725-data.x;
			float delta_z = -455-data.z;
			float des_theta = atan(fabsf(delta_x)/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
						
			if(detection){ 
				avoid_object();
			}
			check_status();
			
            vTaskDelay(50 / portTICK_PERIOD_MS); 
        }
        printf("Waypoint 2\n");
		reset_pid();
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

		
		bool turn2 = false;
        while(!turn2)
        {
			printf("Angle: %f\n",data.theta);
            turn2 = turn(270.0,data.theta);
            vTaskDelay(50 / portTICK_PERIOD_MS);  
        }

		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		reset_pid();
		bool waypoint3 = false;
        while(waypoint3 == false)
        {
            printf("Waypoint 3: Angle: %f\n",data.theta);
            
            if(data.x > 745.0){
                waypoint3 = true;
			}
			float delta_x = 745.0-data.x;
			float delta_z = -497-data.z;
			float des_theta = atan(delta_x/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			if(delta_x > 0){
				des_theta = des_theta + 179.9;
			}
			if(delta_z < 0){
				des_theta = des_theta + 45;
			}
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
			//straight(270.0);
			
			if(detection){ 
				avoid_object();
			}
			check_status();
            vTaskDelay(50 / portTICK_PERIOD_MS); 
        }
        printf("Waypoint 3\n");
		
		reset_pid();
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
		bool turn3 = false;
        while(!turn3)
        {
			printf("Angle: %f\n",data.theta);
            turn3 = turn(124.0,data.theta);
            vTaskDelay(50 / portTICK_PERIOD_MS);  
        }
		
		movement(0, 0, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
		bool waypoint4 = false;
        while(waypoint4 == false)
        {
           printf("Waypoint 4: Angle: %f\n",data.theta);
            
            if(data.x < -728.0){
                waypoint4 = true;
			}

			float delta_x = -728-data.x;
			float delta_z = 730-data.z;
			float des_theta = atan(fabsf(delta_x)/fabsf(delta_z)) / (M_PI/180);
			printf("Calculated: %f, delta x: %f, delta z, %f\n",des_theta, delta_x, delta_z);
			if(delta_x > 0){
				des_theta = des_theta + 179.9;
			}else if(delta_x < 0){
				des_theta = 180 - des_theta;
			}
			if(delta_z < 0){
				des_theta = des_theta + 45;
			}
			printf("Desired Theta: %f\n", des_theta);
			pid(des_theta);
						
			if(detection){ 
				avoid_object();
			}
			check_status();
			
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
        printf("Waypoint 4\n");
		
		reset_pid();

		while(1){
			movement(0, 0, 1);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
        
        vTaskDelay(500 / portTICK_PERIOD_MS);  
	}
}