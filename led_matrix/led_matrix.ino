#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>
// #include <MsTimer2.h>

#include "Deklok.h"
#include "max7219.h"

/*-----------------------------------

General notes: 


-if you are only using one max7219, then use the function maxSingle to control
 the little guy ---maxSingle(register (1-8), collum (0-255))

-if you are using more then one max7219, and they all should work the same, 
then use the function maxAll ---maxAll(register (1-8), collum (0-255))

-if you are using more than one max7219 and just want to change something
at one little guy, then use the function maxOne
---maxOne(Max you wane controll (1== the first one), register (1-8), 
collum (0-255))

/* During initiation, be sure to send every part to every max7219 and then
 upload it.
For example, if you have five max7219's, you have to send the scanLimit 5 times
before you load it-- other wise not every max7219 will get the data. the
function maxInUse  keeps track of this, just tell it how many max7219 you are
using.

*/



// klok variabelen
// lokale variabelen voor klok
int actueelUur     = 1, 
    actueelMinuut  = 1,
    actueelSeconde = 1;

/* in de patronen worden de actuele status 
   van de leds opgelagen. Deze moeten dus ook ge
   tekend worden. Met bitwise operatoren kunnen zo
   snel invers worden bepaald etc. 
*/
byte patroonSec[8];
byte patroonMin[8];
byte patroonUur[8];


void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (--i);              // get bitmask
    digitalWrite( clock, LOW);         // tick
    digitalWrite(dataIn, data & mask);
    digitalWrite(clock, HIGH);         // tock
  }
}

void maxSingle( byte reg, byte col) {    
//maxSingle is the "easy"  function to use for a     //single max7219

  digitalWrite(load, LOW);       // begin     
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data   
  digitalWrite(load, LOW);       // and load da shit
  digitalWrite(load,HIGH); 
}

void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin     
  for ( c =1; c<= maxInUse; c++) {
    putByte(reg);  // specify register
    putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}

void maxOne(byte maxNr, byte reg, byte col) {    
//maxOne is for adressing different MAX7219's, 
//whilele having a couple of them cascaded

  int c = 0;
  digitalWrite(load, LOW);  // begin     

  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data 

  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(load, LOW); // and load da shit
  digitalWrite(load,HIGH); 
}



void flash() {
  static boolean output = HIGH;

  digitalWrite(13, output);
  output = !output;
}

void setup() {
  // Serial.begin(9600);
  pinMode(13, OUTPUT);

//  MsTimer2::set(1000, flash); // 500ms period
//  MsTimer2::start();


  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);

  digitalWrite(13, HIGH);  

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
  testKlok();                                               
  initKlok();
} 

/*************************************
*
* klok hulp routines
*
*************************************/

byte *getPatroon(byte ring) {
    switch (ring) {
      case UUR_RING: 
        return patroonUur;
      case MIN_RING: 
        return patroonMin;
      case SEC_RING: 
        return patroonSec;
    }
}

void tekenPatroon(byte ring, byte *patroon) {
  for (int i = 0; i<= 8; i++) {
    maxOne(ring, i + 1, patroon[i]);
  }
}

/* **********************************
*
* STANDAARD ROUTINES VOOR MAKEN VAN MOOIE PLAATJES
*
************************************/

// laat een enkel ledje een rondje lopen
void rondje(byte ring, int wacht) {
  for (int rij = 1; rij < 9; rij++) {
    for (int kolom = 1; kolom < 9; kolom++) {
      maxOne(ring, rij, LAMPJES_ENKEL[kolom]);
      delay(wacht);
    }
    // laatste lampje in rij uit
    maxOne(ring, rij, 0x00);
  }
}

// laad alle rode led's branden
void rood() {
  for (int i = 1; i < 9 ; i++ ) {
    maxOne(MIN_RING, i, ROOD_MIN_GROEN_UUR[i-1]);
    maxOne(UUR_RING, i, GROEN_MIN_ROOD_UUR[i-1]);
  }
}


