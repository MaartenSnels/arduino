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
  for (char i = 0; i < 60; i++) {
//    Serial.println(i);
    colors[ring * 60 + i] = i < startLed || i >= stopLed ? OFF : clockColor(ring, i);
  }
  klok.write(colors,LED_COUNT);
}

/*
  bepaal de kleur die het lampje moet hebben in de klok modus.
*/
rgb_color clockColor(int ring, int i) {
  rgb_color res;
  char stap = 0;
  rgb_color col1 = colorSec1;
  rgb_color col2 = colorSec2;
  switch (ring) {
    case MIN_RING:
      col1 = colorMin1;
      col2 = colorMin2;
      break;
    case UUR_RING:
      col1 = colorUur1;
      col2 = colorUur2;
      break;
  }
  
  switch (lmMode) {
    case LM_ONE_COLOR:
      res = col1;
      break;
    case LM_ONE_MINUTE:
      res = i % 5 != 0 ? col1 : col2;
      break;
    case LM_BLOCK_05:
      res = ((i / 5 ) % 2) == 0 ? col1 : col2;
      break;
    case LM_BLOCK_10:
      res = ((i / 10) % 2) == 0 ? col1 : col2;
      break;
    case LM_BLOCK_15:
      res = ((i / 15) % 2) == 0 ? col1 : col2;
      break;
    case LM_BLOCK_20:
      res = ((i / 20) % 2) == 0 ? col1 : col2;
      break;
    case LM_BLOCK_30:
      res = ((i / 30) % 2) == 0 ? col1 : col2;
      break;
    default:
      res = col1;
  }
  return dimmLed(res,dimm);
}


void row(int ring, int startLed, int stopLed, rgb_color color) {
  for (char i = 0; i < 60; i++) {
    colors[ring * 60 + i] = i < startLed || i > stopLed ? OFF : color;
  }
  klok.write(colors,LED_COUNT);
}


