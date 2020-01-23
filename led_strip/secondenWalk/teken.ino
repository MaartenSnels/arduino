/*

 routines om ledjes te tekenen (aan en uit zetten)
 
 row(int ring, int startLed, int stopLed)
 writeKlok()

*/


/**
* vul een van de ringen van start tot stop met een kleur.
*/
void row(int ring, int startLed, int stopLed) {
  rgb_color color1, color2;
  switch (ring) {
    case SEC_RING:
      color1 = colorSec;
      color2 = colorSec;
      break;
    case MIN_RING:
      color1 = colorMin;
      color2 = colorUur;
      break;
    case UUR_RING:
      color1 = colorUur;
      color2 = colorMin;
      break;
  }
  for (char i = 0; i < 60; i++) {
//    Serial.println(i);
    colors[ring * 60 + i] = i < startLed || i >= stopLed ? OFF : (i % 5 == 0 ? color1 : color2);
  }
  klok.write(colors,LED_COUNT);
}


/* 
   teken de klok
   routine verzort het dimmen en zet
   de ticks 
*/
void writeKlok() {
  rgb_color cSec = colorSec;
  rgb_color cMin = colorMin; 
  rgb_color cUur = colorUur;
  if (dimm>0) {
      cSec = (rgb_color) {cSec.red >> dimm, cSec.green >> dimm, cSec.blue >> dimm};
      cMin = (rgb_color) {cMin.red >> dimm, cMin.green >> dimm, cMin.blue >> dimm};
      cUur = (rgb_color) {cUur.red >> dimm, cUur.green >> dimm, cUur.blue >> dimm};
  }
  
  for(int i = SEC_START; i <= SEC_STOP; i++) {
    colors[i] = ledStatus[i] ? cSec : OFF;
  }
  for(int i = MIN_START; i <= MIN_STOP; i++) {
    colors[i] = ledStatus[i] ? (i % 5 != 0 ? cMin : cUur) : OFF;
  }
  for(int i = UUR_START; i <= UUR_STOP; i++) {
    colors[i] = ledStatus[i] ? (i % 5 != 0 ? cUur : cMin) : OFF;
  }

  setTicks();
  
  
  klok.write(colors, LED_COUNT);
}

/*

    zet de ticks (5 minute markers) van de klok
    Moet nog op verschillende manieren (elke vijf, elke 15 etc.
    Ook nog kijken of het voor of na twaalf is??
*/
void setTicks() {
  if (!ticks) {
    return;
  }
  for (int i = 0; i<= LED_COUNT; i++) {
    if (i%5==0 && !ledStatus[i]) {
      colors[i] = i%15==0 ? (dimm > TICKS_DIMM ? TICKS_VIJFTIEN_DIMM : TICKS_VIJFTIEN) : (dimm > TICKS_DIMM ? TICKS_VIJF_DIMM : TICKS_VIJF);
    }
  }
}


/*

  Zet alle lampjes aan in de opgegeven kleur. 

*/
void allesAan(rgb_color color) {
  for(int i = 0; i < LED_COUNT; i++) {
    colors[i] = color;
  }
  klok.write(colors,LED_COUNT);
}

