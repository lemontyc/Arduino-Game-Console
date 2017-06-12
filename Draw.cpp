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

#include "Arduino.h"
#include "Draw.h"


void Draw::setUp(){
  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("init");
  tft.setTextWrap(true); // Don't allow text to run off right edge
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);  //Set rotation as landscape with pins to right side
  //------------------------- Do NOT modify before line ------------------
  
}

void Draw::doIntro(){
  int posX,posY;
  randomSeed(analogRead(A7));
  tft.fillScreen(ST7735_BLACK);   //Clear Screen
  //Draw Stars
  for(_i=0;_i<50;_i++){
    for(_j=0;_j<10;_j++){
        posX=random(160);
        posY=random(128);
        tft.drawPixel(posX, posY, ST7735_WHITE);
        delay(5);
    }
  }
  //Draw Text
  tft.setCursor(47,38);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(3);
  tft.println("GAME");
  delay(700);

  tft.setCursor(29,62);
  tft.setTextColor(ST7735_RED);
  tft.println("THING");
  delay(700);

  tft.setCursor(119,54);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(4);
  tft.println("2");
  
  delay(700);
  tft.setCursor(35,84);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(1);
  tft.println("BY Reuben & Luis");
  delay(700);

  //Shake to start Animation, otherwise do this thing:
  _i=posX=posY=0;
  int shake=1;
  while(shake){
     tft.setCursor(50+posX,100+posY);
     tft.setTextColor(ST7735_BLACK);
     tft.setTextSize(2);
     tft.println("SHAKE");

     posX=random(5);
     posY=random(5);
     
     tft.setCursor(50+posX,100+posY);
     tft.setTextColor(ST7735_GREEN);
     tft.setTextSize(2);
     tft.println("SHAKE");
     delay(50);
     
     shake=accelerometer.isShaking();
  }


}

int Draw::doMenu(){
  int posX,posY,roll;
  //Fill Screen eith white
  for(_i=0;_i<1000;_i++){
    for(_j=0;_j<100;_j++){
        posX=random(160);
        posY=random(128);
        tft.drawPixel(posX, posY, ST7735_WHITE);
    }
  }
  tft.fillScreen(ST7735_WHITE);   //FILL remaining Pixels white
  
  tft.setCursor(37,32);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(2);
  tft.println("NEW GAME");

  tft.setCursor(37,50);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(2);
  tft.println("H SCORES");

  tft.setCursor(37,68);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(2);
  tft.println("CREDITS");

  object(13,25,100,ST7735_RED,2);
  tft.setCursor(35,104);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.println("Move");
  
  object(14,80,102,ST7735_RED,2);
  tft.setCursor(89,104);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.println("Select");
  
  posY=31;


  //Add 18 to posY  of invader select menu
    
  while(roll>-30){ 
  //Do this trickery until selected 
    if(accelerometer.getPitch()>-20){
     posY= 31;
    }else if(accelerometer.getPitch()>-40){
      posY=49;
    }else{
      posY=67;
    }
    object(3,15,posY,ST7735_MAGENTA,2); 
    delay(20);
    tft.invertDisplay(true);
    delay(100);    
    object(3,15,posY,ST7735_WHITE,2); 
    roll=accelerometer.getRoll();

    if(accelerometer.getPitch()>-20){
     posY= 31;
    }else if(accelerometer.getPitch()>-40){
      posY=49;
    }else{
      posY=67;
    }
    //delay(10);
    object(3,15,posY,ST7735_MAGENTA,2); 
    delay(20);
    tft.invertDisplay(false);
    delay(100);    
    object(3,15,posY,ST7735_WHITE,2); 
    roll=accelerometer.getRoll();
   
  }
  
  switch(posY){
    case 31:
      
      Serial.println("Start");
      return 1;
      break;
    case 49:
      //showHS();
      Serial.println("HS");
      return 2;
      break;
    case 67:
      //doCredits();
      Serial.println("Creds");
      return 3;
      break;
  }

  
  
  
}

