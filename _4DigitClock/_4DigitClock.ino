#include <EEPROM.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS3231RTC.h>

#include <Wire.h>
#include "max7219.h"
#include "time_matrix.h"
#define DELAY_SHOW_DATE 3000 // aantal ms dat de datum moet blijven staan
#define DEBUG  true // use Serial



/////////////////// BEGIN declaratie functies //////////////////////

/////////////////// EIND declaratie functies //////////////////////

////////////////////////////// LET OP //////////////////////////
////////////////////////////////////////////////////////////////
////////////////// TEST STAAT UIT I.V.M, atmega 168 ////////////
////////////////// IN PLAATS VAN DE atmega 328      ////////////
////////////////////////////////////////////////////////////////

/**
 * TODO
 *    - settins kunnen aanpassen in eigen menu
 *    - date aanpassen in eigen menu
 *    - brightness HELEMAAL UIT
 *    - gegeven op eeprom  opslaan
 *    - verlichting van de knoppen regelen -> button defect
 *    - temperatuurweergave uit kunnen zetten
 *        DONE - datum en jaar weergeven
 *    - alarm instellen
 *    - uitzetten van seconden zonder dat brightness wordt aangeraakt
 *        DONE - am/pm
 *        DONE - temperatuur weergeven
 *        DONE - seconden weergeven
 */

#define MODE_NORMAL 0
#define MODE_SCROLL 1


bool writeTime      = false;       // time must be displayt
byte intensity      = 0x00;        // intencity of led matrix (0-15)
bool startUp        = true;        // first startup. Display temp or time
byte oldSec         = 0;           // last second displayed
byte oldMin         = 0;           // last minute displayed
byte oldUur         = 0;           // last minute displayed
bool amPm           = false;       // 12 uurs of 24 uurs notatie
bool mayShowTemp    = true;        // display temperatuur af en toe
bool mayShowSeconds = false;       // show seconds
bool shutDownMode   = false;       // alle leds uit
byte mode           = MODE_SCROLL; // huidige mode, moet nog uit eeprom komen
bool runTest        = false;       // tests zijn aan het draaien


void setup() {
  Serial.begin(115200);
  Serial.println("Begin");
 
  pinMode(DATA_IN, OUTPUT);
  pinMode(CLOCK,   OUTPUT);
  pinMode(LOAD,    OUTPUT);

  //status led uit
  digitalWrite(13, LOW);


  // knoppen
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_1_LED,   OUTPUT);
  pinMode(BTN_2_LED,   OUTPUT);

// attachInterrupt(digitalPinToInterrupt(BTN_1), handleButtonPressed, FALLING);
// attachInterrupt(digitalPinToInterrupt(BTN_2), handleButtonPressed, FALLING);

//initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
  for (int e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, intensity );    // the first 0x0f is the value you can set
                                           // range: 0x00 to 0x0f
  // test alles, durk op knop laat het stoppen
//  test();
  // voorlopig, moeten natuiurlijk uit staan
  digitalWrite(BTN_1_LED,LOW);
  digitalWrite(BTN_2_LED,LOW);
  Serial.println("Einde setup");
}



void setTijd(tmElements_t *time) {
  Serial.println("Begin setTijd");
  RTC.write(*time);
  writeTime = true;
  Serial.println("einde setTijd");
}

/**
 * Write temp to serial
 */
void  debugPrintTemp(float temp) {
  if (!DEBUG) {
    return;
  }
  Serial.print("temp: ");
  Serial.print(temp);
  Serial.println(" graden Celcius");
}

/**
 * wirte Timt to serial
 */
void debugPrintTime(tmElements_t *time) {
  if (!DEBUG) {
    return;
  }
  Serial.print("Datum: ");
  Serial.print(tmYearToCalendar(time->Year));
  Serial.print("-");
  Serial.print(time->Month);
  Serial.print("-");
  Serial.print(time->Day);
  Serial.print("\tTijd: ");
  Serial.print(time->Hour);
  Serial.print(":");
  Serial.print(time->Minute);
  Serial.print(":");
  Serial.println(time->Second);
}

