#include "Draw.h"           // Implemented libraries
#include "Accelerometer.h"  
#include <SPI.h>            // SD card libraries
#include <SD.h>

Draw draw;                    // Draw object. Used to access functions in Draw.h
Accelerometer accelerometer;  // Same as above, but for Accelerometer.h

typedef enum{                 // Current states of state machine
  MENU,         // Displays Main Menu
  NEW,          // Starts new game
  HS,           // Display High Score
  CREDITS,      // Display Credits
  PLAY,         // MAIN STATE when playing
  GO,           // Game Over Displays score and stats
  SNEW,         // Soft new game, starts new level without reseting the stats
  SDS,          // SD card start: reads scores from sd card file
  NHS,          // New high score: Starts user input for high score
  INTRO,        // Does intro
  
}game_States;

// SD card  and file reading variables
File myFile;            // Opened file
char bufferIn;          // Buffer used to read from file
char namesScores[5][13];// 5 Strings for each line in the HS.txt file
char names[15];         // 5 strings for separating names from scores
int scores[5];          // Array used to separate scores from names as INT

game_States cState;     // current State
unsigned int score;     // current game stats
char level, lifes;   
 
/*
 * These two arrays are needed in order to improve the speed of the game engine:
 *    new_terrain: Everything is written here BEFORE it gets updated on the display
 *    old_terrain: Everything that WAS updated in the display is here
 * 
 * In Order to avoid RAM consumption, it was decided to use arrays 2 by n. These ones keep X and Y coordinates in a lower space:
 *    invadersPos:        keeps X & Y coordinates for invaders AND type/color AND destroyed status
 *    invadersLasersPos:  keeps X & Y coordinates for invaders lasers
 *    humanLasersPos      keeps X & Y coordinates for human lasers
 *    prev_time:          used to move invaders down on the screen
 *    humanLasers_time:   used to limit the human spaceship firing rate
 *    lasers:             counts the number of lasers fired through the game
 *    shots:              counts the human lasers that HIT invaders or invaders missiles
 *    flag_moving:        used to stop movement of human spaceship once exploded
 *    huShipX:            human spaceship X coordinate ( Y is NOT needed)
 */
char new_terrain[160][128], old_terrain[160][128];
unsigned char invadersPos[50][4],invadersLasersPos[10][2],humanLasersPos[5][2];
unsigned int prev_time,humanLasers_time;
float lasers, shots;
char flag_moving;
char huShipX; 
int i,j,k;      // For counters/For cycles

/*  
 *  Two modes:
 *    Hard reset: Resets stats, level, shots, score, lasers, lifes AND terrain, invaders, human lasers, invaders, lasers, ship pos, timers, flags
 *    Soft reset: Resets terrain, invaders, human lasers, invaders, lasers, ship pos
 */
void resetGame(boolean hardreset){
  if(hardreset){
    level=1;  
    lasers=shots=score=0;
    lifes=6;
  }
  
  // If leveled up, add an extra life
  if(lifes<6){
    lifes++;
  }
  
  flag_moving=prev_time=humanLasers_time=0;
  
  for(i=0;i<160;i++){       // Clear terrain
    for(j=0;j<128;j++){
      new_terrain[i][j]=0;
      old_terrain[i][j]=0;
    }
  }
  for(i=0;i<50;i++){        // Clear invaders
    for(int j=0;j<4;j++){
      invadersPos[i][j]=0;
      //invadersPos[i][3]=1;
    }
  }
  for(i=0;i<10;i++){        // Clear invaders lasers
    invadersLasersPos[i][0]=invadersLasersPos[i][1]=invadersLasersPos[i][2]=0;
  }

  for(i=0;i<5;i++){         // Clear human lasers
    humanLasersPos[i][0]=humanLasersPos[i][1]=humanLasersPos[i][2]=0;
  }
  huShipX=80;       // Reset human spaceship start position
}

/*
 * Reads accelerometer value and updates position of the human spaceship:
 *    If tilt left, huShipX=huShipX-6
 *    If tilt right, huShipX=huShipX+6
 *    If no tilt, huShipX=huShipX
 * Writes new position to NEW map. NOTICE: new_terrain HAS NOT  been rendered YET on the screen
 */
