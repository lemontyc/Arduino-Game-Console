/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Accelerometer_h
#define Accelerometer_h

#include "Arduino.h"


class Accelerometer
{
  public:
    /*
     *  Reads Voltage from accelerometer Pins 
     */
    void readVoltage();
    /*
     * Does some fancy trigonometry in order to get roll/pitch from voltage readings
     * We had to calibrate the accelereometer BEFORE applying maths
     * Reference: http://physics.rutgers.edu/~aatish/teach/srr/workshop3.pdf
     */
    void getRollPitch();
    int isShaking();  // Returns 1 if NOT shaking
    int getPitch();   //Returns current pitch value
    int getRoll();    //Returns current roll value
  private:
};
#endif

