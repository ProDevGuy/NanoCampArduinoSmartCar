#include <MedianFilter.h>
#include <SharpDistSensor.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "AFMotor.h"
#include "QTRSensors.h"



#define DistancesensorPin A5




//Unused Variables. TODO: Delete them
double angle_rad = 0.01745329251;
double angle_deg = 180.0/PI;



//Speed Variables

const int fast = 110;
const int medium = 100;
const int slow = 40;

//Setup Variables



float volts;
float distance;





// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

//Object instance of motors and sensors
SharpDistSensor sensor(DistancesensorPin, medianFilterWindowSize);

AF_DCMotor left_motor(3, MOTOR12_1KHZ);
AF_DCMotor right_motor(4, MOTOR12_1KHZ);

QTRSensorsAnalog qtra((unsigned char[]) {1, 2, 3, 4},5, 4, QTR_NO_EMITTER_PIN);



//Arrays

unsigned int sensorValues[5];




void setup(){

    Serial.begin(9600);
    for (int i = 0; i < 300; i++){ //300 = 7.5 seconds
         qtra.calibrate();
    }
    Serial.print("Calibration Done");
    Serial.println(".");
}





  
void loop(){

    //Gets distance from sensor
    volts = analogRead(DistancesensorPin)*0.0048828125;
    distance = 13*pow(volts,-1);;
    // Print distance to Serial
    Serial.println(distance);

    // Wait some time
    delay(50);
    
    
    
    if(distance < 7){

      //Distance = 0
      left_motor.run(2);
      right_motor.run(2);
      left_motor.setSpeed(fast);
      right_motor.setSpeed(fast);

      delay(250);
      left_motor.run(1);
      right_motor.run(1);
      right_motor.setSpeed(medium);
      left_motor.setSpeed(slow);
      delay(500);

    }

    else
    {

      CheckSensorValuesAndMove();


    }

    _loop();
}

void CheckSensorValuesAndMove(){

    qtra.read(sensorValues);



    if((sensorValues[2]) > (ReflectanceThreshhold(2)))
    {
            left_motor.run(1);
            right_motor.run(1);
            left_motor.setSpeed(fast);
            right_motor.setSpeed(fast);
    }
    else if(sensorValues[3] > ReflectanceThreshhold(3) || sensorValues[0] > ReflectanceThreshhold(0))
    {
          left_motor.run(1);
          right_motor.run(1);
          right_motor.setSpeed(medium);
          left_motor.setSpeed(slow);
    }
    else if (sensorValues[1] > ReflectanceThreshhold(1) || sensorValues[4] > ReflectanceThreshhold(4))
    {
          left_motor.run(1);
          right_motor.run(1);
          right_motor.setSpeed(slow);
          left_motor.setSpeed(medium);
    } 
    else 
    {
          left_motor.run(2);
          right_motor.run(2);
          right_motor.setSpeed(70);
          left_motor.setSpeed(70);
    }

  }



 

float ReflectanceThreshhold(int a){

  float b = 2*(qtra.calibratedMaximumOn[a] - qtra.calibratedMinimumOn[a])/3 + qtra.calibratedMinimumOn[a];
  return b;

}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){}
