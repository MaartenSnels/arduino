/*
  (c) Maarten Snels 2015
  Programma is gebasseerd op de Klok, zoals die ooit is geschreven voor de Commodore64 door 
  ondergetekende, in samenwerking met Jan Kees van der Meulen
  
  Kort gebruik van de toetsen
  A  -
  B  Toggel tikcs (aanwijzers om de vijf minuten. Elk kwartier heeft een afwijkende kleur
  C  Instellen kleur
     1  seconden vorige kleur
     2  seconde volgende kleur
     4  minuut vorige kleur
     5  minuut volgende kleur
     7  uur vorige kleur
     8  uur volgende kleur
     A  anuleer acties
     C  bewaren en opslaan
 D   Dimmen, standen van 0->1..->5->0 etc.
 #   Zet de lengte van de wijzers voor de Fill en Drop mode.
     De lengte kan varieren van 2 tot 50
     1  seconde wijzer één langer
     2  seconde wijzer één korter
     4  minuten wijzer één langer
     5  minuten wijzer één korter
     7  uur wijzer één langer
     9  uur wijzer één korter
     A  anuleer acties
     #  bewaren en opslaan
 *   Zet de klok op tijd 
     1  seconde wijzer één langer
     2  seconde wijzer één korter
     4  minuten wijzer één langer
     5  minuten wijzer één korter
     7  uur wijzer één langer
     9  uur wijzer één korter
     A  anuleer acties
     *  bewaren en opslaan
 1   Wissel mode hands -> walk -> fill -> drop -> party
*/

#include <Keypad.h>
#include <PololuLedStrip.h>
#include <Wire.h>
/* 
 * LET OP! als je Time include dan moet je van de DateStrings.cpp wat aanpassingingen doen
 *
 * monthShortNames_P[], dayNames_P[] en dayShortNames_P[]
 * moeten als constanten gedeclareerd worden
 *
 */
#include <Time.h>
#include <DS3231RTC.h>
#include <EEPROM.h>

#include "kleuren.h"

#define PIN         13      // dataline
#define LED_COUNT  180      // aantal lejdes in de strip

#define SEC_START     0      // eerste lanmpje voor seconden
#define SEC_STOP     59      // laaste lampje voor seconden
#define MIN_START    60      // eerste lanmpje voor minuten
#define MIN_STOP    119      // laaste lampje voor minuten
#define UUR_START   120      // eerste lanmpje voor uren
#define UUR_STOP    179      // laaste lampje voor uren

#define SEC_RING      0      // buitenste ring
#define MIN_RING      1      // mindeen ring
#define UUR_RING      2      // binneste ring

#define WAIT_FAST     0      // delay voor vallende lampjes, nu ff op 0, routine duurt te lang
#define COLORS       26      // aantal kleuren waaruit gekozen kan worden

// ONTHOUDEN WAARDEN VOOR DE EPPROM
#define E_SECS          0
#define E_MINS          1
#define E_HOURS         2
#define E_MODE          3
#define E_DIMM          4
#define E_TICKS         5
#define E_COLOR_SEC    10  // lengte 3 voor rgb
#define E_COLOR_MIN    13  // lengte 3 voor rgb
#define E_COLOR_UUR    16  // lengte 3 voor rgb

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[LED_COUNT];
// sla hier voor de tijd de lampjes op die aan moeten staan. Voor
// dimmen e.d. wordt de kleur pas bepaald in de writeKlok routine
bool ledStatus[LED_COUNT];
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<PIN> klok; //initialiseer de strip, geen aan op welke pin de output staat

rgb_color colorSec;                // gekozen kleur voor de seconden
rgb_color colorMin;                // gekozen kleur voor de minuten
rgb_color colorUur;                // gekozen kleur voor de uren

unsigned char SECS      =  5;     // aantal seconden dat de wijzer lang is
unsigned char MINS      =  5;     // aantal minuten dat de wijzer lang is
unsigned char HOURS     =  5;     // aantal uren dat de wijzer lang is

int  seconde      = 0;             // huidgie tijd, seconden
int  minuut       = 0;             // huidige tijd, minuten
int  uur          = 0;             // huidige tijd, uren + minuten % 12 (elke 12 minuten komt er een "uur" bij
bool pm           = false;         // am of pm
#define TICKS_DIMM  2              // ticks hebben twee standen. Afhankelijk van hoeveelheid dimmen wordt de donkere gekozen
#define DIMM_MODES  5              // aantal helderheden
char dimm         = 0;             // Hoeveel moet alles gedimd worden (0, 1 of 2)
boolean ticks     = true;          // moeten ticks gezien worden

// knoppen keybord met speciale betekenis
#define KEY_SEC_MIN         '1'
#define KEY_SEC_PLUS        '2'
#define KEY_MIN_MIN         '4'
#define KEY_MIN_PLUS        '5'
#define KEY_UUR_MIN         '7'
#define KEY_UUR_PLUS        '8'
#define KEY_ANNULEER        'A'
#define KEY_TICKS           'B'
#define KEY_SET_COLOR       'C'
#define KEY_SET_DIMM        'D'
#define KEY_SET_HAND_LENGTH '#'
#define KEY_SET_TIME        '*'

