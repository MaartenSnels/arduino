/*

  In deze file staan de routines die de setting van de klok bepalen:
  
  setCurrentTime()
  setHandColor()
  setHandLength()

Hulpprogrammas
  previewColor()
*/


void setCurrentTime() {
  tmElements_t tm;
  RTC.read(tm); 
  int  s  = tm.Second;
  int  m  = tm.Minute;
  bool pm = tm.Hour >= 12;
  int  u  = tm.Hour % 12;
  
  row(SEC_RING, 0, s);
  row(MIN_RING, 0, m);
  row(UUR_RING, 0, u*5);
  colors[179] = pm ? WARM : ON;
  klok.write(colors, LED_COUNT);
  
  char key = keypad.getKey();
  while( key != KEY_SET_TIME) {
    if (key) {
      switch (key) {
        case KEY_ANNULEER:
          allesAan(OFF);
          return;
        case KEY_SEC_MIN:
          if (s > 0) {
            s--;
          }
          break;
        case KEY_SEC_PLUS:
          if (s < 59) {
            s++;
          }
          break;
        case KEY_MIN_MIN:
          if (m > 0) {
            m--;
          }
          break;
        case KEY_MIN_PLUS:
          if (m < 59) {
            m++;
          }
          break;
        case KEY_UUR_MIN:
          if (u > 0) {
            u--;
          }
          break;
        case KEY_UUR_PLUS:
          if (u < 11) {
            u++;
          }
          break;
        case KEY_UUR_24:
          pm = !pm;
          break;  
      }
      row(SEC_RING, 0, s);
      row(MIN_RING, 0, m);
      row(UUR_RING, 0, u*5);
      colors[179] = pm ? WARM : ON;
      klok.write(colors, LED_COUNT);
    }  
    delay(10);
    key = keypad.getKey();
  }
  // sla nu de tijd op
  tm.Second = s;
  tm.Minute = m;
  tm.Hour   = pm ? u + 12 : u;

  RTC.write(tm);
  allesAan(OFF);
}

/*
    set date
*/
void setCurrentDate() {
  tmElements_t tm;
  RTC.read(tm); 
  char d = tm.Day;
  char m = tm.Month;
  int y  = tm.Year + YEAR_OFFSET;
  
  allesAan(OFF);
  showDate(d, m, y, 0);
  char key = keypad.getKey();
  while( key != KEY_SET_DATE) {
    if (key) {
      switch (key) {
        case KEY_ANNULEER:
          allesAan(OFF);
          return;
        case KEY_SEC_MIN:
          if (d > 1) {
            d--;
          }
          break;
        case KEY_SEC_PLUS:
          if (d < 31) {
            d++;
          }
          break;
        case KEY_MIN_MIN:
          if (m > 1) {
            m--;
          }
          break;
        case KEY_MIN_PLUS:
          if (m < 12) {
            m++;
          }
          break;
        case KEY_UUR_MIN:
          if (y > 2013) {
            y--;
          }
          break;
        case KEY_UUR_PLUS:
          if (y < 2099) {
            y++;
          }
          break;
      }
      showDate(d, m, y, 0);
    }  
    delay(10);
    key = keypad.getKey();
  }
  
 
  // sla nu de tijd op
  RTC.read(tm);
  tm.Day   = d;
  tm.Month = m;
  tm.Year  = y - YEAR_OFFSET;

  RTC.write(tm);
  allesAan(OFF);
}

void showDate(char d, char m, int y, char dimmValue) {
  digital(y / 1000       , 2  , dimmLed(VIOLET , dimmValue));
  digital((y / 100) % 10 , 8  , dimmLed(VIOLET , dimmValue));
  digital((y / 10) % 10  , 14 , dimmLed(VIOLET , dimmValue));
  digital(y % 10         , 20 , dimmLed(VIOLET , dimmValue));
  digital(d / 10         , 54 , dimmLed(BLUE   , dimmValue));
  digital(d % 10         , 48 , dimmLed(BLUE   , dimmValue));
  digital(m / 10         , 41 , dimmLed(PURPLE , dimmValue));
  digital(m % 10         , 35 , dimmLed(PURPLE , dimmValue));
  klok.write(colors, LED_COUNT);
}


