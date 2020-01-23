void setHandWalk(char ring) {
  int pos, startPos, stopPos, lengthHand;
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
  int pos, startPos, stopPos;
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

void initClock() {
  tmElements_t tm;
  
  RTC.read(tm); 
  seconde = tm.Second;
  minuut = tm.Minute;
  echtUur = tm.Hour;
  uur = (echtUur % 12) * 5 + minuut / 12;
  pm = echtUur >= 12;
  dag = tm.Day;
  maand = tm.Month;
  jaar = tm.Year + YEAR_OFFSET;
  
  switch(mode) {
    case HANDS:
    case WALK:
      if (!isPendulaOn()) {
        initWalk(SEC_RING);
      }
      initWalk(MIN_RING);
      initWalk(UUR_RING);
      break;
    case FILL:
    case DROP:
      for (int i = 0; i < 60; i++) {
        ledStatus[i+SEC_START] = (i <= seconde && !pendulaOn);
      }
      for (int i = 0; i < 60; i++) {
        ledStatus[i+MIN_START] = (i <= minuut);
      }
      for (int i = 0; i < 60; i++) {
        ledStatus[i+UUR_START] = (i <= uur);
      }
      break;
    case ALJO:
      initDigital();
      break;  
    case TRAIL:
    case TRAIL_DOUBLE:
      initTrail();
      break;
    case CLASSIC:
    case CLASSIC_VERVAAG:
      initClassic();
      break;
  }
  writeKlok();
}

void initClassic() {
  for (int i = 0; i < LED_COUNT; i++){
    colors[i] = OFF;
  } 
  setTicks();
  // langste wijzer is seconcen
  colors[seconde + SEC_START] = dimmLed(colorSec1, dimm);
  colors[seconde + MIN_START] = dimmLed(colorSec1, dimm);
  colors[seconde + UUR_START] = dimmLed(colorSec1, dimm);
  // dan kom minuten (valt over secs heen)
  colors[minuut  + SEC_START] = dimmLed(colorMin1, dimm);
  colors[minuut  + MIN_START] = dimmLed(colorMin1, dimm);
  colors[minuut  + UUR_START] = dimmLed(colorMin1, dimm);
  // uren (valt over sec en mins heen)
  colors[uur     + MIN_START] = dimmLed(colorUur1, dimm);
  colors[uur     + UUR_START] = dimmLed(colorUur1, dimm);
  klok.write(colors,LED_COUNT);
}

void initTrail() {
  for (int i = 10; i > 0; i--) {
    if (!pendulaOn) {
      colors[((seconde - i + 60) % 60) + SEC_START] = dimmLed(colorSec1, dimm);
    }
    colors[((minuut  - i + 60) % 60) + MIN_START] = dimmLed(colorMin1, dimm);
    colors[((uur     - i + 60) % 60) + UUR_START] = dimmLed(colorUur1, dimm);
    if (mode == TRAIL_DOUBLE) {
      if (!pendulaOn) {
        colors[((seconde + i + 60) % 60) + SEC_START] = dimmLed(colorSec2, dimm);
      }
      colors[((minuut  + i + 60) % 60) + MIN_START] = dimmLed(colorMin2, dimm);
      colors[((uur     + i + 60) % 60) + UUR_START] = dimmLed(colorUur2, dimm);
    }
    dimmAll(1);
  }
  setTicks();
//  if (mode != TRAIL_DOUBLE) {
    if (!pendulaOn) {
      colors[seconde + SEC_START] = dimmLed(colorSec1, dimm);
    }
    colors[minuut  + MIN_START] = dimmLed(colorMin1, dimm);
    colors[uur     + UUR_START] = dimmLed(colorUur1, dimm);
//  }
  klok.write(colors,LED_COUNT);
}

void initDigital() {
  // vijf seconde de datum
  if (seconde >= DATE_ON && seconde < DATE_OFF) {
    if (seconde == DATE_ON) {
      allesAan(OFF);
      showDate(dag,maand,jaar, dimm);
    }    
    showDate(dag, maand, jaar, dimm);
    return;
  }
  
  // vijf seconde de temperatuur
  if (seconde >= TEMP_ON && seconde < TEMP_OFF) {
    if (seconde == TEMP_ON) {
      allesAan(OFF);
      writeTemp(dimmLed(RED,dimm));
    }    
    writeTemp(dimmLed(RED,dimm));
    return;
  }
  
  if (seconde == DATE_OFF || seconde == TEMP_OFF) {
    allesAan(OFF);
  }
  
  digital(echtUur / 10, 49, dimmLed(colorUur1, dimm));
  digital(echtUur % 10, 42, dimmLed(colorUur1, dimm));
  digital(minuut  / 10,  7, dimmLed(colorMin1, dimm));
  digital(minuut  % 10, 14, dimmLed(colorMin1, dimm));
  digital(seconde % 10, 22, dimmLed(colorSec1, dimm));
  digital(seconde / 10, 34, dimmLed(colorSec1, dimm));
  klok.write(colors, LED_COUNT);
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


