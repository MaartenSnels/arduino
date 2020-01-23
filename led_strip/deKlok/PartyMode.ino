/*

    Hier party's definieren. 
    Mogelijk om ze zelf te keizen of ramdom

*/
#define SHIMMER_LOOPS             300  // duur van shimmer
#define GRADIENT_LOOPS            300  //
#define EXPLOSION_LOOPS           630  //
#define TRADITIONALCOLORS_LOOPS   600  //
#define RANDOMCOLORWALK_LOOPS     400  //
#define BRIGHTTWINKEL_LOOPS       800  //
#define MAX_LAMP_DUUR              10  //    Aantal seconden dat de lamp maximaal aan mag staan 

#define WHIPE                          1
#define RADAR                          2
#define SHIMMER                        3
#define GRADIENT                       4
#define EXPLOSION                      5
#define COLLISION                      6
#define TRADITIONAL_COLORS             7
#define RANDOM_COLOR_WALK              8
#define BRIGHT_TWINKLE                 9 
#define RACE                          10 
#define PARTIES                       11

void runParty(int maxParties) {
  int parties = 0;
  while (parties++ < maxParties) {
    switch(random(PARTIES)) {
    case WHIPE:
      if (!runWhipe(5))
        return;
      break;
    case RADAR:
      if (!runRadar(5, GREEN))
        return;
      break;
    case SHIMMER:
      if (!runShimmer())
        return;
      break;
    case GRADIENT:
      if (!runGradient())
        return;
      break;
    case EXPLOSION:
      if (!runExplosion())
        return;
      break;
    case COLLISION:
      if (!runCollision())
        return;
      break;
    case TRADITIONAL_COLORS:
      if (!runTraditionalColors())
        return;
      break;
    case RANDOM_COLOR_WALK:
      if (!runRandomColorWalk())
        return; 
      break;
    case BRIGHT_TWINKLE:
      if (!runBrightTwinkle())
        return;
      break;
    case RACE:
      if (!doRace())
        return;
      break;
    }
    reRandomnize(0); 
  }
//  lamp();
}


bool runBrightTwinkle() {
  allesAan(OFF);
  int loopCount = 0;
  while(loopCount < BRIGHTTWINKEL_LOOPS) {
    if (keypad.getKey()) {
      return false;
    }
    loopCount++;
    // random LEDs light up brightly and fade away; it is a very similar
    // algorithm to colorExplosion (just no radiating outward from the
    // LEDs that light up); as time goes on, allow progressively more
    // colors, halting generation of new twinkles for last 100 counts.
    if (loopCount < BRIGHTTWINKEL_LOOPS / 4)
    {
      brightTwinkle(0, 1, 0);  // only white for first 400 loopCounts
    }
    else if (loopCount < BRIGHTTWINKEL_LOOPS * 2 / 4)
    {
      brightTwinkle(0, 2, 0);  // white and red for next 250 counts
    }
    else if (loopCount < BRIGHTTWINKEL_LOOPS * 3 / 4)
    {
      brightTwinkle(1, 2, 0);  // red, and green for next 250 counts
    }
    else
    {
      // red, green, blue, cyan, magenta, yellow for the rest of the time
      brightTwinkle(1, 6, loopCount > BRIGHTTWINKEL_LOOPS - 100);
    }
    klok.write(colors, LED_COUNT);
  }
  allesAan(OFF);
  return true;
}



bool runRandomColorWalk() {
  allesAan(OFF);
  int loopCount = 0;
  while(loopCount < RANDOMCOLORWALK_LOOPS) {
    if (keypad.getKey()) {
      return false;
    }
    loopCount++;
    // start with alternating red and green colors that randomly walk
    // to other colors for 400 loopCounts, fading over last 80
    randomColorWalk(loopCount == 1 ? 1 : 0, loopCount > RANDOMCOLORWALK_LOOPS - 80);
    klok.write(colors, LED_COUNT);
  }
  allesAan(OFF);
  return true;
}


bool runTraditionalColors() {
  allesAan(OFF);
  int loopCount = 0;
  while(loopCount < TRADITIONALCOLORS_LOOPS) {
    if (keypad.getKey()) {
      return false;
    }
    reRandomnize(loopCount);
    loopCount++;
    traditionalColors(loopCount);
    klok.write(colors, LED_COUNT);
  }
  allesAan(OFF);
  return true;
}


void reRandomnize(int loopCount) {
  // for these two patterns, we want to make sure we get the same
  // random sequence six times in a row (this provides smoother
  // random fluctuations in brightness/color)
  if (loopCount % 6 == 0)
  {
    seed = random(30000);
  }
  randomSeed(seed);
}



bool runCollision() {

  allesAan(OFF);
  int loopCount = 0;
  while(!collision(loopCount)) {
    if (keypad.getKey()) {
      return false;
    }
    loopCount++;
    klok.write(colors, LED_COUNT);
  }
  allesAan(OFF);
  return true;
}

