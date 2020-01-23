#include <Keypad.h>

#include <DS3231RTC.h>
#include <PololuLedStrip.h>

#include <Wire.h>
#include <Time.h>
#include "Deklok.h"
#include "max7219.h"


// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<13> strip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 60
rgb_color colors[LED_COUNT];

// const byte secButtonPin = 7;
// const byte setUurButtonPin = 11;
// const byte setMinButtonPin = 12;
// const byte setSecButtonPin = 10;

// set keypad
const byte rows = 4; //four rows
const byte cols = 4; //four columns
char keys[rows][cols] = {
  {'D','C','B', 'A'},
  {'#','9','6', '3'},
  {'0','8','5', '2'},
  {'*','7','4', '1'}
};
// aansluiting van matrix van rechts -> links (meest rechts -> 2, meest links -> 9)
byte rowPins[rows] = {5, 6,  7,  8 }; //connect to the row pinouts of the keypad
byte colPins[cols] = {9, 10, 11, 12}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup() {
//  Serial.begin(9600);

//  setTijd(21,50,00);
  
  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);

//  digitalWrite(13, HIGH);  

//initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
  for (int e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x00 );    // the first 0x0f is the value you can set
                                                 // range: 0x00 to 0x0f
                                                 
//  for(int i = 0; i< LED_COUNT;  i++) {
//    colors[i] = (rgb_color) {100,100,100};
//    strip.write(colors,LED_COUNT);
//    colors[i] = (rgb_color) {0,0,0};
//  }
//  strip.write(colors,LED_COUNT);
}

void tekenPatroon(byte matrix,  byte patroon[]) {
  for (int i = 1; i < 9 ; i++ ) {
    maxOne(matrix, i, patroon[i-1]);
  }
}

byte LEEG[8]    = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte VOL[8]     = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
byte GRADEN[8]  = {0x1E,0x12,0x12,0x1E,0x00,0x00,0x00,0x00};
byte CELCIUS[8] = {0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF};
byte CIJFERS[10][8] = {
  {0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF}, // 0
  {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03}, // 1
  {0xFF,0x03,0x03,0xFF,0xFF,0xC0,0xC0,0xFF}, // 2
  {0xFF,0x03,0x03,0xFF,0xFF,0x03,0x03,0xFF}, // 3
  {0xC3,0xC3,0xC3,0xFF,0xFF,0x03,0x03,0x03}, // 4
  {0xFF,0xC0,0xC0,0xFF,0xFF,0x03,0x03,0xFF}, // 5
  {0xFF,0xC0,0xC0,0xFF,0xFF,0xC3,0xC3,0xFF}, // 6
  {0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03}, // 7
  {0xFF,0xC3,0xC3,0xFF,0xFF,0xC3,0xC3,0xFF}, // 8
  {0xFF,0xC3,0xC3,0xFF,0xFF,0x03,0x03,0xFF}  // 9
};

void adjustTime() {
  
  // loop over de keys, als 4 gehad dan stop
  int cijfer = 1;
  char key = keypad.getKey();
  int uur, minuut, seconde;
  uur = 0;
  minuut = 0;
  seconde =0;
  while( cijfer <= 4 && key != '#') {
    if (key != NO_KEY) {
      // afhankelijk van het veld is er een tijd mogelijk
      // 1-ste mag alleen 0, 1 of 2
      // 2-de bij eerste 0 of 1 1-9
      //      bij eerste 2 0-3
      //3-de 0-5
      //4-de 0-9
      if (cijfer == 4 || cijfer == 2 && uur < 20 ) {
        switch(key) {
          case '0' ... '9':
            if (cijfer == 2) {
              uur = 10 + (key - '0');
            }
            else {
              minuut = minuut + (key - '0');
            }
            cijfer++;
            break;
        }
      }
      else if (cijfer == 2) {
        switch(key) {
          case '0' ... '3':
              uur = uur + (key - '0');
              cijfer++;
              break;
        }
      }
      else if (cijfer == 1) {
        switch(key) {
          case '0' ... '2':
              uur = (key - '0')*10;
              cijfer++;
              break;
        }
      }
      else if (cijfer == 3) {
        switch(key) {
          case '0' ... '5':
              minuut = (key - '0')*10;
              cijfer++;
              break;
        }
      }

    }
    // vul de boel
    tekenPatroon(1,CIJFERS[uur / 10]);
    tekenPatroon(2,CIJFERS[uur % 10]);
    tekenPatroon(3,CIJFERS[minuut / 10]);
    tekenPatroon(4,CIJFERS[minuut % 10]);
    char key = keypad.getKey();
  }
  // zet tijd
  
  if (key != '#' && cijfer == 5) {
    setTijd(uur, minuut, seconde);
  }
}

void setTijd(int uur, int minuut, int seconde) {
    tmElements_t tm;
    tm.Second = seconde;
    tm.Minute = minuut;
    tm.Hour   = uur;
  
    RTC.write(tm);
}

void temperatuur() {
  float temp = RTC.getTemp();
//  Serial.println(temp);
  tekenPatroon(1,CIJFERS[(byte)temp / 10]);
  tekenPatroon(2,CIJFERS[(byte)temp % 10]);
  tekenPatroon(3,GRADEN);
  tekenPatroon(4,CELCIUS);
}

#define TICK 15
void setTicks() {
  rgb_color color = (rgb_color){10,10,10};
  for (int i=0;i<LED_COUNT; i+=TICK) {
    colors[i] = color;
  }
}


#define OFF (rgb_color) {0,0,0}
void tekenSeconde(char seconde) {
  int r = 0,g = 1,b = 0;
  while (r + g + b == 0) {
    r = random(2);
    g = random(2);
    b = random(2);
  }
  setTicks();
  rgb_color color = (rgb_color){100 * r,100 * g,100 * b};
  colors[seconde] = color;
  strip.write(colors, LED_COUNT);
  colors[seconde] = OFF;
  
}



char oldSec = 0;
char oldMin = 0;
bool writeTime = false;

#define TEMP_START_SEC 10    // seconde waarop de temperatuur moet worden weergegeven
#define TEMP_STOP_SEC  15    // seconde waarop de normale tijd weer moet worden weergegeven

void loop() {
  tmElements_t tm;
  
//  if ( // digitalRead(setSecButtonPin) == LOW ||
//       digitalRead(setMinButtonPin) == LOW ||
//       digitalRead(setUurButtonPin) == LOW ) {
//
//      adjustTime();
//  }
  if (keypad.getKey() == '#'){
    adjustTime();
  }
  
  
  RTC.read(tm); 
  char uur = tm.Hour;
  char minuut = tm.Minute;
  char seconde = tm.Second;
  
//  Serial.print(uur);
//  Serial.print(":");
//  Serial.print(minuut);
//  Serial.print(":");
//  Serial.println(seconde);
  
  if (oldSec != seconde) 
  {
    oldSec = seconde; 
    if (seconde >= TEMP_START_SEC && seconde < TEMP_STOP_SEC) {
      // schrijf de temperatuur, maar doe het maar ff. en daarna weer de
      // tijd. Herhaal het echet niet te vaak.
    }
    // tekenSeconde(seconde);
    if (oldMin != minuut || writeTime) {
      oldMin = minuut;
      writeTime = false;
      tekenPatroon(1,CIJFERS[uur / 10]);
      tekenPatroon(2,CIJFERS[uur % 10]);
      tekenPatroon(3,CIJFERS[minuut / 10]);
      tekenPatroon(4,CIJFERS[minuut % 10]);
    }
  }
  
}
  