void moveSpaceship(){
  int temp;
  if(accelerometer.getRoll()<-10 &&  (huShipX+6)<155 && flag_moving==0){    // Move Right
    huShipX+=5;
    new_terrain[huShipX++][119]=7;        // 7: Is the identifier for the human spaceship. INFO check  object() at Draw.h/Draw.cpp
  }else if(accelerometer.getRoll()>=10 && (huShipX-6)>5 && flag_moving==0){ //Move Left
    huShipX-=5;
    new_terrain[huShipX--][119]=7;
  }else{                                  // Same pos
    new_terrain[huShipX][119]=7;
  }
}
/*
 * Creates 5 rows of 10 enemies each. Difficulty only affects the type of invader generated
 */
void createEnemies(int difficulty){
  char type;
  
  j=16;                   // Start position on X for first invader
  if(difficulty==2){      // Invader type selection
    type=random(1,3);  
  }else if(difficulty==3){
    type=random(1,5);
  }else if(difficulty==4){
    type=random(1,7);
  }else{
    type=1;
  }
  
  for(i=0;i<10;i++){
    if(difficulty>4){
      type=random(1,7);
    }
    invadersPos[i][0]=j;    // X Pos
    invadersPos[i][1]=20;   // Y Pos
    invadersPos[i][2]=type; // Type and color
    invadersPos[i][3]=0;    // Destroyed: 0 is fine, 1 is destroyed
    j+=14;                  //Separation on X axis
  }
  
  if(difficulty==2){
    type=random(3,5);  
  }else if(difficulty==3){
    type=random(3,7);
  }else if(difficulty==4){
    type=random(1,7);
  }else{
    type=3;
  }
  
  j=14;
  for(i=10;i<20;i++){
    if(difficulty>4){
      type=random(1,7);
    }
    invadersPos[i][0]=j;
    invadersPos[i][1]=30;
    invadersPos[i][2]=type;
    invadersPos[i][3]=0;
    j+=14;
  }

  if(difficulty==2){
    type=random(3,5);  
  }else if(difficulty==3){
    type=random(3,7);
  }else if(difficulty==4){
    type=random(1,7);
  }else{
    type=4;
  }

  j=14;
  for(i=20;i<30;i++){
    if(difficulty>4){
      type=random(1,7);
    }
    invadersPos[i][0]=j;
    invadersPos[i][1]=40;
    invadersPos[i][2]=type;
    invadersPos[i][3]=0;
    j+=14;
  }

  if(difficulty==2){
    type=random(5,7);  
  }else if(difficulty==3){
    type=random(3,7);
  }else if(difficulty==4){
    type=random(1,7);
  }else{
    type=5;
  }
  
  j=15;
  for(i=30;i<40;i++){
    if(difficulty>4){
      type=random(1,7);
    }
    invadersPos[i][0]=j;
    invadersPos[i][1]=50;
    invadersPos[i][2]=type;
    invadersPos[i][3]=0;
    j+=14;
  }
  
  if(difficulty==2){
    type=random(5,7);  
  }else if(difficulty==3){
    type=random(3,7);
  }else if(difficulty==4){
    type=random(1,7);
  }else{
    type=6;
  }

  j=15;
  for(i=40;i<50;i++){
    if(difficulty>4){
      type=random(1,7);
    }
    invadersPos[i][0]=j;
    invadersPos[i][1]=60;
    invadersPos[i][2]=type;
    invadersPos[i][3]=0;
    j+=14;
  }
}

/*
 * Moves invaders down on the screen according to difficulty. Eg: increased difficulty, increased speed
 */
