#include <MedianFilter.h>
#include <SharpDistSensor.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "AFMotor.h"
#include "QTRSensors.h"






int fast = 200;
int medium = 100;
int slow = 50;



// Analog pin to which the sensor is connected
const byte sensorPin = A0;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

// Create an object instance of the SharpDistSensor class
SharpDistSensor sensor(sensorPin, medianFilterWindowSize);

const double distanceMultipler = 0.0114285714;

float volts;
float distance;

double angle_rad = 0.01745329251;
double angle_deg = 180.0/PI;

QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4},5, 4, QTR_NO_EMITTER_PIN);

unsigned int sensorValues[5];

AF_DCMotor left_motor(3, MOTOR12_1KHZ);
AF_DCMotor right_motor(4, MOTOR12_1KHZ);


void setup(){

    Serial.begin(9600);
    for (int i = 0; i < 300; i++){ //300 = 7.5 seconds
         qtra.calibrate();
    }
    Serial.print("Calibration Done");
}

void loop(){

    //unsigned int distance = sensor.getDist();
    volts = analogRead(sensorPin)*0.0048828125;
    distance = 13*pow(volts,-1);;
    // Print distance to Serial
    Serial.println(distance);

    // Wait some time
    delay(50);

    left_motor.run(1);
    right_motor.run(1);


    if((sensorValues[2]) > (ReflectanceThreshhold(2))){
        if((sensorValues[4]) > (ReflectanceThreshhold(4))){
            
            left_motor.setSpeed(fast);
            right_motor.setSpeed(fast);
        }else{
            left_motor.setSpeed(medium);
            right_motor.setSpeed(medium);
        }
    }else{
        if((sensorValues[3]) > (ReflectanceThreshhold(3))){
          right_motor.setSpeed(medium);
          left_motor.setSpeed(slow);
        }
        if ((sensorValues[1]) > ReflectanceThreshhold(1)){
          right_motor.setSpeed(slow);
          left_motor.setSpeed(medium);


        }
        else{
          left_motor.run(-1);
          right_motor.run(-1);
          right_motor.setSpeed(slow);
          left_motor.setSpeed(slow);


        }



    }

    if(distance > 50 || distance < 7){

      //Distance = 0
      left_motor.run(-1);
      right_motor.run(-1);

      delay(1000);
      left_motor.run(1);
      right_motor.run(1);
      right_motor.setSpeed(medium);
      left_motor.setSpeed(slow);


    }

    _loop();
}

int ReflectanceThreshhold(int a){

  2*(qtra.calibratedMaximumOn[a] - qtra.calibratedMinimumOn[a])/3 + qtra.calibratedMinimumOn[a];

}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
}