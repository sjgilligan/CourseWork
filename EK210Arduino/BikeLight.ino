#include <Arduino_LSM9DS1.h>
#include <math.h>

float x, y, z;
  int degreesX = 0;
  int degreesY = 0;
  const int lightSensorPin = A0;
  int led_pin = 6;
  bool motion = false;
  
void setup() {
  // put your setup code here, to run once:
  IMU.begin();
}

void loop() {
  IMU.readAcceleration(x, y, z);
  if(x > .1 || y > .1){
    motion = true;
  }
  double lightSensorReading;
  double lightSensorOut;
  if(motion == true){
  lightSensorReading = analogRead(lightSensorPin);
    delay(100);
    //lightSensorReading range: 0 - 500
    //lightSensorOut range:0 - 200
    if(lightSensorReading > 100){
      lightSensorOut = 0;
    }else if(lightSensorReading <= 0){
      lightSensorOut = 200;
    }else{
      lightSensorOut = (lightSensorReading * -2) + 200;
      //lightSensorOut = 100;
      //analogWrite(led_pin, lightSensorOut);
    }
    Serial.println("Light Sensor");
    Serial.println(lightSensorReading);
    Serial.println("Light Out");
    Serial.println(lightSensorOut);
    analogWrite(led_pin, lightSensorOut);
    delay(100);
    }else{
    lightSensorOut = 0;
    analogWrite(led_pin, lightSensorOut);
  }
}