void moveInvaders(float difficulty){
  if(millis()-prev_time>(9000.0/(1+(difficulty/10)))){
    for(i=0;i<50;i++){
      invadersPos[i][1]+=5;   // Sum 5 on y axis coordinates
    }
    prev_time=millis();
  }
  for(i=0;i<50;i++){              // Updates the new Y coordinate on all invaders except destroyed ones
    if(invadersPos[i][3]!=1){     // destroy invaders by not updating position
       new_terrain[invadersPos[i][0]][invadersPos[i][1]]=invadersPos[i][2];
    }
  }
}
/*
 * Creates invaders lasers according to difficulty. Eg: more difficulty, more lasers are fired
 * Algorithm searches for the 10 closest invaders to the bottom. Generates lasers randomly at that position
 * It also lowers the firing rate accordingly to the alive invaders on map. Eg: 1 invader on map lowers firing to almost 0.
 * This avoids making the level near impossible. Otherwise, last invader would fire constanlty 
 */
void createInvadersLasers(float difficulty){
  char alive=0;
  char ten;
  char selected[10];
  char temp=0;
  for(i=0;i<50;i++){          // Check and count invaders alive on map
    if(invadersPos[i][3]==0){
      alive++;
    }
  }
  
  ten=(alive*10)/50;    // lower firing rate 
  
  for(i=49;i>=0 && temp!=ten;i--){     // Searches for 10 invaders closest to bottom (alive)
    if(invadersPos[i][3]!=1){
      selected[temp]=i;
      temp++;
    }else{
      selected[temp]=60;              // 60 is selected randomly to avoid firing from dead invaders
    }
  }
  
  for(i=0;i<ten;i++){
    unsigned int prob=random(400);
    if(prob<(100.0-(100.0/(1+(difficulty/10.0))))){ //If probability decides to fire
      if(invadersLasersPos[i][0]==0 && invadersLasersPos[i][1]==0){ // If laser not created already
        if(selected[temp]!=60){     // If not dead
          invadersLasersPos[i][0]=invadersPos[selected[i]][0];  // CReate laser at invaders XY coordinates
          invadersLasersPos[i][1]=invadersPos[selected[i]][1];
        }
      }
    }
  }
}

/*
 * If accelerometer at position, fires human laser if not fired already
 * Limit for human lasers is 5 at a time, with a time limit of 300 ms for each
 */
void createHumanLasers(){
  if(accelerometer.getPitch()>-30 && (millis()-humanLasers_time)>300){  // If accelerometer in position and time allows it, fire
    i=0;
    while(humanLasersPos[i][0]!=0 && humanLasersPos[i][1]!=0){    //checks for NON fired lasers
      i++;
    }
    if(i<5){                              // Generate laser at current spaceship POS X
      humanLasersPos[i][0]= huShipX;
      humanLasersPos[i][1]= 116;
      lasers++;                          // Increment lasers fired counter
    }
    humanLasers_time=millis();
  }
}

// Self explanatory
void createLasers(float difficulty){
  createInvadersLasers(difficulty);
  createHumanLasers();
}

/*
 * Moves invaders lasers on Y axis 5 pixels at a time.
 * Deletes them once they went out the map
 */
void moveInvadersLasers(float difficulty){
  for(i=0;i<10;i++){  
    if(invadersLasersPos[i][0]!=0 && invadersLasersPos[i][1]!=0){ //If lasers exist, move them
      if(invadersLasersPos[i][1]+5<130){    // If laser is not out of map 
        invadersLasersPos[i][1]+=5;
        new_terrain[invadersLasersPos[i][0]][invadersLasersPos[i][1]]=10; //draw them
      }else{
        invadersLasersPos[i][0]=0;    // Else delete laser
        invadersLasersPos[i][1]=0;
      }
    }
  }
}
/*
 * Moves created human lasers 5 pixels at a time
 * Deletes them once they are at the top of screen
 */
void moveHumanLasers(){
  for(i=0;i<5;i++){
    if(humanLasersPos[i][0]!=0 && humanLasersPos[i][1]!=0){
      if(humanLasersPos[i][1]-5>15){
        humanLasersPos[i][1]-=5;  
        new_terrain[humanLasersPos[i][0]][humanLasersPos[i][1]]=11; //Draw laser
      }else{
        humanLasersPos[i][0]=0;   //Delete lasers
        humanLasersPos[i][1]=0;
      }
    }
  }
}
// Self explanatory 
void moveLasers(float difficulty){
  moveInvadersLasers(difficulty);
  moveHumanLasers();
}
/*
 * Validates invaders lasers hitting human spaceship.
 * Checks if laser exists, then laser's X coordinate and compares with ship.
 * Checks Y coordinates and if hit, substracts 1 life
 */