// set keypad
#define KB_ROWS              4  //four rows
#define KB_COLS              4  //four columns
char keys[KB_COLS][KB_ROWS] = {
  {KEY_SET_DIMM        , KEY_SET_COLOR, KEY_TICKS   , KEY_ANNULEER },
  {KEY_SET_HAND_LENGTH ,'9'           , '6'         , '3'          },
  {'0'                 , KEY_UUR_PLUS , KEY_MIN_PLUS, KEY_SEC_PLUS },
  {KEY_SET_TIME        , KEY_UUR_MIN  , KEY_MIN_MIN , KEY_SEC_MIN  }
};
// aansluiting van matrix van rechts -> links (meest rechts -> 2, meest links -> 9)
byte rowPins[KB_COLS] = {5, 6,  7,  8 }; //connect to the row pinouts of the keypad
byte colPins[KB_ROWS] = {9, 10, 11, 12}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, KB_ROWS, KB_COLS );


// modes
enum Mode {
  hands = 0,
  walk  = 1,
  fill  = 2,
  drop  = 3,
  party = 5
};
#define MAX_MODE 5       // aantal modi waarover geloopt wordt. Hiermee bepaal je of party wel of niet meegenomen wordt
unsigned char mode;



void setup() {
    mode     = EEPROM.read(E_MODE) % MAX_MODE;
    ticks    = EEPROM.read(E_TICKS);
    colorSec = (rgb_color){EEPROM.read(E_COLOR_SEC), EEPROM.read(E_COLOR_SEC + 1) , EEPROM.read(E_COLOR_SEC + 2)};
    colorMin = (rgb_color){EEPROM.read(E_COLOR_MIN), EEPROM.read(E_COLOR_MIN + 1) , EEPROM.read(E_COLOR_MIN + 2)};
    colorUur = (rgb_color){EEPROM.read(E_COLOR_UUR), EEPROM.read(E_COLOR_UUR + 1) , EEPROM.read(E_COLOR_UUR + 2)};

    dimm      = min(max(EEPROM.read(E_DIMM ), 0), 2);

    
    SECS      = min(max(EEPROM.read(E_SECS ), 2), 50);
    MINS      = min(max(EEPROM.read(E_MINS ), 2), 50);
    HOURS     = min(max(EEPROM.read(E_HOURS), 2), 50);
    initClock();
    
    //debugging
//    Serial.begin(9600);
}

void loop() {
  
  char key = keypad.getKey();
  
  switch (key) {
    case KEY_SET_HAND_LENGTH:
      setHandLength();
      break;
    case KEY_SEC_MIN :
      mode = (mode + 1) % MAX_MODE;
      EEPROM.write(E_MODE,mode);
      break;
    case KEY_SET_COLOR:
      setHandColor();
      break;
    case KEY_SET_TIME:
      setCurrentTime();
      break;
    case KEY_SET_DIMM:
      dimm = (dimm + 1) % DIMM_MODES;
      EEPROM.write(E_DIMM,dimm);
      initClock();
      break;
    case KEY_TICKS:
      ticks = !ticks;
      EEPROM.write(E_TICKS,ticks);
      initClock();
      break;
  }
  
  if (mode == party) {
    runParty();
  }
    
  tmElements_t tm;
  RTC.read(tm); 
  int s = tm.Second;
  int m = tm.Minute;
  int u = (tm.Hour % 12) * 5 + m / 12;
  pm = tm.Hour > 12;
  
  
  if (s!=seconde) {
    seconde = s; 
    nextSec();
  }
  if (m!=minuut) {
    minuut = m;
    nextMin();
  }
  if (u!=uur) {
    uur = u;
    nextUur();
  }
  
  delay (5);
}

void showMode() {
    row(SEC_RING, 0, SECS);
    row(MIN_RING, 0, MINS);
    row(UUR_RING, 0, HOURS);
}

void setHandWalk(char ring) {
  char pos, startPos, stopPos, lengthHand;
  switch (ring) {
    case SEC_RING:
      pos = seconde;
      startPos = SEC_START;
      stopPos = SEC_STOP;
      lengthHand = SECS;
      break;
    case MIN_RING:
      pos = minuut + MIN_START;
      startPos = MIN_START;
      stopPos = MIN_STOP;
      lengthHand = MINS;
      break;
    case UUR_RING:
      pos = uur + UUR_START;
      startPos = UUR_START;
      stopPos = UUR_STOP;
      lengthHand = HOURS;
      break;
  }

  // er zijn twee mogelijkheden: vanaf
  // sec - SECS > 0  ( Alle ledjes branden achter elkaar)
  //   stap 1 -> loop van sec-SECS naar SEC_START 
  //   stap 2 -> loop van SEC_STOP naar sec
  // sec - SECS < 0
  //   stap 1 -> loop van sec-SECS + 60 naar sec
  
  if (pos - lengthHand >= startPos) {
    // stap 1
    for (int i = pos - lengthHand; i >= startPos; i--) {
      ledStatus[i] = true;
      writeKlok();
      delay(WAIT_FAST);
      ledStatus[i] = false;  
    }
    
    // stap 2
    for (int i = stopPos; i >= pos; i--) {
      ledStatus[i] = true;  
      writeKlok();
      delay(WAIT_FAST);
      ledStatus[i] = false;  
    }
  }
  else {
    for (int i = pos + 60 - lengthHand; i >= pos; i--) {
      ledStatus[i] = true;  
      writeKlok();
      delay(WAIT_FAST);
      ledStatus[i] = false;  
    }
    
  }
  // laatste ledje had niet uitgezet mogen worden
  ledStatus[pos] = true;
  writeKlok();
}