// laat alle groene leds branden
void groen() {
  for (int i = 1; i < 9 ; i++ ) {
    maxOne(MIN_RING, i, GROEN_MIN_ROOD_UUR[i-1]);
    maxOne(UUR_RING, i, ROOD_MIN_GROEN_UUR[i-1]);
  }
}

/* veeg (een voor een) alle lampjes aan of uit */
void veeg(byte ring, bool aan, int wacht) {
  for(int i = 1; i <= 60; i++) {
    if (aan) {
      fill(ring,i);
    } else {
      fill(ring, 60 - i);
    }
    delay(wacht);  //15*60=900ms
  }
}

void allesAanUit(byte ring, bool aan) {
  byte patroon = LAMPJES_RIJ[0];
  if (aan)
    patroon = LAMPJES_RIJ[8];
  
  byte i = 1;
  while (i <=8) {
    maxOne(ring, i, patroon);
    i = i + 1;
  }
}

/*****************************************************
*
* KLOK TIJD ROUTINES
*
******************************************************/

/**
  voor het effect valt er steeds een lampje van de zestig naar beneden, 
  verder gelijk aan fill
*/
void drop(byte ring, byte aantal) {

    if (aantal==1) {
      veeg(ring,false,10);
      fill(ring, aantal);
      return;
    } 
    
    byte *patroon = getPatroon(ring);
    for (int i = 60; i > aantal; i--) {
      int rij = i / 8 + 1; // wordt altijd afgekapt
      int rest = i % 8;
      
      byte patroonFlits = patroon[rij] |  LAMPJES_ENKEL[rest];
      maxOne(ring, rij, patroonFlits);
      delay(5);
      maxOne(ring, rij, patroon[rij]);
    } 
  
  fill(ring, aantal);
}

/**
  vul de ringen lampme voor lampje, als de ring helemaal vol is, leeg deze dan 
*/
void fill(byte ring, byte aantal) {
  
  
    byte *patroon = getPatroon(ring);
    
    // bereken het aantal lampjes dat aan moet staan
    int aantalRij = aantal / 8; // wordt altijd afgekapt
    int rest = aantal % 8;
    
    int i = 0;
    // deze moeten allemaal aan staan
    while(i<aantalRij) {
      patroon[i] = LAMPJES_RIJ[8];
      ++i;
    }
    //
    patroon[aantalRij] = LAMPJES_RIJ[rest];
    // deze moeten allemaal uit staan
    i=aantalRij+1;
    while( i<9) {
      patroon[i] = LAMPJES_RIJ[0];
      ++i;
    }
    
    tekenPatroon(ring,patroon);
}

void draai(byte ring, byte aantal) {
  // verzin hiet nog vanalles. waarchijnlijk met
  // patroon en bitshift
  // een verder is een shift. een and met de laatste
  // aantal of er iets van af valt. Deze met een or
  // bij de volgende optellen. Zo heb je alleen een initieel
  // patroon nodig en verder alleen een +1. Misschien ook zoiets voor fill, hoeven we niet steeds
  // alles eerst te berekenen, alleen in de initklok

    byte *patroon = getPatroon(ring);
    
    bool over = false;
    for (int i = 0; i<= 8; i++) {
      bool nuOver = false;
      if (patroon[i] & 0x01 == 0x01) {
        nuOver = true;
      }
      patroon[i] = patroon[i] >> 1; //shift een naar rechts
      if (over) {
        patroon[i] = patroon[i] | 0x80; //zet eerste lampje aan
      }
      over = nuOver; // onthoudt voor de volgende ronde
    }
    if (over) {
      patroon[0] = patroon[0] | 0x80; //zet eerste lampje aan
    }
    
    tekenPatroon(ring, patroon);
    
} 

/*****************************************************
*
* KLOK ROUTINES
*
******************************************************/