/* 
   teken de klok
   routine verzort het dimmen en zet
   de ticks 
*/
void writeKlok() {

  if (mode == ALJO              || 
      mode == TRAIL             || 
      mode == TRAIL_DOUBLE      || 
      mode == CLASSIC_VERVAAG   || 
      mode == CLASSIC )   {
    return;
  }
  
//  if (isPendulaOn()) {
//    setTicks();
//    pendula();
//    return;
//  }
  
  for(int i = 0; i < 60; i++) {
    colors[i + SEC_START] = ledStatus[i + SEC_START] ? clockColor(SEC_RING, i) : OFF;
    colors[i + MIN_START] = ledStatus[i + MIN_START] ? clockColor(MIN_RING, i) : OFF;
    colors[i + UUR_START] = ledStatus[i + UUR_START]  ? clockColor(UUR_RING, i) : OFF;
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
  if (ticks == TICKS_OFF) {
    return;
  }
  
  char ticksDimm = dimm + 2 > TICKS_MAX -1 ? TICKS_MAX - 1: dimm + 2;
  switch (ticks) {
    case TICKS_05_HALF:
    case TICKS_ALL_HALF:
    case TICKS_05:
      setTick(5,  dimmLed(colorTicks05, ticksDimm));
      setTick(10, dimmLed(colorTicks05, ticksDimm));
      setTick(20, dimmLed(colorTicks05, ticksDimm));
      setTick(25, dimmLed(colorTicks05, ticksDimm));
      setTick(35, dimmLed(colorTicks05, ticksDimm));
      setTick(40, dimmLed(colorTicks05, ticksDimm));
      setTick(50, dimmLed(colorTicks05, ticksDimm));
      setTick(55, dimmLed(colorTicks05, ticksDimm));
    case TICKS_15_HALF:
    case TICKS_15:
      setTick(15, dimmLed(colorTicks15, ticksDimm));
      setTick(45, dimmLed(colorTicks15, ticksDimm));
    case TICKS_30_HALF:
    case TICKS_30:
      setTick(30, dimmLed(colorTicks15, ticksDimm));
    case TICKS_60_HALF:
    case TICKS_60:
      setTick(0, dimmLed(colorTicks15, ticksDimm));
      break;
  }
  
  
}

/*
  ticks elke 15 minuten. Laat de tick weer zien als het eerste ledje voorbij de tick is. 
  TODO hoe zit dat in de FILL en DROP mode?
*/
void setTick(int index, rgb_color color) {
    if (tickOn(index)) {
      colors[index] = color;
    }
    if (tickOn(index + 60)) {
      colors[index + 60] = color;
    }
    if (tickOn(index + 120)) {
      colors[index + 120] = color;
    }
}


/*
  true als tick lampje aan mag staan
  kan ook zijn dat de mode het niet mogelijk maakt
*/
boolean tickOn(int index) {
  if (index >= 60) {
    if (ticks == TICKS_05_HALF && index % 15 != 0) {
      return false;
    }
    if (ticks == TICKS_ALL_HALF ||
        ticks == TICKS_60_HALF  ||
        ticks == TICKS_30_HALF  ||
        ticks == TICKS_15_HALF     ) {
      return false;
    }
  }
  
  int next = index+1;
  if (next==60)
    next=0;
  if (next==120)
    next =59;
  if (next==180)
    next = 119;
  return (!ledStatus[index] || ledStatus[next]);
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

/*
  set tick
*/
void setTickAlways(int index, rgb_color color) {
    colors[index] = color;
    colors[index + 60] = color;
    colors[index + 120] = color;
}

#define TIME_VERVAAG 20
#define VERVAAG_STAP  2
/*
    in een aantal stappen verlagen van de helderheid van de seconde
    tegelijkertijd die van de volgende seconde verhogen. Dit alles moet  precies een
    seconde duren.
    
    Er moet du begonnen worden dadat er een seconde is verstreken en dan moet de timer starten
*/
void vervaagClassicSeconde() {
  if (seconde == minuut) {
    return;
  }
  unsigned long mills = millis();
  if (oldMills > mills || oldMills + TIME_VERVAAG < mills) {
    fade(&colors[seconde + SEC_START].red,   VERVAAG_STAP);
    fade(&colors[seconde + SEC_START].green, VERVAAG_STAP);
    fade(&colors[seconde + SEC_START].blue,  VERVAAG_STAP);
    oldMills = millis();
    klok.write(colors, LED_COUNT);
  }
}

void pendula() {

  unsigned long mills = millis();
  if (oldMills > mills || oldMills + TIME_PENDULA < mills) {
    // ff dimmen
    for (char i = LEFT_PENDULA; i > LEFT_PENDULA - LENGTE_PENDULA; i--) {
      colors[i] = dimmLed(colors[i],1);
    }
    
    oldMills = mills;
    if (heenPendula) {
      posPendula++;
      if (posPendula >= LENGTE_PENDULA) {
        posPendula--;
        heenPendula = !heenPendula;
      }
    }
    else {
      posPendula--;
      if (posPendula < 0) {
        posPendula++;
        heenPendula = !heenPendula;
      }
    }
    colors[LEFT_PENDULA - posPendula] = dimmLed(colorPendula, dimm);
    klok.write(colors , LEFT_PENDULA + 1);
  }
}


// TODO mag je zover dimmen dat een kleur verdwijnt??
rgb_color dimmLed(rgb_color color, char amount) {
  if (amount == 0) {
    return color;
  }
  return (rgb_color) {color.red >> amount, color.green >> amount, color.blue >> amount};  
}

void dimmAll(char amount) {
  for(int i = 0; i < LED_COUNT; i++) {
    colors[i] = dimmLed(colors[i],amount);
  }
}

void dimmRing(char ring, char amount) {
  for(int i = 0; i < 60; i++) {
    colors[ring * 60 + i] = dimmLed(colors[ring * 60 + i] , amount);
  }
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

// bepaal of een ledje aan staat (NIET GELIJK AAN ledStatus[index] = true
boolean isOn(int index) {
  rgb_color c = colors[index];
  return c.red || c.green || c.blue; 
}

