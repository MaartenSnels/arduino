/*
  (c) Maarten Snels 2015
  Programma is gebasseerd op de Klok, zoals die ooit is geschreven voor de Commodore64 door 
  ondergetekende, in samenwerking met Jan Kees van der Meulen
  
  Kort gebruik van de toetsen
  A  Als je in een instellingmodus zit Annuleer
  B  Toggel ticks (aanwijzers om de vijf minuten. Elk kwartier heeft een afwijkende kleur)
     Ticks heeft meerder modi gekregen:
     12 uur -> elk half uur -> elk kwartier -> elke vijf minuten -> elke vijf minuten (kort) en elk kwartier (lang) -> uit -> 
  C  Instellen kleur
     1  seconden primair vooruit
     2  seconde secundair vooruit
     3  slinger volgende kleur vooruit
     4  minuut primair vooruit
     5  minuut secundair vooruit
     6  kleur ticks elke vijf minuten
     7  uur primair vooruit
     8  uur secundair vooruit
     9  kleur ticks elke 15 minuten
     A  anuleer acties
     C  bewaren en opslaan
 D   Dimmen, standen van 0->1..->5->0 etc.
 #   Zet de lengte van de wijzers voor de Fill en Drop mode.
     De lengte kan varieren van 2 tot 50
     1  seconde wijzer een langer
     2  seconde wijzer een korter
     4  minuten wijzer een langer
     5  minuten wijzer een korter
     7  uur wijzer een langer
     9  uur wijzer een korter
     A  anuleer acties
     #  bewaren en opslaan
 *   Zet de klok op tijd 
     1  seconde een vooruit
     2  seconde een terug
     4  minuten een vooruit
     5  minuten een terug
     7  uur een vooruit
     8  uur een terug
     9  12 uur vooruit (24 uurs modus voor datum en digitaal belangrijk)
           Als uurlampje 1 fel wit is dan is het voor de middag. Als het lampje
           warm wit is dan is het na de middag 
     A  anuleer acties
     *  bewaren en opslaan
 9   Zet de datum 
     1  dag van de maand een verder
     2  dag van de maand een terug
     4  maand een vooruit
     5  maand een terug
     7  jaar een vooruit
     9  jaar een terug   
     A  anuleer acties
     9  bewaren en opslaan
 0   Gebruik klok als lamp
 1   Wissel mode   hands -> walk -> hands pendula -> fill -> drop -> 
                   fill pendula -> trail -> trail pendula -> 
                   trail double -> classic -> Aljo
 2   race (bedenksel van Job)
 3   Schakel partymode in, elke volgende toestaanslag stopt de mode
 4   schakel tussen Light Mode Alles een kleur -> alle 5-minuten lampjes andere kleru -> blokken van 5 minuten -> blokken van 10 minuten ->
                               blokken van 15 -> 20 -> 30 ->
 6   Bekijk datum -> temp 
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
#define E_SECS            0  // lengte van de seconde wijzer 
#define E_MINS            1  // lengte van de minuten wijzer
#define E_HOURS           2  // lengte van de uren wijzer
#define E_MODE            3  // huidige mode (party wordt NOOIT opgeslagen)
#define E_DIMM            4  // huidige stand van het dimmen van de leds
#define E_TICKS           5  // weergave modus van de ticks
#define E_RANDOM_SEED     6  // sla random getal op om volgende keer een beter random nummer te genereren
#define E_LM_MODE         7  // sla gegevens voor Light Mode op
#define E_PENDULA         8  // sla pendula mode op
#define E_COLOR_SEC1     10  // lengte 3 voor rgb
#define E_COLOR_MIN1     13  // lengte 3 voor rgb
#define E_COLOR_UUR1     16  // lengte 3 voor rgb
#define E_COLOR_PENDULA  19  // lengte 3 voor rgb
#define E_COLOR_TICKS_15 22  // lengte 3 voor rgb
#define E_COLOR_TICKS_05 25  // lengte 3 voor rgb
#define E_COLOR_SEC2     28  // lengte 3 voor rgb
#define E_COLOR_MIN2     31  // lengte 3 voor rgb
#define E_COLOR_UUR2     34  // lengte 3 voor rgb


// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[LED_COUNT];
// sla hier voor de tijd de lampjes op die aan moeten staan. Voor
// dimmen e.d. wordt de kleur pas bepaald in de writeKlok routine
bool ledStatus[LED_COUNT];
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<PIN> klok; //initialiseer de strip, geen aan op welke pin de output staat

rgb_color colorSec1;
rgb_color colorSec2;
rgb_color colorMin1;                // gekozen kleur voor de minuten
rgb_color colorMin2;                // gekozen kleur voor de minuten
rgb_color colorUur1;                // gekozen kleur voor de uren
rgb_color colorUur2;                // gekozen kleur voor de uren
rgb_color colorPendula;             // kleur van de slinger
rgb_color colorTicks15;             // kleur ticks voor elke 15 minuten
rgb_color colorTicks05;             // kleur ticks voor elke 05 minuten

unsigned char SECS  = 5;            // aantal seconden dat de wijzer lang is
unsigned char MINS  = 5;            // aantal minuten dat de wijzer lang is
unsigned char HOURS = 5;            // aantal uren dat de wijzer lang is

char  seconde       = 0;            // huidgie tijd, seconden
char  minuut        = 0;            // huidige tijd, minuten
char  uur           = 0;            // huidige tijd, uren + minuten % 12 (elke 12 minuten komt er een "uur" bij
char  echtUur       = 0;            // huidige tijd, uren + minuten % 12 (elke 12 minuten komt er een "uur" bij
bool  pm            = false;        // am of pm
char  dag           = 1;            // dat in de maand
char  maand         = 1;
int   jaar          = 2000;         // jaartal
#define YEAR_OFFSET   1970          // klop begint met jaar 0 en dat is 1970
#define DIMM_MODES    6             // aantal helderheden
char dimm           = 0;            // Hoeveel moet alles gedimd worden (0, 1 of 2)

#define TICKS_OFF      0            // Geen ticks
#define TICKS_60       1            // alleen bij het hele uur
#define TICKS_60_HALF  2            // alleen bij het hele uur
#define TICKS_30       3            // elke 30 minute
#define TICKS_30_HALF  4            // elke 30 minute
#define TICKS_15       5            // elke 15 minuten
#define TICKS_15_HALF  6            // elke 15 minuten
#define TICKS_05       7            // elke 5  minuten
#define TICKS_05_HALF  8            // elke 5 minuten, maar dan alleen het eerste ledje
#define TICKS_ALL_HALF 9            // elke 5 minuten, maar dan alleen het eerste ledje
#define TICKS_MAX      10           // zorg voor een loop, begin na vijf weer op 0
#define TICKS_DIMM     3            // ticks zijn altijd donkderder dan de rest an het licht. Hier wordt aangegeven hoeveel donkerder
char ticks;                         // bijhouden van ticks status

// Light Modes
#define LM_ONE_COLOR   0            // alles een kleur
#define LM_ONE_MINUTE  1            // alleen een ander ledje, om de vijf minuten            
#define LM_BLOCK_05    2            // blok van vijf ledjes
#define LM_BLOCK_10    3            // block van 10 ledjaes
#define LM_BLOCK_15    4            // block van 15 ledjaes
#define LM_BLOCK_20    5            // block van 20 ledjaes
#define LM_BLOCK_30    6            // block van 30 ledjaes
#define LM_MAX         7
char lmMode;                        // sla actuele LM modus op

// Variabele en constanten voor de slinger
char posPendula          = 0;
bool heenPendula         = true;
bool pendulaOn           = false;
#define TIME_PENDULA       50           // aantal milliseconden dat het duur voordat de slinger een positie opschuift
#define LENGTE_PENDULA     11           // lengte van de slinger
#define LEFT_PENDULA       35           // waar begint de slinger te slingeren (30 + LENGTE_PENDULA / 2)
unsigned long oldMills   = 0;           // bijhouden of de slinger eentje op moet schruiven

// knoppen keybord met speciale betekenis
#define KEY_LAMP               '0'
#define KEY_SEC_MIN            '1'
#define KEY_SET_MODE           '1'
#define KEY_SEC_PLUS           '2'
#define KEY_RACE               '2'
#define KEY_SET_COLOR_PENDULA  '3'
#define KEY_PARTY_MODE         '3'
#define KEY_SET_LM_MODE        '4'
#define KEY_MIN_MIN            '4'
#define KEY_PENDULA            '5'
#define KEY_MIN_PLUS           '5'
#define KEY_SET_COLOR_TICKS_05 '6'
#define KEY_SHOW_DATE_TEMP     '6'
#define KEY_UUR_MIN            '7'
#define KEY_UUR_PLUS           '8'
#define KEY_SET_COLOR_TICKS_15 '9'
#define KEY_UUR_24             '9'
#define KEY_SET_DATE           '9'
#define KEY_ANNULEER           'A'
#define KEY_TICKS              'B'
#define KEY_SET_COLOR          'C'
#define KEY_SET_DIMM           'D'
#define KEY_SET_HAND_LENGTH    '#'
#define KEY_SET_TIME           '*'

// set keypad
#define KB_ROWS              4  //four rows
#define KB_COLS              4  //four columns
char keys[KB_COLS][KB_ROWS] = {
  {KEY_SET_DIMM        , KEY_SET_COLOR, KEY_TICKS   , KEY_ANNULEER   },
  {KEY_SET_HAND_LENGTH , KEY_SET_DATE , '6'         , KEY_PARTY_MODE },
  {KEY_LAMP            , KEY_UUR_PLUS , KEY_MIN_PLUS, KEY_SEC_PLUS   },
  {KEY_SET_TIME        , KEY_UUR_MIN  , KEY_MIN_MIN , KEY_SEC_MIN    }
};
// aansluiting van matrix van rechts -> links (meest rechts -> 2, meest links -> 9)
byte rowPins[KB_COLS] = {5, 6,  7,  8 }; //connect to the row pinouts of the keypad
byte colPins[KB_ROWS] = {9, 10, 11, 12}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, KB_ROWS, KB_COLS );


// modes
#define    HANDS             0         // wijzers per ring, lengte is te bepalen
#define    WALK              1         // HANDS, met vallende lampjes
#define    FILL              2         // ring wordt steeds verder gevuld tot het aan de 60 komt, dan leet
#define    DROP              3         // FILL met vallende lampjes
#define    TRAIL             4         // Wijzers wordt vanaf aanwijspunt naar achter steeds vager (spoor van licht)
#define    TRAIL_DOUBLE      5         // van vaag naar klok naar vaag, twee kleuren
#define    CLASSIC           6         // Wijzers, laange is seconde, korte uren en de middelste de minuten.
#define    CLASSIC_VERVAAG  99         // work in progress
#define    ALJO              7         // Digitale cijfers
#define    PARTY             8         // Feestelijke bedoeling

#define MAX_MODE PARTY              // aantal modi waarover geloopt wordt. Hiermee bepaal je of party wel of niet meegenomen wordt
unsigned char mode, oldMode;        // mode en vroergere mode van de applicatie  
unsigned int seed = 0;              // used to initialize random number generator


// in digital mode, wanneer moet er naar datum en temperatuur gekeken worden
#define DATE_ON       9  // seconde waarop datum aan gaat
#define DATE_OFF     14  // seconde waarop datum uit gaat
#define TEMP_ON      44  // seconde waarop temperatuur aan gaat
#define TEMP_OFF     49  // seconde waarop temperatuur uit gaat

#define VERJAARDAG_MINUTEN      15        // om de hoeveel minuten moet er een party afgaan op verjaardagen
#define PARTIES_IN_PARTYMODE    25        // hoeveel parties mogen er achter elkaar gedraaid worden
#define PARTIES_IN_VERJAARDAGEN  2        // als verjaardag, hoeveel parties mogen er achter elkaar gedraaid worden


void setup() {
    // haal de instellingen voor de klok op
    mode         = EEPROM.read(E_MODE   ) % MAX_MODE;
    lmMode       = EEPROM.read(E_LM_MODE) % LM_MAX;
    oldMode      = mode;
    ticks        = EEPROM.read(E_TICKS) % TICKS_MAX;
    dimm         = EEPROM.read(E_DIMM ) % DIMM_MODES;
//    pendulaOn    = EEPROM.read(E_PENDULA);
    SECS         = min(max(EEPROM.read(E_SECS ), 2), 50);
    MINS         = min(max(EEPROM.read(E_MINS ), 2), 50);
    HOURS        = min(max(EEPROM.read(E_HOURS), 2), 50);
    getColors();
    initClock();


  // initialize the random number generator with a seed obtained by
  // summing the voltages on the disconnected analog inputs
  for (int i = 0; i < 8; i++)
  {
    seed += analogRead(i);
  }
  seed += EEPROM.read(E_RANDOM_SEED);  // get part of the seed from EEPROM
  randomSeed(seed);
  // save a random number in EEPROM to be used for random seed
  // generation the next time the program runs
  EEPROM.write(E_RANDOM_SEED, random(256));
    
    //debugging
    Serial.begin(9600);
}

void loop() {
  
  char key = keypad.getKey();
  char parties = 25;
  if (key) {
    Serial.println(key);
    switch (key) {
      case KEY_SET_HAND_LENGTH:
        setHandLength();
        initClock();
        break;
      case KEY_SET_MODE :
        mode = (mode + 1) % MAX_MODE;
        pendulaOn = false;
  //      EEPROM.write(E_PENDULA,pendulaOn);
        // nooit in party mode blijven hangen!
        if (mode!=PARTY) {
          EEPROM.write(E_MODE,mode);
          allesAan(OFF);
          initClock();
        }
        break;
      case KEY_PENDULA:
        if (isPendulaAllowed()) {
          pendulaOn = !pendulaOn;
          if (pendulaOn) {
            row(SEC_RING,SEC_START,SEC_STOP, OFF);
            for (int i = 0; i < 60; i++) {
              ledStatus[i + SEC_START] = false;
            }
          }
          setTicks();
          initClock();
  //        EEPROM.write(E_PENDULA,pendula);
        }
        break;
      case KEY_SET_COLOR:
        setHandColor();
        initClock();
        break;
      case KEY_SET_TIME:
        setCurrentTime();
        break;
      case KEY_SET_DATE:
        setCurrentDate();
        break;
      case KEY_SHOW_DATE_TEMP:
        // TODO implementatie van onderstaande functies maken
        runShowDate();
        runShowTemp();
        initClock();
        break;
      case KEY_SET_DIMM:
        dimm = (dimm + 1) % DIMM_MODES;
        EEPROM.write(E_DIMM,dimm);
        initClock();
        break;
      case KEY_SET_LM_MODE:
        lmMode = (lmMode + 1) % LM_MAX;
        EEPROM.write(E_LM_MODE, lmMode);
        initClock();
        break;
      case KEY_TICKS:
        ticks = (ticks + 1) % TICKS_MAX;
        EEPROM.write(E_TICKS,ticks);
        initClock();
        break;
      case KEY_LAMP:
        lamp();
        initClock();
        break; 
      case KEY_PARTY_MODE:
        oldMode = mode;
        mode = PARTY;
        initClock();
        break;
      case KEY_RACE:
        runRace();
        //runRadar(10,GREEN);
        break; 
    }
  }

initTime:
  tmElements_t tm;
  RTC.read(tm); 
  char s = tm.Second;
  char m = tm.Minute;
  char u = (tm.Hour % 12) * 5 + m / 12;
  dag = tm.Day;
  maand = tm.Month;
  pm = tm.Hour > 12;
  
  // eerst ff checken of de party mode aan moet
  bool verjaardag = false;
  if (m % VERJAARDAG_MINUTEN == 0 && s > 5 && s < 7) {
    if (isVerjaardag()) {
      oldMode = mode;
      verjaardag = true;
      mode    = PARTY;
    }
  }
  if (mode == PARTY) {
    runParty(verjaardag? PARTIES_IN_VERJAARDAGEN: PARTIES_IN_PARTYMODE);
    mode = oldMode;
    verjaardag = false;
    initClock();
    // de tijd moet opnieuw bepaald worden anders dan 
    // staat de klok even verkeerd. Niet zo netjes.
    goto initTime;
  }
  
  // gooi de slinger op en neer
  if (isPendulaOn()) {  
    pendula();
  }
  // vervagen??
  if (mode == CLASSIC_VERVAAG) {
    vervaagClassicSeconde();
  }
  
  if (s!=seconde) {
    printTime(tm);
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
  
}

void printTime(tmElements_t tm) {
  Serial.print("Tijd: ");
  Serial.print(tm.Hour);
  Serial.print(":");
  Serial.print(tm.Minute);
  Serial.print(":");
  Serial.println(tm.Second);
}

bool isPendulaOn() {
  return pendulaOn && isPendulaAllowed(); 
}  

bool isPendulaAllowed() {
  return mode == HANDS         || 
         mode == FILL          ||
         mode == TRAIL_DOUBLE  ||
         mode == TRAIL; 
}  



void nextSec() {
  switch (mode) {
    case HANDS:
    case FILL:
    case TRAIL:
    case TRAIL_DOUBLE:
    case ALJO:
    case CLASSIC_VERVAAG:
    case CLASSIC:
      oldMills = millis();
      // als pendula aan staat, geen seconden tonen
      if (isPendulaOn()) {
        return;
      }
      initClock();
      break;
    case WALK:
      setHandWalk(SEC_RING);
      break;
    case DROP:
      setHandDrop(SEC_RING);
      break;
  }
}

void nextMin() {
  if (isPendulaOn()) {
    initClock();
    return;
  }
  switch (mode) {
    case WALK:
      setHandWalk(MIN_RING);
      break;
    case DROP:
      setHandDrop(MIN_RING);
      break;
  }
}

void nextUur() {
  switch (mode) {
    case WALK:
      setHandWalk(UUR_RING);
      break;
    case DROP:
      setHandDrop(UUR_RING);
      break;
  }
}

