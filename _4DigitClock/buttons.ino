#define DELAY_BTN_PRESS      200  // delay als je knop ingedrukt houdt
#define DELAY_BTN_PRESS_FAST 100  // delay als je knop ingedrukt houdt, na een tijdje
#define BACK_SECONDS           10 // terug na aantal seconde geen knop indrukken
#define MIN_YEAR               17 // minimale jaartal
#define MAX_YEAR               40 // maximale jaartal

#define TICK                  200 // wachten voor eerste knop druk
#define TICKS_TIME             10 // 10 * 200 ms = 2 sec
#define TICKS_DATE             20 // 20 * 200 ms = 4 sec
#define TICKS_SETTINGS         30 // 30 * 200 ms = 6 sec
#define TICKS_TEST             40 // 40 * 200 ms = 8 sec

#define TICKS_TEMP             10 // 10 * 200 ms = 2 sec
#define TICKS_SHOW_DATE        20 // 20 * 200 ms = 4 sec
#define TICKS_SEC              30 // 30 * 200 ms = 6 sec
#define TICKS_RANDOM           40 // 40 * 200 ms = 8 sec

#define DELAY_SHOW           5000 // hoe lang moet datum of tijd getoond worden

void handleButton1Pressed() {
  Serial.println("handelButton1Pressed");
  writeTime = true;
  mayShowSeconds   = false;
  int btn = 0;
  while (!digitalRead(BTN_1)) {
    btn++; 
    if (btn == TICKS_TIME) {
      tekenPatroon(EEN , T);
      tekenPatroon(TWEE, I);
      tekenPatroon(DRIE, M);
      tekenPatroon(VIER, E);
    }
    if (btn == TICKS_DATE) {
      tekenPatroon(EEN , D);
      tekenPatroon(TWEE, A);
      tekenPatroon(DRIE, T);
      tekenPatroon(VIER, E);
    }
    if (btn == TICKS_SETTINGS) {
      tekenPatroon(EEN , S);
      tekenPatroon(TWEE, E);
      tekenPatroon(DRIE, T);
      tekenPatroon(VIER, LEEG);
    }
    if (btn == TICKS_TEST) {
      tekenPatroon(EEN , T);
      tekenPatroon(TWEE, E);
      tekenPatroon(DRIE, S);
      tekenPatroon(VIER, T);
    }
    delay(TICK);
  }

  // 
  if (btn > TICKS_TIME && btn < TICKS_DATE) {
    adjustTime();
    return;
  }
  if (btn >= TICKS_DATE && btn < TICKS_SETTINGS) {
    adjustDate();
    return;
  }
  if (btn >= TICKS_SETTINGS && btn < TICKS_TEST) {
    adjustSettings();
    return;
  }
  if (btn >= TICKS_TEST) {
//    test();
    return;
  }
}

void handleButton2Pressed() {
  Serial.println("handelButton2Pressed");
  writeTime = true;
  mayShowSeconds   = false;
  int btn = 0;
  while (!digitalRead(BTN_2)) {
    btn++;

    if (btn == TICKS_TEMP) {
      tekenPatroon(EEN , T);
      tekenPatroon(TWEE, E);
      tekenPatroon(DRIE, M);
      tekenPatroon(VIER, P);
    }
    if (btn == TICKS_SHOW_DATE) {
      tekenPatroon(EEN , D);
      tekenPatroon(TWEE, A);
      tekenPatroon(DRIE, T);
      tekenPatroon(VIER, E);
    }
    if (btn == TICKS_SEC) {
      tekenPatroon(EEN , S);
      tekenPatroon(TWEE, E);
      tekenPatroon(DRIE, C);
      tekenPatroon(VIER, LEEG);
    }
    if (btn == TICKS_RANDOM) {
      tekenPatroon(EEN , R);
      tekenPatroon(TWEE, A);
      tekenPatroon(DRIE, N);
      tekenPatroon(VIER, D);
    }
    delay(TICK);
  }

  // 
  if (btn > TICKS_TEMP && btn < TICKS_SHOW_DATE) {
    showTemp();
    delay(DELAY_SHOW);
    return;
  }
  if (btn >= TICKS_SHOW_DATE && btn < TICKS_SEC) {
    showDate();
    return;
  }
  if (btn >= TICKS_SEC && btn < TICKS_RANDOM) {
    mayShowSeconds = true;
    return;
  }
  if (btn >= TICKS_RANDOM) {
    runTest = true;
//    while (runTest) {
//      randomLeds(30000, false);
//    }
    runTest = true;
    return;
  }
  if (btn > 0) {
    shutdownMatrix();
    return;
  }
}

void shutdownMatrix() {
  shutDownMode = !shutDownMode;
  digitalWrite(BTN_2_LED,shutDownMode);
  maxAll(max7219_reg_shutdown, !shutDownMode);
}

void adjustBrightness() {
  Serial.print("Set brightness to ");
  intensity = (intensity +2) % 16;
  Serial.println(intensity);
  maxAll(max7219_reg_intensity, intensity);
}