void setHandColor() {
  char oldLmMode = lmMode;
  lmMode = LM_BLOCK_05;
  char key = keypad.getKey();  
  char numSec1    = 1;
  char numMin1    = 1;
  char numUur1    = 1;
  char numSec2    = 1;
  char numMin2    = 1;
  char numUur2    = 1;
  char numPendula = 1;
  char numTicks15 = 1;
  char numTicks05 = 1;
  previewColor();
  while( key != KEY_SET_COLOR) {
    if (key) {
      switch (key) {
        case KEY_ANNULEER:
          getColors();
          lmMode = oldLmMode;
          allesAan(OFF);
          return;
        case KEY_SEC_MIN:
          numSec1 = (numSec1 + 1) % COLORS;
          colorSec1 = rgbColorNr(numSec1);
          previewColor();
          break;
        case KEY_SEC_PLUS:
          numSec2 = (numSec2 + 1) % COLORS;
          colorSec2 = rgbColorNr(numSec2);
          previewColor();
          break;
        case KEY_MIN_MIN:
          numMin1 = (numMin1 + 1) % COLORS;
          colorMin1 = rgbColorNr(numMin1);
          previewColor();
          break;
        case KEY_MIN_PLUS:
          numMin2 = (numMin2 + 1) % COLORS;
          colorMin2 = rgbColorNr(numMin2);
          previewColor();
          break;
        case KEY_UUR_MIN:
          numUur1 = (numUur1 + 1) % COLORS;
          colorUur1 = rgbColorNr(numUur1);
          previewColor();
          break;
        case KEY_UUR_PLUS:
          numUur2 = (numUur2 + 1) % COLORS;
          colorUur2 = rgbColorNr(numUur2);
          previewColor();
          break;
        case KEY_SET_COLOR_PENDULA:
          numPendula = (numPendula + 1) % COLORS;
          colorPendula = rgbColorNr(numPendula);
          previewColor();
          break;
        case KEY_SET_COLOR_TICKS_15:
          numTicks15 = (numTicks15 + 1) % TIKCKS_KLEUREN_AANTAL;
          colorTicks15 = ticksKleur[numTicks15];
          previewColor();
          break;
        case KEY_SET_COLOR_TICKS_05:
          numTicks05 = (numTicks05 + 1) % TIKCKS_KLEUREN_AANTAL;
          colorTicks05 = ticksKleur[numTicks05];
          previewColor();
          break;
      }
    }  
    delay(10);
    key = keypad.getKey();
  }
  saveColor(E_COLOR_SEC1    , colorSec1   );
  saveColor(E_COLOR_MIN1    , colorMin1   );
  saveColor(E_COLOR_UUR1    , colorUur1   );
  saveColor(E_COLOR_SEC2    , colorSec2   );
  saveColor(E_COLOR_MIN2    , colorMin2   );
  saveColor(E_COLOR_UUR2    , colorUur2   );
  saveColor(E_COLOR_PENDULA , colorPendula);
  saveColor(E_COLOR_TICKS_15, colorTicks15);
  saveColor(E_COLOR_TICKS_05, colorTicks05);
  lmMode = oldLmMode;
  allesAan(OFF);
  initClock();
}

void saveColor(char eeprom, rgb_color color) {
  EEPROM.write(eeprom    ,  color.red  );
  EEPROM.write(eeprom + 1,  color.green);
  EEPROM.write(eeprom + 2,  color.blue );
}