void validateInvadersLasers(){
  for(i=0;i<10;i++){
    if(invadersLasersPos[i][0]!=0 && invadersLasersPos[i][1]!=0){     // if laser exists
      if(invadersLasersPos[i][0]>huShipX-6 && invadersLasersPos[i][0]<huShipX+6){   //if laser is in HSpaceship X range
        if(invadersLasersPos[i][1]+5 >= 119){ // If lasers reach Spaceship Y coordinates
          if(lifes>0){
            lifes--;          // substract to lifes       
          }
          // Destroy laser 
          invadersLasersPos[i][0]=0;
          invadersLasersPos[i][1]=0;
        }
      }
    }
  }
}
/*
 * Validates human lasers with invaders position and invaders lasers
 * If true, destroys invader/laser and draws an explosion
 */
void validateHumanLasers(){
  for(i=0;i<5;i++){     // For each of the human lasers
    for(j=49;j>=0;j--){     // Validate with all invaders
      if(invadersPos[j][3]!=1){ // If invader is alive
        if((invadersPos[j][0]-humanLasersPos[i][0]<=3 && invadersPos[j][0]-humanLasersPos[i][0]>=0) || (humanLasersPos[i][0]-invadersPos[j][0]<7 &&humanLasersPos[i][0]-invadersPos[j][0]>0)){ //If hit on X
          if(humanLasersPos[i][1]-invadersPos[j][1]<6 &&humanLasersPos[i][1]-invadersPos[j][1]>=0){     // If hit on Y
            //Destroy invader & laser    
            invadersPos[j][3]=1;
            humanLasersPos[i][0]=0;
            humanLasersPos[i][1]=0;
            new_terrain[invadersPos[j][0]+1][invadersPos[j][1]+1]=9;  //Create explosion at coordinates
            shots++;            // Increase shots counter
            switch(invadersPos[j][2]){  // increases score according to invader type
              case 1:
                score+=3;
                break;
              case 2:
                score+=3;
                break;
              case 3:
                score+=2;
                break;
              case 4:
                score+=2;
                break;
              case 5:
                score+=1;
                break;
              case 6:
                score+=1;
                break;
            }
          }
        }
      }
    }
    /*
     * Same logic as above. Validates collisions between lasers
     */
    for(j=0;j<10;j++){
      if(invadersLasersPos[j][0]!= 0 && invadersLasersPos[j][1]!=0 ){
        if(abs(invadersLasersPos[j][0]-humanLasersPos[i][0])<5){
          if(humanLasersPos[i][1]-invadersLasersPos[j][1]<=6 && humanLasersPos[i][1]-invadersLasersPos[j][1]>=0){
              shots++;
              new_terrain[invadersLasersPos[j][0]][invadersLasersPos[j][1]+2]=9;  //Create explosion of lasers collision
              invadersLasersPos[j][0]=0;    //Delete invaders lasers
              invadersLasersPos[j][1]=0;
              humanLasersPos[i][0]=0;       //Delete human laser
              humanLasersPos[i][1]=0;
          }
        }
      }
    }
  }
}

//Self explanatory
void validateLasers(){
  validateInvadersLasers();
  validateHumanLasers();
}
// Initializes SD card comms
void initSD(){
    Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}
