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
  int s = tm.Second;
  int m = tm.Minute;
  int u = tm.Hour % 12;
  
  row(SEC_RING, 0, s+1);
  row(MIN_RING, 0, m+1);
  row(UUR_RING, 0, u*5+1);
  
  char key = keypad.getKey();
  while( key != KEY_SET_TIME) {
    if (key) {
      switch (key) {
        case KEY_ANNULEER:
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
      }
      row(SEC_RING, 0, s+1);
      row(MIN_RING, 0, m+1);
      row(UUR_RING, 0, u*5+1);
    }  
    delay(10);
    key = keypad.getKey();
  }
  // sla nu de tijd op
  tm.Second = s;
  tm.Minute = m;
  tm.Hour   = u;

  RTC.write(tm);
}

void setHandColor() {
  char key = keypad.getKey();
  char numSec = 1;
  char numMin = 1;
  char numUur = 1;
  previewColor();
  while( key != KEY_SET_COLOR) {
    if (key) {
      switch (key) {
        case KEY_ANNULEER:
          return;
        case KEY_SEC_MIN:
          if (numSec > 1) {
            numSec--;
            colorSec = rgbColorNr(numSec);
            previewColor();
          }
          break;
        case KEY_SEC_PLUS:
          if (numSec < COLORS) {
            numSec++;
            colorSec = rgbColorNr(numSec);
            previewColor();
          }
          break;
        case KEY_MIN_MIN:
          if (numMin > 1) {
            numMin--;
            colorMin = rgbColorNr(numMin);
            previewColor();
          }
          break;
        case KEY_MIN_PLUS:
          if (numMin < COLORS) {
            numMin++;
            colorMin = rgbColorNr(numMin);
            previewColor();
          }
          break;
        case KEY_UUR_MIN:
          if (numUur > 1) {
            numUur--;
            colorUur = rgbColorNr(numUur);
            previewColor();
          }
          break;
        case KEY_UUR_PLUS:
          if (numUur < COLORS) {
            numUur++;
            colorUur = rgbColorNr(numUur);
            previewColor();
          }
          break;
      }
    }  
    delay(10);
    key = keypad.getKey();
  }
  EEPROM.write(E_COLOR_SEC    ,  colorSec.red   );
  EEPROM.write(E_COLOR_SEC + 1,  colorSec.green );
  EEPROM.write(E_COLOR_SEC + 2,  colorSec.blue  );
  EEPROM.write(E_COLOR_MIN    ,  colorMin.red   );
  EEPROM.write(E_COLOR_MIN + 1,  colorMin.green );
  EEPROM.write(E_COLOR_MIN + 2,  colorMin.blue  );
  EEPROM.write(E_COLOR_UUR    ,  colorUur.red   );
  EEPROM.write(E_COLOR_UUR + 1,  colorUur.green );
  EEPROM.write(E_COLOR_UUR + 2,  colorUur.blue  );
  initClock();
}

void previewColor() {
  row(SEC_RING, 0, 10);
  row(MIN_RING, 0, 10);
  row(UUR_RING, 0, 10);
}

void setHandLength() {
  showMode();
  char key = keypad.getKey();
  while( key != KEY_SET_HAND_LENGTH) {
    if (key) {
      switch (key) {
        case KEY_ANNULEER:
          return;
        case KEY_SEC_MIN:
          if (SECS > 2) {
            SECS--;
            showMode();
          }
          break;
        case KEY_SEC_PLUS:
          if (SECS < 50) {
            SECS++;
            showMode();
          }
          break;
        case KEY_MIN_MIN:
          if (MINS > 2) {
            MINS--;
            showMode();
          }
          break;
        case KEY_MIN_PLUS:
          if (MINS < 50) {
            MINS++;
            showMode();
          }
          break;
        case KEY_UUR_MIN:
          if (HOURS > 2) {
            HOURS--;
            showMode();
          }
          break;
        case KEY_UUR_PLUS:
          if (HOURS < 50) {
            HOURS++;
            showMode();
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
  initClock();
}



