

//#include <MedianFilter.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "AFMotor.h"
#include "QTRSensors.h"








//Unused Variables. TODO: Delete them
double angle_rad = 0.01745329251;
double angle_deg = 180.0/PI;



//Speed Variables

const int fast = 110;
const int medium = 100;
const int back = 80;
const int slow = 50;

//Setup Variables





// Window size of the median filter (odd number, 1 = no filtering)
//const byte medianFilterWindowSize = 5;

//Object instance of motors and sensors
//SharpDistSensor sensor(DistancesensorPin, medianFilterWindowSize);

AF_DCMotor left_motor(3, MOTOR34_64KHZ);
AF_DCMotor right_motor(4, MOTOR34_64KHZ);

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

    

    qtra.read(sensorValues);
    // 2 = front center, 3 = front left, 0 = back left, 1 = front right, 4 = back right

    //front left on black and front centor sensor on white turn left 
    if(sensorValues[3] > ReflectanceThreshhold(3) && sensorValues[2] < ReflectanceThreshhold(2)){
        left_motor.run(1);
        right_motor.run(1);
        left_motor.setSpeed(medium);
        right_motor.setSpeed(slow);

        return;



    }
    //back left on black and back right on white turn right
    else if(sensorValues[0] > ReflectanceThreshhold(0) && sensorValues[4] < ReflectanceThreshhold(4)){
        left_motor.run(2);
        right_motor.run(2);
        left_motor.setSpeed(slow);
        right_motor.setSpeed(medium);

        return;



    }
    else if((sensorValues[2]) > (ReflectanceThreshhold(2)))
    {
            left_motor.run(1);
            right_motor.run(1);
            left_motor.setSpeed(fast);
            right_motor.setSpeed(fast);
            return;
    }
    else if(sensorValues[3] > ReflectanceThreshhold(3) || sensorValues[4] > ReflectanceThreshhold(4))
    {
          left_motor.run(1);
          right_motor.run(1);
          right_motor.setSpeed(medium);
          left_motor.setSpeed(slow);
          return;
    }
    else if (sensorValues[1] > ReflectanceThreshhold(1) || sensorValues[0] > ReflectanceThreshhold(0))
    {
          left_motor.run(1);
          right_motor.run(1);
          right_motor.setSpeed(slow);
          left_motor.setSpeed(medium);
          return;
    } 
    else 
    {
          left_motor.run(2);
          right_motor.run(2);
          right_motor.setSpeed(back);
          left_motor.setSpeed(back);
          return;
    }


    


    _loop();
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