// Reads HS.txt file and saves text
void readHS(){
  // re-open the file for reading:
  myFile = SD.open("HS.txt");
  if (myFile) {
    // read from the file until there's nothing else in it:
    j=i=0;
    while (myFile.available()) {
      bufferIn=myFile.read();
      if( bufferIn=='\r' ){
        myFile.read();
        j++;
        i=0;
        namesScores[j][i]=myFile.read();
      }else{
        namesScores[j][i]=bufferIn;
      }
      i++;
    }
    // close the file:
    myFile.close();
    // Separate names from scores
    getNames();   
    getScores();
    Serial.println("Successfully read file tho");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
/*
 * Converts char to INT, and saves them in array
 */
void getScores(){
  scores[0]=getInt(namesScores[0]);
  scores[1]=getInt(namesScores[1]);
  scores[2]=getInt(namesScores[2]);
  scores[3]=getInt(namesScores[3]);
  scores[4]=getInt(namesScores[4]);
}
/*
 * Stores names in a 1d array for using with draw library
 */
void getNames(){
  names[0]=namesScores[0][0];
  names[1]=namesScores[0][1];
  names[2]=namesScores[0][2];
  names[3]=namesScores[1][0];
  names[4]=namesScores[1][1];
  names[5]=namesScores[1][2];
  names[6]=namesScores[2][0];
  names[7]=namesScores[2][1];
  names[8]=namesScores[2][2];
  names[9]=namesScores[3][0];
  names[10]=namesScores[3][1];
  names[11]=namesScores[3][2];
  names[12]=namesScores[4][0];
  names[13]=namesScores[4][1];
  names[14]=namesScores[4][2];
}

/*
 * Returns converted Int number from char
 */
int getInt(char numbs[13]){
  int result=0;
  for(i=4;numbs[i]!='\0';i++){
  }
  i--;
  for(j=0,i;i>=4;i--,j++){
    result+=(numbs[i]-48)*pow(10,j);
  }
  return result;
}
// Returns the position of the current score compared to already stored
int getOrder(int score){
  readHS();
  for(i=0;i<5 && scores[i]> score;i++){
  }
  return i;
}
/*
 * Function gets input from the user in order to save his/her name
 * Gets three upper case letters to save
 */
void getName(int posi){
  char chars[]="AAA";
  char namesOrder[5][3];
  draw.stars();
  i=0;
  while(i!=3){
    if(accelerometer.getPitch()>-10 && chars[i]>65){ // Letters UP
      chars[i]--;
     }else if(accelerometer.getPitch()<-35 && chars[i]< 90){ // Letters DOWN
      chars[i]++;
    }

    if(accelerometer.getRoll()>10 && i>0){ // Select left
      i--;
 
    }else if(accelerometer.getRoll()<-10 && i<3 && accelerometer.getRoll()>-20 & i!=2){ // Select Right
      i++;
    }else if(i==2 && accelerometer.getRoll()<-30 && i<3){ // Done editing
      i++;  
    }
    draw.updateNames(i,chars);  //Redraws letters
    delay(300);
  }

  //Reconfigures scores according to new score and orders them
  switch(posi){
    case 0:
      namesOrder[4][0]=names[9];
      namesOrder[4][1]=names[10];
      namesOrder[4][2]=names[11];

      namesOrder[3][0]=names[6];
      namesOrder[3][1]=names[7];
      namesOrder[3][2]=names[8];

      namesOrder[2][0]=names[3];
      namesOrder[2][1]=names[4];
      namesOrder[2][2]=names[5];

      namesOrder[1][0]=names[0];
      namesOrder[1][1]=names[1];
      namesOrder[1][2]=names[2];

      namesOrder[0][0]=chars[0];
      namesOrder[0][1]=chars[1];
      namesOrder[0][2]=chars[2];

      scores[4]=scores[3];
      scores[3]=scores[2];
      scores[2]=scores[1];
      scores[1]=scores[0];
      scores[0]=score;

      break;
    case 1:
      namesOrder[4][0]=names[9];
      namesOrder[4][1]=names[10];
      namesOrder[4][2]=names[11];

      namesOrder[3][0]=names[6];
      namesOrder[3][1]=names[7];
      namesOrder[3][2]=names[8];

      namesOrder[2][0]=names[3];
      namesOrder[2][1]=names[4];
      namesOrder[2][2]=names[5];

      namesOrder[1][0]=chars[0];
      namesOrder[1][1]=chars[1];
      namesOrder[1][2]=chars[2];

      namesOrder[0][0]=names[0];
      namesOrder[0][1]=names[1];
      namesOrder[0][2]=names[2];

      scores[4]=scores[3];
      scores[3]=scores[2];
      scores[2]=scores[1];
      scores[1]=score;
      
      break;
    case 2:
      namesOrder[4][0]=names[9];
      namesOrder[4][1]=names[10];
      namesOrder[4][2]=names[11];

      namesOrder[3][0]=names[6];
      namesOrder[3][1]=names[7];
      namesOrder[3][2]=names[8];

      namesOrder[2][0]=chars[0];
      namesOrder[2][1]=chars[1];
      namesOrder[2][2]=chars[2];

      namesOrder[1][0]=names[3];
      namesOrder[1][1]=names[4];
      namesOrder[1][2]=names[5];

      namesOrder[0][0]=names[0];
      namesOrder[0][1]=names[1];
      namesOrder[0][2]=names[2];
      
      scores[4]=scores[3];
      scores[3]=scores[2];
      scores[2]=score;
      
      break;
    case 3:
      namesOrder[4][0]=names[9];
      namesOrder[4][1]=names[10];
      namesOrder[4][2]=names[11];

      namesOrder[3][0]=chars[0];
      namesOrder[3][1]=chars[1];
      namesOrder[3][2]=chars[2];

      namesOrder[2][0]=names[6];
      namesOrder[2][1]=names[7];
      namesOrder[2][2]=names[8];

      namesOrder[1][0]=names[3];
      namesOrder[1][1]=names[4];
      namesOrder[1][2]=names[5];

      namesOrder[0][0]=names[0];
      namesOrder[0][1]=names[1];
      namesOrder[0][2]=names[2];

      scores[4]=scores[3];
      scores[3]=score;
     
      break;
    case 4:
      namesOrder[4][0]=chars[0];
      namesOrder[4][1]=chars[1];
      namesOrder[4][2]=chars[2];

      namesOrder[3][0]=names[9];
      namesOrder[3][1]=names[10];
      namesOrder[3][2]=names[11];

      namesOrder[2][0]=names[6];
      namesOrder[2][1]=names[7];
      namesOrder[2][2]=names[8];

      namesOrder[1][0]=names[3];
      namesOrder[1][1]=names[4];
      namesOrder[1][2]=names[5];

      namesOrder[0][0]=names[0];
      namesOrder[0][1]=names[1];
      namesOrder[0][2]=names[2];
      
      scores[4]=score;
     
      break;
    
  }
    //Writes new ordered scores to txt file
    SD.remove("HS.txt");
    
    myFile=SD.open("HS.txt",FILE_WRITE);
    if(myFile){
      myFile.print(namesOrder[0][0]);
      myFile.print(namesOrder[0][1]);
      myFile.print(namesOrder[0][2]);
      myFile.print(",");
      myFile.println(scores[0]);

      myFile.print(namesOrder[1][0]);
      myFile.print(namesOrder[1][1]);
      myFile.print(namesOrder[1][2]);
      myFile.print(",");
      myFile.println(scores[1]);

      myFile.print(namesOrder[2][0]);
      myFile.print(namesOrder[2][1]);
      myFile.print(namesOrder[2][2]);
      myFile.print(",");
      myFile.println(scores[2]);

      myFile.print(namesOrder[3][0]);
      myFile.print(namesOrder[3][1]);
      myFile.print(namesOrder[3][2]);
      myFile.print(",");
      myFile.println(scores[3]);

      myFile.print(namesOrder[4][0]);
      myFile.print(namesOrder[4][1]);
      myFile.print(namesOrder[4][2]);
      myFile.print(",");
      myFile.println(scores[4]);

      myFile.close();
    }
    cState=MENU;
}
/*
 * Checks game stats, updates score, substracts lifes
 * If all invaders on screen are killed: next level
 * If invader reaches bottom of map: game over
 */
void updateStats(){
  char alive=0;
  for(i=0;i<50;i++){
    if(invadersPos[i][3]==0){
      alive++;
      if(invadersPos[i][1]>111){  // If invader reaches bottom
        cState=GO;
      }
    }
  }
  if(alive==0){     // If level cleared, generate new level
    level++;
    cState=SNEW;
  }
  
  draw.updateScore(score);
  
  switch(lifes){
    case 0:
      flag_moving=1;  //Stop movement on spaceship
      draw.object(7,huShipX,119,ST7735_BLACK,1);//Delete spaceship
      draw.object(9,huShipX-4,115,ST7735_WHITE,2);//Create explosion
      draw.object(12,139,1,ST7735_BLACK,1); //DElete last heart
      cState=GO;
      break;
    case 1:
      draw.object(12,146,1,ST7735_BLACK,1);
      break;
    case 2:
      draw.object(12,153,1,ST7735_BLACK,1);
      break;
    case 3:
      draw.object(12,139,8,ST7735_BLACK,1);
      break;
    case 4:
      draw.object(12,146,8,ST7735_BLACK,1);
      break;
    case 5:
      draw.object(12,153,8,ST7735_BLACK,1);
      break;
  }
}
/*
 * In order to reduce rendering lag, it was decided to only update parts of the screen that currently have drawings on it.
 * Therefore we WILL never use tft.fillScreen() on the state PLAY
 * In order to optimize speed, two arrays are needed. One with all the previous positions of objects CURRENTLY on the Screen
 * And another array with the future positions.
 * 
 * Delete Logic: 
 *    Function searches the old_terrain array until not 0. 
 *    If not 0, replaces object with same object in color BLACK.
 *    Deletes the object from old_terrain by asigning a 0
 * Draw Logic: 
 *    Same as Delete, but searches in new_terrain. And draws with COLOR.
 *    After Drawing, old_terrain=new_terrain
 *    And deletes entry on new_terrain
 */
void updateTerrain(){
  for(i=0;i<160;i++){
    for(j=0;j<128;j++){
      
      if(new_terrain[i][j] != 0){
        draw.object(new_terrain[i][j],i,j,ST7735_CYAN,1);
        old_terrain[i][j]=new_terrain[i][j];
        new_terrain[i][j]=0;
      }
      else if(old_terrain[i][j]!= 0 ){
        draw.object(old_terrain[i][j],i,j,ST7735_BLACK,1);
        old_terrain[i][j]=0;
      }
    }
  }
}

void setup(void) {
  Serial.begin(9600);     // For debugging prupuses
  initSD();
  
  draw.setUp();           // Configures the Display, SET ROTATION HERE
  draw.doIntro();         // Nifty little animation. If device shaked, next STATE=MENU
  cState=MENU; 
}
/*
 * Game FSM machine is defined here.
 */
void loop(void) {       
  switch(cState){
    case MENU:
      switch(draw.doMenu()){
        case 1:
          cState=NEW;     //Selected new game
          break;
        case 2:
          cState=SDS;      //Selected High Scores
          break;
        case 3:
          cState=CREDITS; //Selected Credits
          break;
      }
      break;
      
    case NEW:           
      resetGame(true);      // Hard reset
      draw.newGame(level,lifes,score);
      createEnemies(level);
      cState=PLAY;
      break;
    case SNEW:          // Soft reset
      resetGame(false);
      draw.newGame(level,lifes,score);
      createEnemies(level);
      cState=PLAY;
      break;
    case PLAY:
      createLasers(level);  //Creates invaders lasers if not created yet
      moveLasers(level);  // Moves created lasers
      validateLasers();         // Validates collisions of lasers with ship
      moveInvaders(level);    //Moves invaders
      moveSpaceship(); //moves AND UPDATES spaceship on screen
      updateTerrain(); //Done after EVERYthing ELSE
      updateStats();    //Updates score, and lifes 
      break;
    case SDS:
      readHS();   //Read info from file
      cState=HS;
      break;
    case HS:  // Display High scores
      getScores();
      getNames();
      draw.highScores(scores,names);
      cState=MENU;
      break;
    case GO:    // Game over checks if score is high score
      readHS();
      getScores();
      if(getOrder(score)==5){
        draw.gameOver(score,shots,lasers);
        cState=MENU;      // NOT high score
      }else{
        draw.gameOver(score,shots,lasers);
        cState=NHS;     //yes highscore
      }
      break;
    case NHS:
      getName(getOrder(score)); // Gets user input and saves score
      break;
    case CREDITS:   //Displays credits
      draw.credits();
      cState=INTRO;
      break;
    case INTRO:   //Does intro
      draw.doIntro();
      cState=MENU;
      break;
    default:
      break;
  }
}