bool runExplosion() {
  allesAan(OFF);
  int loopCount = 0;
  while(loopCount < EXPLOSION_LOOPS) {
    if (keypad.getKey()) {
      return false;
    }
    loopCount++;
    // bursts of random color that radiate outwards from random points
    // for 630 loop counts; no burst generation for the last 70 counts
    // of every 200 count cycle or over the over final 100 counts
    // (this creates a repeating bloom/decay effect)
    colorExplosion(loopCount, (loopCount % 200 > 130) || (loopCount > EXPLOSION_LOOPS - 100));
    klok.write(colors, LED_COUNT);
  }
  allesAan(OFF);
  return true;
}

bool runGradient() {
  allesAan(OFF);
  int loopCount = 0;
  while(loopCount < GRADIENT_LOOPS) {
    if (keypad.getKey()) {
      return false;
    }
    loopCount++;
    gradient(loopCount);
    klok.write(colors, LED_COUNT);
  }
  allesAan(OFF);
  return true;
}

bool runShimmer() {
  allesAan(OFF);
  int loopCount = 0;
  while(loopCount < SHIMMER_LOOPS) {
    if (keypad.getKey()) {
      return false;
    }
    reRandomnize(loopCount);
    loopCount++;
    warmWhiteShimmer(loopCount > SHIMMER_LOOPS - 50);
    klok.write(colors, LED_COUNT);
  }
  allesAan(OFF);
  return true;
}

bool runRace() {
  unsigned char mills = 0;
  unsigned char secs = 3;
  allesAan(OFF);
  digital(secs, 43, RED);
  colors[0]   = RED;
  colors[60]  = GREEN;
  colors[120] = BLUE;
  klok.write(colors,LED_COUNT);
  while(secs > 0) {
    if (keypad.getKey()) {
      return false;
    }
    delay(10);
    if (mills++ > 100) {
      secs--;
      mills=0;
      digital(secs, 43, RED);
      klok.write(colors,LED_COUNT);
    }
  }
  allesAan(OFF);
  return doRace();
}

bool doRace() {
  char winner = -1;
  unsigned char pos[3] = {0, 0, 0};
  while (winner < 0) {
    if (keypad.getKey()) {
      return false;
    }
    char stap = random(3);
    pos[stap] = pos[stap] + 1;
    dimmRing(stap,1);
    colors[pos[0]      ] = RED;
    colors[pos[1] +  60] = GREEN;
    colors[pos[2] + 120] = BLUE;
    klok.write(colors,LED_COUNT);
    delay(50);
    for (char i = 0; i < 3; i++) {
      if (pos[i] == 59) {
        winner = i;
      }
    }
  }
  for (char i = 0; i < 5; i++) {
    row(winner, 0, 59, winner == 0 ? RED : (winner == 1 ? GREEN : BLUE));
    klok.write(colors, LED_COUNT);
    delay(150);
    for (char j = 0; j < 3; j++) {
      dimmRing(winner, 1);
      klok.write(colors, LED_COUNT);
      delay(150);
    }
  }
  
  return true;
}

bool lamp() {
  unsigned char mills = 0;
  unsigned int secs = 0;
  allesAan(WHITE);
  while(secs <= MAX_LAMP_DUUR) {
    if (keypad.getKey()) {
      return false;
    }
    delay(10);
    if (mills++ > 100) {
      secs++;
      mills=0;
    }
  }
  allesAan(OFF);
  
  return true;
}


bool runWhipe (int loops) {
  char c1 = random(TIKCKS_KLEUREN_AANTAL);
  char c2 = random(TIKCKS_KLEUREN_AANTAL);
  while (c1 ==c2) {
    c2 = random(TIKCKS_KLEUREN_AANTAL);
  }
  
  rgb_color color1 = ticksKleur[c1];
  rgb_color color2 = ticksKleur[c2];
  int loopCount = 0;
  while(loopCount < loops) {
    if (keypad.getKey()) {
      return false;
    }
    loopCount++;
    if (!whipeOn(color1))
      return false;
    if (!whipeOn(color2))
      return false;
 
  }
  allesAan(OFF);
  return true;

}


bool whipeOn(rgb_color color) {
  char i;
  for ( i = 0;  i < 60 && !keypad.getKey(); i++ ) {
    setTickAlways(i,color);
    klok.write(colors, LED_COUNT);
  }
  return i>=60;
}

bool runRadar(int loops, rgb_color color) {
  char i;
  int hit = random(180);
  rgb_color colorHit = BLUE;
  bool isHit = false;
  for (int j = 0; j < loops; j++) {
    for ( i = 0;  i < 60 && !keypad.getKey(); i++ ) {
      dimmAll(1);
      setTickAlways(i,color);
      if (hit % 60 == i) {
        isHit = true;
        hit = (hit + 179) % 180;
        colorHit = BLUE;
      }
      if (isHit) {
        colors[hit] = colorHit;
        fade(&colors[hit].red, 4);
        fade(&colors[hit].green, 4);
        fade(&colors[hit].blue, 4);
        colorHit = colors[hit];
      }
      klok.write(colors, LED_COUNT);
      delay(30);
    }
  }
  return i>=60;
}

// IDEEEN VOOR PARTYIES


// ERASE: in twee of drie rondjes alles dimmen met een ronddraaiend gummetje
// vervagend rondje in 30 stappen




