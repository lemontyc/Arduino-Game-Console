/***************************************************
  This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 
#ifndef Draw_h
#define Draw_h

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "Arduino.h"
#include "Accelerometer.h"

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

class Draw
{
  public:
    void setUp();   // Sets up the screen. ROTATION SET here
    void doIntro(); // Fancy intro animation
    int doMenu();   // Displays the menu. Returns selected option
    void gameOver(unsigned int score,float shots, float lasers);
    void updateScore(unsigned int score);
    void stars();
    void credits();
    void newGame(char level,char lifes,unsigned int sccore); // Draws the new game screen
    void transition_lines(uint16_t color);  // Fancy transition animation from adafruit library
    void highScores(int scores[5], char names[15]);
    void updateNames(int pos, char names[3]);
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);  // Screen object. VERY IMPORTANT
    /*
     * Draws all the custom objects on screen. Needs object type, X coordinate, Y coordinate, color and scale
     * CREATE NEW objects HERE
     */
    void object(int type, int posX, int posY,  uint16_t color,int scale); 
  private:
    int _i;
    int _j;
    Accelerometer accelerometer;        
};
#endif