void Draw::newGame(char level,char lifes,unsigned int score){
  int posX, posY;
  transition_lines(ST7735_BLACK);
  tft.fillScreen(ST7735_BLACK);
  //Draw Stars
  for(_i=0;_i<25;_i++){
    for(_j=0;_j<5;_j++){
        posX=random(160);
        posY=random(128);
        tft.drawPixel(posX, posY, ST7735_WHITE);
        delay(5);
    }
  }

  //Draw Level text
  tft.setCursor(3,3);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(1);
  tft.println("LVL:");
  tft.setCursor(28,3);
  tft.print((int)level);

  //Draw level Score
  tft.setCursor(63,3);
  tft.setTextColor(ST7735_RED);
  tft.println("SCORE");
  
  tft.setCursor(76,12);
  tft.setTextColor(ST7735_WHITE);
  tft.print(score);

  //Draw level life
  tft.setCursor(110,3);
  tft.setTextColor(ST7735_YELLOW);
  tft.println("LIFE:");
  switch(lifes){
    case 1:
      object(12,139,1,ST7735_RED,1);
      break;
    case 2:
      object(12,139,1,ST7735_RED,1);
      object(12,146,1,ST7735_RED,1);
      break;
    case 3:
      object(12,139,1,ST7735_RED,1);
      object(12,146,1,ST7735_RED,1);
      object(12,153,1,ST7735_RED,1);
      break;
    case 4:
      object(12,139,1,ST7735_RED,1);
      object(12,146,1,ST7735_RED,1);
      object(12,153,1,ST7735_RED,1);
    
      object(12,139,8,ST7735_RED,1);
      break;
    case 5:
      object(12,139,1,ST7735_RED,1);
      object(12,146,1,ST7735_RED,1);
      object(12,153,1,ST7735_RED,1);
    
      object(12,139,8,ST7735_RED,1);
      object(12,146,8,ST7735_RED,1);
      break;
    case 6:
      object(12,139,1,ST7735_RED,1);
      object(12,146,1,ST7735_RED,1);
      object(12,153,1,ST7735_RED,1);
    
      object(12,139,8,ST7735_RED,1);
      object(12,146,8,ST7735_RED,1);
      object(12,153,8,ST7735_RED,1);
      break;
  }


  //Draw Human Spaceship
  //object(4,80,119,ST7735_CYAN,1);
}


 

char updateLCD(char (*old)[160],char (*newT)[160]){
  return 'l';
}


void Draw::transition_lines(uint16_t color){
  // tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
  }

  //tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
  }

  //tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
  }

  //tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
  }
}


void Draw::gameOver(unsigned int score,float shots, float lasers){
  transition_lines(ST7735_WHITE);
  tft.fillScreen(ST7735_WHITE);

  tft.setCursor(47,18);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(3);
  tft.println("GAME");
  delay(700);

  tft.setCursor(47,48);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(3);
  tft.println("OVER");
  delay(700);

  tft.setCursor(30,78);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.println("Score:");


  tft.setCursor(70,74);
  tft.setTextColor(ST7735_MAGENTA);
  tft.setTextSize(2);
  tft.println(score);
  delay(500);
  
  tft.setCursor(15,99);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.println("Shots fired:");
  
  tft.setCursor(90,99);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.println(lasers);
  
  tft.setCursor(15,112);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.println("Accuracy:");

  tft.setCursor(80,112);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.println((shots/lasers)*100);

  tft.setCursor(120,108);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.println("%");
  
  delay(4500);

  
  
  
}

void Draw::updateScore(unsigned int score){
  tft.fillRect(75,12,20,8,ST7735_BLACK);
  tft.setCursor(76,12);
  tft.setTextColor(ST7735_WHITE);
  tft.print(score);
}


void Draw::highScores(int scores[5], char names[15]){
  int posX, posY,m;
  tft.fillScreen(ST7735_BLACK);
  //Draw Stars
  for(_i=0;_i<25;_i++){
    for(_j=0;_j<5;_j++){
        posX=random(160);
        posY=random(128);
        tft.drawPixel(posX, posY, ST7735_YELLOW);
        delay(5);
    }
  }
  tft.setCursor(15,2);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println("High Scores");
  delay(300);
  //------------------------------ 1
  tft.setCursor(20,20);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println(1);
  
  tft.setCursor(35,20);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2);
  tft.println(names[0]);

  tft.setCursor(50,20);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2);
  tft.println(names[1]);
  
  tft.setCursor(65,20);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2);
  tft.println(names[2]);

  tft.setCursor(90,20);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println(scores[0]);
  delay(300);

  //------------------------------ 2
  tft.setCursor(20,40);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println(2);
  
  tft.setCursor(35,40);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.println(names[3]);

  tft.setCursor(50,40);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.println(names[4]);
  
  tft.setCursor(65,40);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.println(names[5]);

  tft.setCursor(90,40);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println(scores[1]);
  delay(300);

  //------------------------------ 3
  tft.setCursor(20,60);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println(3);
  
  tft.setCursor(35,60);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(2);
  tft.println(names[6]);

  tft.setCursor(50,60);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(2);
  tft.println(names[7]);
  
  tft.setCursor(65,60);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(2);
  tft.println(names[8]);

  tft.setCursor(90,60);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println(scores[2]);
  delay(300);

  //------------------------------ 4
  tft.setCursor(20,80);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println(4);
  
  tft.setCursor(35,80);
  tft.setTextColor(ST7735_MAGENTA);
  tft.setTextSize(2);
  tft.println(names[9]);

  tft.setCursor(50,80);
  tft.setTextColor(ST7735_MAGENTA);
  tft.setTextSize(2);
  tft.println(names[10]);
  
  tft.setCursor(65,80);
  tft.setTextColor(ST7735_MAGENTA);
  tft.setTextSize(2);
  tft.println(names[11]);

  tft.setCursor(90,80);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println(scores[3]);
  delay(300);

  //------------------------------ 5
  tft.setCursor(20,100);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println(5);
  
  tft.setCursor(35,100);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(2);
  tft.println(names[12]);

  tft.setCursor(50,100);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(2);
  tft.println(names[13]);
  
  tft.setCursor(65,100);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(2);
  tft.println(names[14]);

  tft.setCursor(90,100);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println(scores[4]);
  

  delay(5000);
  
}