void setHandDrop(char ring) {
  char pos, startPos, stopPos;
  switch (ring) {
    case SEC_RING:
      pos = seconde + SEC_START;
      startPos = SEC_START;
      stopPos = SEC_STOP;
      break;
    case MIN_RING:
      pos = minuut + MIN_START;
      startPos = MIN_START;
      stopPos = MIN_STOP;
      break;
    case UUR_RING:
      pos = uur + UUR_START;
      startPos = UUR_START;
      stopPos = UUR_STOP;
      break;
  }
  
  // als aantal is 0, haal eerst de hele reeks leeg!
  if (pos == startPos) {
    for (int i = stopPos; i >= startPos; i--) {
      ledStatus[i] = false;
    }
    writeKlok();
  }

  for (int i = stopPos; i >= pos; i--) {
    ledStatus[i] = true;
    writeKlok();
    delay(WAIT_FAST);
    ledStatus[i] = false;
  }
  // laatste ledje had niet uitgezet mogen worden
  ledStatus[pos] = true;
  writeKlok();
}


/**
  Hier moet nog gekekn worden naar de modus etc.
*/
void nextSec() {
  switch (mode) {
    case hands:
    case fill:
      initClock();
      break;
    case walk:
      setHandWalk(SEC_RING);
      break;
    case drop:
      setHandDrop(SEC_RING);
  }
}

void nextMin() {
  switch (mode) {
    case hands:
    case fill:
      initClock();
      break;
    case walk:
      setHandWalk(MIN_RING);
      break;
    case drop:
      setHandDrop(MIN_RING);
  }
}

void nextUur() {
  switch (mode) {
    case hands:
    case fill:
      initClock();
      break;
    case walk:
      setHandWalk(UUR_RING);
      break;
    case drop:
      setHandDrop(UUR_RING);
  }
}


void initClock() {
  tmElements_t tm;
  
  RTC.read(tm); 
  seconde = tm.Second;
  minuut = tm.Minute;
  uur = (tm.Hour % 12) * 5 + minuut / 12;

  
  switch(mode) {
    case hands:
    case walk:
      initWalk(SEC_RING);
      initWalk(MIN_RING);
      initWalk(UUR_RING);
      break;
    case fill:
    case drop:
      for (int i = 0; i < 60; i++) {
        ledStatus[i+SEC_START] = (i <= seconde);
      }
      for (int i = 0; i < 60; i++) {
        ledStatus[i+MIN_START] = (i <= minuut);
      }
      for (int i = 0; i < 60; i++) {
        ledStatus[i+UUR_START] = (i <= uur);
      }
      break;
    
  }
  writeKlok();
}

void initWalk(char ring) {
  char pos, startPos, length;
  switch (ring) {
    case SEC_RING:
      pos = seconde;
      startPos = SEC_START;
      length = SECS;
      break;
    case MIN_RING:
      pos = minuut;
      startPos = MIN_START;
      length = MINS;
      break;
    case UUR_RING:
      pos = uur;
      startPos = UUR_START;
      length = HOURS;
      break;
  }
     
  if (pos - SECS >= 0) {
    for (int i = 0; i < 60; i++) {
      ledStatus[i+startPos] = (i > pos-length && i <= pos);
    }
  } 
  else {
    for (int i = 0; i < 60; i++) {
      ledStatus[i+startPos] = (i > pos - length + 60 || i <= pos);
    }
  }

}


// bepaal of een ledje een bepaalde kleur heeft, 
// index is de positie in de colors matrix
boolean compare(int index, rgb_color color) {
  rgb_color c = colors[index];
  return c.red==color.red && c.green == color.green && c.blue == color.blue; 
}

/**
* bepaal een kleur aan de hand van de onderstaande codering. Bij het verhogen
* van het aantal rgb kleuren moet je ook de define COLORS aanpassen, dit is
* (aantal_elementen ^ 3) - 1
* op dit moment 3^3-1 = 26
*/
unsigned char rgb[] = {0,51,102}; //,153,204,255};
rgb_color rgbColorNr(unsigned char nummer) {
    return (rgb_color) {rgb[(nummer / 9) % 3],rgb[(nummer / 3) % 3],rgb[nummer % 3]};
}