void setMode(byte mode, tmElements_t *time) {
  Serial.print("Set mode to ");
  Serial.println(mode);
  if (mode == ADJUST_NONE) {
    return;
  }
  leeg();
  int value = 0;
  switch(mode) {
    case ADJUST_HOUR:
      value = time->Hour;
      tekenPatroon(EEN , U);
      tekenPatroon(TWEE, U);
      break;
    case ADJUST_MINUTE:    
      value = time->Minute;
      tekenPatroon(EEN,  M);
      tekenPatroon(TWEE, I);
      break;
    case ADJUST_SECOND:
      value = time->Second;
      tekenPatroon(EEN,  S);
      tekenPatroon(TWEE, C);
      break;
    case ADJUST_YEAR:
      value = tmYearToCalendar(time->Year) % 100;
      tekenPatroon(EEN,  J);
      tekenPatroon(TWEE, R);
      break;
    case ADUSTT_MONTH:
      value = time->Month;
      tekenPatroon(EEN,  M);
      tekenPatroon(TWEE, A);
      break;
    case ADJUST_DAY:
      value = time->Day;
      tekenPatroon(EEN,  D);
      tekenPatroon(TWEE, A);
      break;
    case ADJUST_12_24:
      value = amPm ? 12 : 24;
      tekenPatroon(EEN, A);
      tekenPatroon(TWEE, P);
      break;
    case ADJUST_BRIGHTNESS:
      value = intensity;
      tekenPatroon(EEN, B);
      tekenPatroon(TWEE, R);
      break;
    case ADJUST_MODE:
      value = mode;
      tekenPatroon(EEN, M);
      tekenPatroon(TWEE, O);
  }
  tekenPatroon(DRIE, CIJFERS[value / 10]);
  tekenPatroon(VIER, CIJFERS[value % 10]);

}

byte daysInMonth(tmElements_t *time) {
  //simpele berekeing, gaat alleen voor jaren vanaf 2015 tot 2040)
  switch (time->Month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      return 31;
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;
    case 2: // schikkeljaar op 400 tallen genegeerd.
      if (time->Year % 4 == 0 && time->Year % 100 != 0) {
        return 29;
      }
      return 28;
  }
}

byte inceaseValue(byte mode, tmElements_t *time) {
  Serial.print("Mode ");
  Serial.print(mode);
  int value = 0;
  switch(mode) {
    case ADJUST_HOUR:
      value = (time->Hour + 1) % 24;
      time->Hour = value;
      break;
    case ADJUST_MINUTE:    
      value = (time->Minute + 1) % 60;
      time->Minute = value;
      break;
    case ADJUST_SECOND:
      value = (time->Second + 1) % 60;
      time->Second = value;
      break;
    case ADJUST_YEAR:
      value = tmYearToCalendar(time->Year) % 100 + 1;
      if (value < MIN_YEAR || value > MAX_YEAR) {
        value = MIN_YEAR;
      }
      time->Year = CalendarYrToTm(value + 2000);
      break;
    case ADUSTT_MONTH:
      value = time->Month + 1;
      if (value > 12) {
        value = 1;
      }
      time->Month = value;
      break;
    case ADJUST_DAY:
      value = time->Day + 1;
      if (value > daysInMonth(time)) {
        value = 1;
      }
      time->Day = value;
      break;
    case ADJUST_12_24:
      amPm = !amPm;
      value = amPm ? 12 : 24;
      break;
    case ADJUST_MODE:
      if (mode == MODE_NORMAL) {
        mode = MODE_SCROLL;
      } else {
        mode = MODE_NORMAL;
      }
      value = mode;
      break;
    case ADJUST_BRIGHTNESS:
      intensity = (intensity + 1) % 16;
      value = intensity;
      break;
  }
  tekenPatroon(DRIE, CIJFERS[value / 10]);
  tekenPatroon(VIER, CIJFERS[value % 10]);
  Serial.print(" new value = ");
  Serial.println(value);
  debugPrintTime(time);
}

void adjustDate() {
  doAdust(ADJUST_YEAR, ADJUST_DAY);
}

void adjustSettings() {
  
}

void adjustTime() {
  doAdust(ADJUST_HOUR, ADJUST_SECOND);
}

void doAdust(byte modeStart, byte modeStop) {
  Serial.println("Begin doAjust");
  /*
    Indrukken van btn_1 levert een nieuwe modus op. We beginnen nu met modus ADJUST_HOUR (uur)
    en we stoppen bij ADUST_END;
  */
  tmElements_t tm;
  RTC.read(tm); 
  
  int _delay = DELAY_BTN_PRESS;
  byte mode = modeStart;
  int notPressed = 0;
  int stopIfNotPressed = 1000 / _delay * BACK_SECONDS;
  
  setMode(mode, &tm); 
  while (mode != ADJUST_NONE && notPressed < stopIfNotPressed) {
    delay(_delay);
    if (!digitalRead(BTN_1)) {
      // switch mode
      mode++;
      if (mode > modeStop) {
        mode = ADJUST_NONE;
      }
      setMode(mode, &tm);
      notPressed = 0;
    }
    if (!digitalRead(BTN_2)) {
      byte getal = inceaseValue(mode, &tm);
      notPressed = 0;
    }
    notPressed++;
  }
  // nog ff een delay om te zorgen dat je niet van de laatste
  // meteen weer naar de eerste gaat
  delay(_delay);

  // op gewone manier instellingen verlaten, sla de tijd op.
  if ( notPressed < stopIfNotPressed) {
    setTijd(&tm);
  }
  Serial.println("Einde doAjust");
}