void Draw::stars(){
  int posX, posY,m;
  tft.fillScreen(ST7735_BLACK);
  //Draw Stars
  for(_i=0;_i<25;_i++){
    for(_j=0;_j<5;_j++){
        posX=random(160);
        posY=random(128);
        tft.drawPixel(posX, posY, ST7735_YELLOW);
        delay(5);
    }
  }
    tft.setCursor(40,2);
    tft.setTextColor(ST7735_RED);
    tft.setTextSize(2);
    tft.println("Congrats!");
    
    tft.setCursor(15,30);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.println("Enter");

    tft.setCursor(17,39);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.println("your");

    tft.setCursor(16,47);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.println("name:");

    object(13,25,100,ST7735_RED,2);
    tft.setCursor(35,104);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.println("Letters");
    
    object(14,90,102,ST7735_RED,2);
    tft.setCursor(99,104);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.println("Move");

}
void Draw::updateNames(int pos, char names[3]){
    
    tft.fillRect(65,28,80,40,ST7735_BLACK);
    
    tft.setCursor(65,28);
    tft.setTextColor(ST7735_BLUE);
    tft.setTextSize(4);
    tft.println(names);
    switch(pos){
      case 0:
        tft.fillRect(65,60,20,2,ST7735_YELLOW);
        break;
      case 1:
        tft.fillRect(89,60,20,2,ST7735_YELLOW);
        break;
      case 2:
        tft.fillRect(113,60,20,2,ST7735_YELLOW);
        break;
      case 3:
        //tft.fillRect(137,60,20,2,ST7735_YELLOW);
        break;
    }
    
    delay(40);
}

void Draw::credits(){
  tft.fillScreen(ST7735_BLACK);
  int color = 100;
  int i;
  int t;
  int k=0;
  
  tft.setCursor(40,60);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.println("Luis & Reuben:");
  while(k<10){
    for(t = 0 ; t <= 4; t+=1) {
      int x = 0;
      int y = 0;
      int w = tft.width()-2;
      int h = tft.height()-2;
      for(i = 0 ; i <= 16; i+=1) {
        tft.drawRoundRect(x, y, w, h, 5, color);
        x+=2;
        y+=3;
        w-=4;
        h-=6;
        color+=1100;
      }
      color+=100;
    }
    k++;
  }
   
  
}