void showTemp() {
  float temp = RTC.getTemp();
  debugPrintTemp(temp);
  tekenPatroon(EEN,  CIJFERS[(byte)temp / 10]);
  tekenPatroon(TWEE, CIJFERS[(byte)temp % 10]);
  tekenPatroon(DRIE, GRADEN);
  tekenPatroon(VIER, CELCIUS);
}

void showDate() {
  tmElements_t time;
  RTC.read(time); 

  tekenPatroon(EEN,  CIJFERS[time.Day   / 10]);
  tekenPatroon(TWEE, CIJFERS[time.Day   % 10]);
  tekenPatroon(DRIE, CIJFERS[time.Month / 10]);
  tekenPatroon(VIER, CIJFERS[time.Month % 10]);
  delay(DELAY_SHOW_DATE);
  int jaar = tmYearToCalendar(time.Year);
  tekenPatroon(EEN,  CIJFERS[ jaar / 1000     ]);
  tekenPatroon(TWEE, CIJFERS[(jaar / 100) % 10]);
  tekenPatroon(DRIE, CIJFERS[(jaar / 10 ) % 10]);
  tekenPatroon(VIER, CIJFERS[ jaar % 10       ]);
  delay(DELAY_SHOW_DATE);
}

void showSec(int oldTime, tmElements_t *time) {
  if (mode == MODE_SCROLL) {
    int newTime = time->Minute * 100 + time->Second;
    draaiVanNaar(oldTime, newTime, 20);
    return;
  }
  // normal mode
  tekenPatroon(EEN,  CIJFERS[time->Minute / 10]);
  tekenPatroon(TWEE, CIJFERS[time->Minute % 10]);
  tekenPatroon(DRIE, CIJFERS[time->Second / 10]);
  tekenPatroon(VIER, CIJFERS[time->Second % 10]);
}

byte getUur(byte uur) {
  if (amPm && uur > 12) {
    uur = uur - 12;
  }
  return uur;
}

void showTime(int oldTime, tmElements_t *time) {
  byte uur = getUur(time->Hour);
  
  if (mode == MODE_SCROLL) {
    int newTime = uur * 100 + time->Minute;
    draaiVanNaar(oldTime, newTime, 20);
    return;
  }
  
  tekenPatroon(EEN,  CIJFERS[uur         / 10]);
  tekenPatroon(TWEE, CIJFERS[uur          % 10]);
  tekenPatroon(DRIE, CIJFERS[time->Minute / 10]);
  tekenPatroon(VIER, CIJFERS[time->Minute % 10]);
  
}

void loop() {

  // kijk of er een knop wordt aangeraakt
  if (!digitalRead(BTN_1)) {
    handleButton1Pressed();
  }
  if (!digitalRead(BTN_2)) {
    handleButton2Pressed();
  }

  tmElements_t time;
  RTC.read(time); 
  byte uur     = time.Hour;
  byte minuut  = time.Minute;
  byte seconde = time.Second;

  if (oldSec != seconde || writeTime) 
  {
    debugPrintTime(&time);
    int oldTimeSec = oldMin*100 + oldSec;
    int oldTimeMin = getUur(oldUur)*100 + oldMin;
    
    
    if (mayShowSeconds) {
      // niet netjes, maar het moet meerdere keren. 
      oldSec = seconde;
      oldMin = minuut;
      oldUur = uur;
      showSec(oldTimeSec, &time); 
      writeTime = false;
      return;
    }
    oldSec = seconde;
  
    // mag temperatuur gezien worden?
    if (mayShowTemp) {
      if (!writeTime && (seconde >= TEMP_START_SEC && seconde < TEMP_STOP_SEC)) {
        // schrijf de temperatuur, maar doe het maar ff. en daarna weer de
        // tijd. Herhaal het echet niet te vaak.
        if (seconde == TEMP_START_SEC) {
          showTemp();
        }
        return;
      }
    }
    
    // laat minuten zien
    if (oldMin != minuut || writeTime || seconde == TEMP_STOP_SEC) {
      oldMin = minuut;
      oldUur = uur;
      writeTime = false;
      showTime(oldTimeMin, &time);
    }
  }
  
}
  