void getColors() {
  colorSec1    = (rgb_color){EEPROM.read(E_COLOR_SEC1    ), EEPROM.read(E_COLOR_SEC1     + 1) , EEPROM.read(E_COLOR_SEC1     + 2)};
  colorMin1    = (rgb_color){EEPROM.read(E_COLOR_MIN1    ), EEPROM.read(E_COLOR_MIN1     + 1) , EEPROM.read(E_COLOR_MIN1     + 2)};
  colorUur1    = (rgb_color){EEPROM.read(E_COLOR_UUR1    ), EEPROM.read(E_COLOR_UUR1     + 1) , EEPROM.read(E_COLOR_UUR1     + 2)};
  colorSec2    = (rgb_color){EEPROM.read(E_COLOR_SEC2    ), EEPROM.read(E_COLOR_SEC2     + 1) , EEPROM.read(E_COLOR_SEC2     + 2)};
  colorMin2    = (rgb_color){EEPROM.read(E_COLOR_MIN2    ), EEPROM.read(E_COLOR_MIN2     + 1) , EEPROM.read(E_COLOR_MIN2     + 2)};
  colorUur2    = (rgb_color){EEPROM.read(E_COLOR_UUR2    ), EEPROM.read(E_COLOR_UUR2     + 1) , EEPROM.read(E_COLOR_UUR2     + 2)};
  colorPendula = (rgb_color){EEPROM.read(E_COLOR_PENDULA ), EEPROM.read(E_COLOR_PENDULA  + 1) , EEPROM.read(E_COLOR_PENDULA  + 2)};
  colorTicks15 = (rgb_color){EEPROM.read(E_COLOR_TICKS_15), EEPROM.read(E_COLOR_TICKS_15 + 1) , EEPROM.read(E_COLOR_TICKS_15 + 2)};
  colorTicks05 = (rgb_color){EEPROM.read(E_COLOR_TICKS_05), EEPROM.read(E_COLOR_TICKS_05 + 1) , EEPROM.read(E_COLOR_TICKS_05 + 2)};
}

void previewColor() {
  row(SEC_RING, 0, 10);
  row(MIN_RING, 0, 10);
  row(UUR_RING, 0, 10);
  colors[29] = colorPendula;
  colors[30] = colorPendula;
  colors[31] = colorPendula;
  setTick(45, dimmLed(colorTicks15, TICKS_DIMM));
  setTick(50, dimmLed(colorTicks05, TICKS_DIMM));
  setTick(55, dimmLed(colorTicks05, TICKS_DIMM));


  klok.write(colors,LED_COUNT);
}

void setHandLength() {
  showHandLength();
  char key = keypad.getKey();
  while( key != KEY_SET_HAND_LENGTH) {
    if (key) {
      switch (key) {
        case KEY_ANNULEER:
          allesAan(OFF);
          return;
        case KEY_SEC_MIN:
          if (SECS > 2) {
            SECS--;
            showHandLength();
          }
          break;
        case KEY_SEC_PLUS:
          if (SECS < 50) {
            SECS++;
            showHandLength();
          }
          break;
        case KEY_MIN_MIN:
          if (MINS > 2) {
            MINS--;
            showHandLength();
          }
          break;
        case KEY_MIN_PLUS:
          if (MINS < 50) {
            MINS++;
            showHandLength();
          }
          break;
        case KEY_UUR_MIN:
          if (HOURS > 2) {
            HOURS--;
            showHandLength();
          }
          break;
        case KEY_UUR_PLUS:
          if (HOURS < 50) {
            HOURS++;
            showHandLength();
          }
          break;
      }
    }  
    delay(10);
    key = keypad.getKey();
  }
  EEPROM.write(E_SECS,  SECS );
  EEPROM.write(E_MINS,  MINS );
  EEPROM.write(E_HOURS, HOURS);
  allesAan(OFF);
  initClock();
}

void showHandLength() {
    row(SEC_RING, 0, SECS);
    row(MIN_RING, 0, MINS);
    row(UUR_RING, 0, HOURS);
}