void Draw::object(int type, int posX, int posY,  uint16_t color,int scale){
  /* 
   * 1: Small invader GREEN
   * 2: Small invader RED
   * 3: Medium invader  GREEN
   * 4: Medium invader  CYAN
   * 5: Large invader CYAN
   * 6: Large invader MAGENTA
   * 7: Human Spaceship CYAN
   * 8: Human Spaceship YELLOW
   * 9: Explosion WHITE
   * 10: Enemeny laser WHITE
   * 11: Huma laser YELLOW
   * 12: Heart ANY
   * 13: UP/Down arrows ANY
   * 14: Right arrow ANY
   * 
   */
  switch(type){
    case 1: //SMALL invader
      if(color!=ST7735_BLACK){
        color=ST7735_GREEN;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(1*scale),posY,scale,scale,color);
      
      tft.fillRect(posX-(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX,posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(7*scale),scale,scale,color);
      
      break;
    case 2: //SMALL invader
      if(color!=ST7735_BLACK){
        color=ST7735_RED;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(1*scale),posY,scale,scale,color);
      
      tft.fillRect(posX-(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX,posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(7*scale),scale,scale,color);
      
      break;
    case 3: //MEDIUM invader
      if(color!=ST7735_BLACK && color!=ST7735_WHITE){
        color=ST7735_GREEN;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(6*scale),posY,scale,scale,color);

      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(8*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX,posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(8*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX,posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(8*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX+(1*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(7*scale),scale,scale,color);
      break;
    case 4: //MEDIUM invader
      if(color!=ST7735_BLACK && color!=ST7735_WHITE){
        color=ST7735_CYAN;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(6*scale),posY,scale,scale,color);

      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(8*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX,posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(8*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX,posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(8*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX+(1*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(7*scale),scale,scale,color);
      break;
    case 5: //BIG INVADER
      if(color!=ST7735_BLACK){
        color=ST7735_CYAN;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(1*scale),posY,scale,scale,color);
      tft.fillRect(posX+(2*scale),posY,scale,scale,color);
      tft.fillRect(posX+(3*scale),posY,scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX,posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(7*scale),scale,scale,color);
      
      break;
    case 6: //BIG INVADER
      if(color!=ST7735_BLACK){
        color=ST7735_MAGENTA;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(1*scale),posY,scale,scale,color);
      tft.fillRect(posX+(2*scale),posY,scale,scale,color);
      tft.fillRect(posX+(3*scale),posY,scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX,posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(7*scale),scale,scale,color);
      
      break;
    case 7: //Human spaceship
      if(color!=ST7735_BLACK){
        color=ST7735_CYAN;
      }
      tft.fillRect(posX,posY,scale,scale,color);
        
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      
      tft.fillRect(posX-(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(3*scale),scale,scale,color);
      //tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      //tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(5*scale),scale,scale,color);
      //tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX,posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX,posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(7*scale),scale,scale,color);
      
      break;
    case 8: //Human spaceship
      if(color!=ST7735_BLACK){
        color=ST7735_YELLOW;
      }
      tft.fillRect(posX,posY,scale,scale,color);
        
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      
      tft.fillRect(posX-(2*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(3*scale),scale,scale,color);
      //tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(4*scale),scale,scale,color);
      //tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(5*scale),scale,scale,color);
      //tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX,posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX,posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(7*scale),scale,scale,color);
      
      break;
    case 9:     //explosion
      if(color!=ST7735_BLACK){
        color=ST7735_WHITE;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(4*scale),posY,scale,scale,color);
      
      tft.fillRect(posX-(3*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX-(4*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX-(3*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(8*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(5*scale),posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(6*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(7*scale),posY+(7*scale),scale,scale,color);

      tft.fillRect(posX,posY+(8*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(8*scale),scale,scale,color);
      break;
    case 10: //Enemies lasers
      if(color!=ST7735_BLACK){
        color=ST7735_WHITE;
      }
      tft.fillRect(posX,posY,scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX,posY+(2*scale),scale,scale,color);

      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      break;
    case 11: //Human lasers
      if(color!=ST7735_BLACK){
        color=ST7735_YELLOW;
      }
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      break;
    case 12:  //Heart
      tft.fillRect(posX,posY,scale,scale,color);
      tft.fillRect(posX+(3*scale),posY,scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(4*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX+(3*scale),posY+(4*scale),scale,scale,color);

      tft.fillRect(posX+(1*scale),posY+(5*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(5*scale),scale,scale,color);
      break;
    case 13: //Up and down arrows
      tft.fillRect(posX,posY,scale,scale,color);
      
      tft.fillRect(posX-(1*scale),posY+(1*scale),scale,scale,color);
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      tft.fillRect(posX,posY+(5*scale),scale,scale,color);

      tft.fillRect(posX-(2*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX,posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(6*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(6*scale),scale,scale,color);

      tft.fillRect(posX-(1*scale),posY+(7*scale),scale,scale,color);
      tft.fillRect(posX,posY+(7*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(7*scale),scale,scale,color);

      tft.fillRect(posX,posY+(8*scale),scale,scale,color);

      
      break;
    case 14:  //right arrow
      tft.fillRect(posX,posY,scale,scale,color);
      
      tft.fillRect(posX,posY+(1*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(1*scale),scale,scale,color);

      tft.fillRect(posX-(3*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(2*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX-(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX,posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(2*scale),scale,scale,color);
      tft.fillRect(posX+(2*scale),posY+(2*scale),scale,scale,color);

      tft.fillRect(posX,posY+(3*scale),scale,scale,color);
      tft.fillRect(posX+(1*scale),posY+(3*scale),scale,scale,color);

      tft.fillRect(posX,posY+(4*scale),scale,scale,color);
      break;


  }
}

