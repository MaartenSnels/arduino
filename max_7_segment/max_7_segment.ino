#include <Time.h>
#include <TimeLib.h>

#include <DS3231RTC.h>
#include <Wire.h>
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 10 is connected to the CLK 
 pin 9 is connected to LOAD 
 We have only a single MAX72XX.
 */
 //LedControl(int dataPin, int clkPin, int csPin, int numDevices=1)
 
#define DATA     8
#define CLS     10
#define CS       9
#define DEVICES  1
#define HELDER   1
LedControl lc=LedControl(DATA,CLS,CS,DEVICES);

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,HELDER);
  /* and clear the display */
  lc.clearDisplay(0);

  randomSeed(analogRead(0));
  
  Serial.begin(9600);

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.clearDisplay(0);
  lc.setChar(0,7,'a',false);
  delay(delaytime);
  lc.setRow(0,6,0x05);
  delay(delaytime);
  lc.setChar(0,5,'d',false);
  delay(delaytime);
  lc.setRow(0,4,0x1c);
  delay(delaytime);
  lc.setRow(0,3,B00010000);
  delay(delaytime);
  lc.setRow(0,2,0x15);
  delay(delaytime);
  lc.setRow(0,1,0x1D);
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
}

void draai(char count) {
  for (char j = 0; j<count; j++) {
    for (char i = 0; i<8; i++) {
      lc.clearDisplay(0);
      lc.setColumn(0, i, 255);
      delay(50);
    }
  }
}

void lijntjes(boolean aan, int _delay, int seg) {
    for (int i=7; i>=0; i--) {
      lc.setLed(0, i, seg, aan);
      delay(_delay);
    }
  
}

void binair() {
  for (int i=0; i<=255; i++) {
    lc.setColumn(0, 1, i);
    delay(30);
  }
  lc.clearDisplay(0);
}

void eenVoorTwee(bool aan, int _delay) {
  for(int i = 0; i< 8; i++) {
    lijntjes(aan, _delay, i);
  }
  
}
void eenVoorEen(bool aan, int _delay) {
  for (int i=7; i >= 0; i--) {
    for (int j=7; j >= 0; j--) {
      lc.setLed(0, i, j, aan);
      delay(_delay);
    }
  }
}

void randomAanUit(int aantal){
  lc.clearDisplay(0);
  for (; aantal > 0; aantal--) {
    lc.setLed(0,random(8),random(8), random(2));
    delay(20);
  }
  lc.clearDisplay(0);
}

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void randomDigits() {
  for(int i=0;i<400;i++) {
    lc.setDigit(0, random(8), random(10), false);
    delay(10);
  }
  lc.clearDisplay(0);
  //delay(delaytime);
}

void tellen(int start, int stop) {
  for (int i = start; i<= stop; i++) {
    displayInt(i); 
  }
}

void displayInt(long getal) {
  lc.clearDisplay(0);
  // negatieve getallen max 7 lang
  int stop = 8;
  if (getal < 0) {
    stop = 7;
    lc.setChar(0, 7, '-', false);
  }
  for (int i = 0; i < stop; i++) {
    char cijfer = getal % 10;
    getal /= 10;
    lc.setDigit(0, i, cijfer, false);
    // laat maximaal 1 nul zien als het getal 0 is, anders overgebleven nullen negeren
    if (getal == 0) {
      return;
    }
  }
}

void bright() {
  lc.clearDisplay(0);
  
  for (int i = 0; i < 8; i++) {
    lc.setDigit(0,i,8,true);  
  }
  for (int i = 0; i < 16; i++) {
    lc.setIntensity(0,i);
    delay(delaytime);
  }
  for (int i = 15; i >= 0; i--) {
    lc.setIntensity(0,i);
    delay(delaytime);
  }
  lc.setIntensity(0,HELDER);
}

void tel(int _delay) {
  displayInt(0);
  delay(_delay);
  displayInt(10);
  delay(_delay);
  displayInt(210);
  delay(_delay);
  displayInt(3210);
  delay(_delay);
  displayInt(43210);
  delay(_delay);
  displayInt(543210);
  delay(_delay);
  displayInt(6543210);
  delay(_delay);
  displayInt(76543210);
  delay(_delay);

}

void help(int _delay){
  lc.clearDisplay(0);
  lc.setChar(0, 7, '-', false);
  lc.setChar(0, 6, '-', false);
  lc.setChar(0, 5, 'H', false);
  lc.setChar(0, 4, 'E', false);
  lc.setChar(0, 3, 'L', false);
  lc.setChar(0, 2, 'p', false);
  lc.setChar(0, 1, '-', false);
  lc.setChar(0, 0, '-', false);
  delay(_delay);
  lc.clearDisplay(0);
}


int oldSec = -1;
void loop() { 

    tmElements_t tm;
  

  if (timeStatus() == timeSet) {
    digitalClockDisplay();
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(4000);
  }
  delay(1000);
  

  
//  writeArduinoOn7Segment();
//  randomDigits();
//  tellen(100,300);
//  draai(10);
//  bright();
//  binair();
//  eenVoorTwee(true, 50);
//  eenVoorTwee(false, 50);
//  eenVoorEen(true, 50);
//  eenVoorEen(false, 50);
//  help(1000);
//  tel(300);
//  randomAanUit(400);
}


/*  
 *   111111111111
 *   6          2
 *   6          2
 *   6          2
 *   777777777777
 *   5          3
 *   5          3
 *   5          3
 *   444444444444  00
 *   
 *   
    7 =     lc.setRow(0,7,B00000001);
    6 =     lc.setRow(0,6,B00000010);
    5 =     lc.setRow(0,5,B00000100);
    4 =     lc.setRow(0,4,B00001000);
    3 =     lc.setRow(0,3,B00010000);
    2 =     lc.setRow(0,2,B00100000);
    1 =     lc.setRow(0,1,B01000000);
    0 =     lc.setRow(0,0,B10000000);
 */


void digitalClockDisplay(){
  // digital clock display of the time
  lc.setRow(0,7,B00001111); // t
  lc.setRow(0,6,B10000110);  //I.
  printTijdElement(5, hour());
  printTijdElement(3, minute());
  printTijdElement(1, second());
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printTijdElement(int start, int tijd){
  int t0 = tijd / 10;
  int t1 = tijd % 10;
  lc.setChar(0,start,     t0, false);
  lc.setChar(0,start - 1, t1, false);
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