void initKlok() {
// lokale variabelen voor klok
  actueelUur     = 1; 
  actueelMinuut  = 1;
  actueelSeconde = 1;
  
//  while(true)
//    draai(SEC_RING, 1);
  
  patroonUur[0] = LAMPJES_ENKEL[1];
  patroonMin[0] = LAMPJES_ENKEL[1];
  patroonSec[0] = LAMPJES_ENKEL[1];
  int i = 1;
  while(i<9) {
    patroonUur[i] = LAMPJES_ENKEL[0];
    patroonMin[i] = LAMPJES_ENKEL[0];
    patroonSec[i] = LAMPJES_ENKEL[0];
    i++;
  }
  drop(UUR_RING, actueelUur);
  drop(MIN_RING, actueelMinuut); 
  drop(SEC_RING, actueelSeconde);

}

void testKlok() {
  byte wacht = 1000;
  // alles uit
  allesAanUit(UUR_RING,false);
  allesAanUit(MIN_RING,false);
  allesAanUit(SEC_RING,false);
//  delay(500);
//  allesAanUit(UUR_RING,false);
//  allesAanUit(MIN_RING,false);
//  allesAanUit(SEC_RING,false);
  
  // even allee ringen een voor een aan
  allesAanUit(UUR_RING,true);
  delay(wacht);
  allesAanUit(UUR_RING,false);
  delay(wacht);
  allesAanUit(MIN_RING,true);
  delay(wacht);
  allesAanUit(MIN_RING,false);
  delay(wacht);
  allesAanUit(SEC_RING,true);
  delay(wacht);
  allesAanUit(SEC_RING,false);
  delay(wacht);
  
  // alles weer aan
  allesAanUit(UUR_RING,true);
  allesAanUit(MIN_RING,true);
  allesAanUit(SEC_RING,true);
  
  // veeg alles uit
  veeg(UUR_RING, false, 10);
  veeg(MIN_RING, false, 10);
  veeg(SEC_RING, false, 10);
  // veeg alles aan
  veeg(UUR_RING, true, 10);
  veeg(MIN_RING, true, 10);
  veeg(SEC_RING, true, 10);

  // alles uit
  allesAanUit(UUR_RING,false);
  allesAanUit(MIN_RING,false);
  allesAanUit(SEC_RING,false);

  // vijf rondjes
  byte i = 0;
  while(i++<4) {
    rondje(SEC_RING, (4-i)*5 + 1);
    rondje(MIN_RING, (4-i)*5 + 1);
    rondje(UUR_RING, (4-i)*5 + 1);
  }
  // nu per kleur, geel ff overslaan
  rood();
  delay(2000);
  groen();
  delay(2000);
  
  // alles uit
  allesAanUit(UUR_RING,false);
  allesAanUit(MIN_RING,false);
  allesAanUit(SEC_RING,false);

  
}


/*****************************************************
*
* HET ACTUELE PROGRAMMA
*
******************************************************/



void loop () {
  
  /*
   *  Begin van de eerst poging om wat seconde lampjes te laten branden  
   */
 
// //patroonSec[0]=LAMPJES_RIJ[5];
// while(true) {
//   draai(SEC_RING, 1);
//   delay(30);
// }
   
  // uur voorbij
  if (actueelMinuut > 60) {
    actueelMinuut = 1;
    if (actueelUur > 60) {
      actueelUur = 1;
    }
  }

  // minuut voorbij
  if (actueelSeconde > 60) {
    actueelSeconde = 1;
    ++actueelMinuut;
    if (actueelMinuut % 12 == 0) {
      actueelUur++;
      drop(SEC_RING, actueelUur);
    }
    drop(MIN_RING, actueelMinuut);
  }
  
  // begin met seconde
  drop(SEC_RING, actueelSeconde);
  ++actueelSeconde;
  
  
  delay(1000 - (60 - actueelMinuut)*5 );

}
