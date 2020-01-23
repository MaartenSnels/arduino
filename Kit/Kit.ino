#include <DigiPixel.h>
#include <avr/pgmspace.h>


// leave the following line uncommented for use with a Digispark
//DigiPixel digiPixel(3,0,5,2,1);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

// leave the following line uncommented for use with an Arduino
DigiPixel digiPixel(5,2,6,4,3);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

const byte KIT = 1;
const byte RANDOM = 2;
const byte KITRANDOM = 3;

// Variables
byte aantalUit = 1;
byte mode = KIT;
byte ScanDelay = 20;
byte ThisPixel = 0;
byte color = 1;
byte row = 0;
boolean GoingRight = true;
boolean modeKeysPressed = false;
boolean randomButtonPressed=false;

void setup(){
}


void loop(){
 switch (mode) {
   case KIT:
     kitMode(false);
     break;
   case KITRANDOM:
     kitMode(true);
     break;
   case RANDOM:
     randomMode();
     break;
 }
  checkModeKeys(); 
}

void checkModeKeys() {
  digiPixel.saveButtonStates();

  if (digiPixel.buttonAPressed && digiPixel.buttonLeftPressed && !modeKeysPressed ) {
    changeMode();
    modeKeysPressed = true;
  } else {
    modeKeysPressed = false;
  }

}

void kitMode(boolean randomColor) {
  int colorLed = color;
  if (randomColor) {
    colorLed = 1 + random(7);
  }
  digiPixel.setPixel(ThisPixel, row, colorLed);
  digiPixel.drawScreen();            
  digiPixel.setPixel(ThisPixel, row, 0);
  MovePixel();
  delay(ScanDelay);
  checkDelayButtonsPressed();
}

void randomMode() {
  if (random(aantalUit)==1) {
    digiPixel.setPixel(random(8), random(8), random(8));
  }
  else {
    digiPixel.setPixel(random(8), random(8), 0);
  }
  digiPixel.drawScreen();
  checkButtonsRandom();  
}

void changeMode() {
  digiPixel.clearScreen();
  if (mode==KIT) 
    mode = KITRANDOM;
  else if (mode==KITRANDOM) 
    mode = RANDOM;
  else
    mode = KIT;
}

void checkDelayButtonsPressed() {
  digiPixel.saveButtonStates();

  if (digiPixel.buttonLeftPressed) {
    ScanDelay--;
    if (ScanDelay == 255) {
      ScanDelay=0;
    }
  }
  if (digiPixel.buttonRightPressed) {
    ScanDelay++;
    if (ScanDelay == 0) {
      ScanDelay=255;
    }
  }
}

void checkButtons() {
 switch (mode) {
   case KIT:
     checkButtonsKit();
     break;
   case RANDOM:
     checkButtonsRandom();
     break;
 }
}
  
void checkButtonsKit() {
  digiPixel.saveButtonStates();
  if (digiPixel.buttonAPressed) {
        color++;
        if (color > 7){
          color = 1;
        }
  }
  if (digiPixel.buttonBPressed) {
        color--;
        if (color == 0){
          color = 7;
        }
  }
  if (digiPixel.buttonUpPressed) {
        row++;
        if (row > 7){
          row = 7;
        }
  }
  if (digiPixel.buttonDownPressed) {
        row--;
        if (row == 255){
          row = 0;
        }
  }
}

void checkButtonsRandom() {
  digiPixel.saveButtonStates();
  if(!randomButtonPressed) {
    if (digiPixel.buttonDownPressed) {
          aantalUit++;
          if (aantalUit > 7){
            aantalUit = 7;
          }
    }
    if (digiPixel.buttonUpPressed) {
          aantalUit--;
          if (aantalUit < 1){
            aantalUit = 1;
          }
    }
    randomButtonPressed=true;
  } else {
    randomButtonPressed=false;
  }
}

void MovePixel(){
    if(GoingRight == true){
      ThisPixel++;
      if (ThisPixel == 8){
        ThisPixel = 6;
        GoingRight = false;
        checkButtonsKit();
      }
    }
    else{
      ThisPixel--;
      if (ThisPixel == 255){
        ThisPixel = 1;
        GoingRight = true;
        checkButtonsKit();
      }
    }
}



