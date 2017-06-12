/*
  Draw.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Accelerometer.h"

// Use this pins for the Accelerometer axes
#define accelXpin A0
#define accelYpin A1
#define accelZpin A2

//-------------------Accelerometer variables-----------------
int posXShip,posYShip;
int xVoltage,yVoltage,zVoltage;
float xAcc,yAcc,zAcc,roll,pitch;
float scale=98;

//Callibration values
int xZero=502;
int zZero=512;
int yZero=497;
//Sensitivity to detect direction
int sense=20;
//----------------------------------------------------------

void Accelerometer::readVoltage(){
  xVoltage=analogRead(accelXpin);
  yVoltage=analogRead(accelYpin);
  zVoltage=analogRead(accelZpin);
}

void Accelerometer::getRollPitch(){
  xAcc=(xVoltage-xZero)/scale;
  yAcc=(yVoltage-yZero)/scale;
  zAcc=(zVoltage-zZero)/scale;
  // apply trigonometry to get the pitch and roll:
  pitch = atan(xAcc/sqrt(pow(yAcc,2) + pow(zAcc,2)));
  roll = atan(yAcc/sqrt(pow(xAcc,2) + pow(zAcc,2)));
  //convert radians into degrees
  pitch = pitch * (180.0/PI);
  roll = roll * (180.0/PI) ;
}

int Accelerometer::isShaking(){
  readVoltage();
  getRollPitch();
  if(pitch>30||pitch<-30||roll>30||roll<-30){
    delay(500);
    readVoltage();
    getRollPitch();
    if(pitch>30||pitch<-30||roll>30||roll<-30){
      return 0;  
    }
  }else{
    return 1;
  }
}

int Accelerometer::getRoll(){
  readVoltage();
  getRollPitch();
  return roll;
}

int Accelerometer::getPitch(){
  readVoltage();
  getRollPitch();
  return pitch;
}
