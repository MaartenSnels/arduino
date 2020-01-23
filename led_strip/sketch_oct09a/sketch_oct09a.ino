#include <Wire.h>

#include <Adafruit_NeoPixel.h>
#include <DS3231RTC.h>
#include <Time.h>

#define PIN 13

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

uint32_t OFF     = strip.Color( 0   , 0   , 0   );
uint32_t RED     = strip.Color( 255 , 0   , 0   );
uint32_t BLUE    = strip.Color( 0   , 0   , 255 );
uint32_t GREEN   = strip.Color( 0   , 255 , 0   );
uint32_t YELLOW  = strip.Color( 255 , 255 , 0   );
uint32_t WHITE   = strip.Color( 255 , 255 , 255 );

uint16_t MAX_BRIGHTNESS = 255;

void setup() {
  strip.begin();
  
  strip.setBrightness(MAX_BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'
  
  randomSeed(analogRead(0)); //voor bepalen ramnom getallen
  
}

void loop() {

//  tmElements_t tm;
//  RTC.read(tm);
//
//
//  int uur = tm.Hour;
//  int minuut = tm.Minute;
//  int seconde = tm.Second;
//  float temp = RTC.getTemp();
//
//   int wait = 2000;
//   setTemperatuur(temp);
//   delay(wait);
//   setMinuut(minuut);
//   delay(wait);
//   setSeconde(seconde);
//   delay(wait);

  
  switch(random(11,12)) {
    case 0:
      effect0();
      break;
    case 1:
      effect1();
      break;
    case 2:
      effect2();
      break;
    case 3:
      effect3();
      break;
    case 4:
      effect4();
      break;
    case 5:
      effect5();
      break;
    case 6:
      effect6();
      break;
    case 7:
      effect7();
      break;
    case 8:
      effect8();
      break;
    case 9:
      effect9();
      break;
    case 10:
      effect10(); // random fill
      break;
    case 11:
      fastRun();
      break;
      
  }  
}
boolean redFast = true;
int redFastCount = 0;
void fastRun() {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i,redFast?RED:GREEN);
    strip.show();
    delay(1);
    strip.setPixelColor(i,OFF);
  }
  if (++redFastCount%100==0) {
    redFast = !redFast;
    redFastCount=0;
  }
}

// KLOK ROUTINES
void setMinuut(int tijd) {
  setColor(OFF);
  for(int i=0; i < tijd; i++) {
    if(i%5==0) {
      strip.setPixelColor(i,RED);
    } 
    else {
      strip.setPixelColor(i,GREEN);
    }
  }
  strip.show();
}
void setSeconde(int tijd) {
  setColor(OFF);
  for(int i=0; i < tijd; i++) {
    if(i%5==0) {
      strip.setPixelColor(i,YELLOW);
    } 
    else {
      strip.setPixelColor(i,YELLOW);
    }
  }
  strip.show();
}

void setTemperatuur(float temp) {
  int roundedTemp = (temp + 0.5);

    setColor(OFF);
    for (int i = 0; i <= roundedTemp && i < 16; i++) {
        strip.setPixelColor(i,BLUE);
    }
    for (int i = 16; i <= roundedTemp && i < 18; i++) {
        strip.setPixelColor(i,YELLOW);
    }
    for (int i = 18; i <= roundedTemp && i < 22; i++) {
        strip.setPixelColor(i,GREEN);
    }
    for (int i = 22; i <= roundedTemp && i < strip.numPixels(); i++) {
        strip.setPixelColor(i,RED);
    }
    strip.show();
}




// random color
uint32_t randomColor() {
  return strip.Color(random(256),random(256),random(256));
}

uint32_t randomPrimaryColor() {
    switch(random(7)) {
      case 0:
        return RED;
      case 1:
        return GREEN;
      case 2:
        return BLUE;
      case 3:
        return YELLOW;
      case 4:
        return RED;
      case 5:
        return WHITE;
      case 6: //orange
        return strip.Color(255,165,0);
    }

}

// EFFECTS
void effect0() {
  uint32_t c1=randomPrimaryColor();
  uint32_t c2=randomPrimaryColor();
  
  int wait = random(10,50);
  strip.clear();
  strip.show();
  
  pingPong(c1,c2,wait);
  pingPong(c2,c1,wait);
}

void effect1() {
  int delayOverlap = 200;
  gradient(  1,   -1,   0, 255, delayOverlap);
  gradient(  1,   -1,   0, 127, delayOverlap);
  gradient(  1,   -1,   0,   0, delayOverlap);
  gradient(  1,    0,  -1, 255, delayOverlap);
  gradient(  1,    0,  -1, 127, delayOverlap);
  gradient(  1,    0,  -1,   0, delayOverlap);
  gradient(  0,    1,  -1, 255, delayOverlap);
  gradient(  0,    1,  -1, 127, delayOverlap);
  gradient(  0,    1,  -1,   0, delayOverlap);
  
}

void effect2() {
  onOff(RED,    10);
  onOff(GREEN,  10);
  onOff(BLUE,   10);
  onOff(WHITE,  10);
  onOff(YELLOW, 10);
}

void effect3() {
  for(int i = 0; i<10; i++) {
    uren(250);
    minuten(250);
    seconden(250);
  }
}

void effect4() {
  theaterChase(RED, 50);
  theaterChase(GREEN, 50);
  theaterChase(BLUE, 50);
  theaterChase(WHITE, 50);
  theaterChase(YELLOW,50);
}

void effect5() {
  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

void effect6() {
  int delayWipe = 50;
  colorWipe(RED   , delayWipe);
  colorWipe(GREEN , delayWipe);
  colorWipe(BLUE  , delayWipe);
  colorWipe(WHITE , delayWipe);
  colorWipe(YELLOW, delayWipe);
}

void effect7() {
  int delayWalk = 75;
  colorWalk(true,  RED   , delayWalk);
  colorWalk(false, RED   , delayWalk);
  colorWalk(true,  GREEN , delayWalk);
  colorWalk(false, GREEN , delayWalk);
  colorWalk(true,  BLUE  , delayWalk);
  colorWalk(false, BLUE  , delayWalk);
  colorWalk(true,  WHITE , delayWalk);
  colorWalk(false, WHITE , delayWalk);
  colorWalk(true,  YELLOW, delayWalk);
  colorWalk(false, YELLOW, delayWalk);
}

void effect8() {
  int delayWalk = 75;
  twoColorWalk(true,  RED   , GREEN, delayWalk);
  twoColorWalk(false, GREEN , RED, delayWalk);
  twoColorWalk(true,  YELLOW   , BLUE, delayWalk);
  twoColorWalk(false, BLUE , YELLOW, delayWalk);
  twoColorWalk(true,  OFF   , WHITE, delayWalk);
  twoColorWalk(false, WHITE , OFF, delayWalk);
}

void effect9() {
  for(int i = 0; i<10; i++) {
    twoColors(2, BLUE,YELLOW,200);
    twoColors(2, YELLOW, BLUE, 200);
  }
}

void effect10() {
  setColor(OFF);
  fill(randomColor(),random(5,25));
}

// tekenen van kleurtjes e.d.

void fill(uint32_t c, int wait) {
  int stop = 0;
  int max = strip.numPixels();
  strip.clear();
  for (int j = 0; j <= max; j++) { 
    for (int i = max; i > stop; i--) {
      strip.setPixelColor(i,c);
      strip.show();
      delay(wait);
      strip.setPixelColor(i,OFF);
    }
    strip.setPixelColor(stop,c);
    stop++; 
  }
}

void pingPong(uint32_t c1, uint32_t c2, int wait) {
  int max = strip.numPixels();
  for(int i = 0; i < max; i++) {
    strip.setPixelColor(i,c1);
    strip.setPixelColor(max-i-1,c2);
    strip.show();
    delay(wait);
    strip.setPixelColor(i,OFF);
    strip.setPixelColor(max-i-1,OFF);
  }


}


/**
    multiplier = 0, 1 of -1
    offest tussen 0 en 256 als multiplier = 0
    
**/
void gradient(int multiplierR, int multiplierG, int multiplierB, int offset, int wait){
  //uint8_t brightness = strip.getBrightness();
  //strip.setBrightness(255);
  int colorStep = 255/strip.numPixels();
  for(int i = 0; i < strip.numPixels(); i++) {
    int step=i*colorStep;
    uint32_t r = multiplierR > 0 ? step : (multiplierR < 0 ? 255 - step : offset);
    uint32_t g = multiplierG > 0 ? step : (multiplierG < 0 ? 255 - step : offset);
    uint32_t b = multiplierB > 0 ? step : (multiplierB < 0 ? 255 - step : offset);
    strip.setPixelColor(i,strip.Color(r,g,b));
  }   
  strip.show();
  delay(wait);
  //strip.setBrightness(brightness);
}

void verloop() {
  uint8_t brightness = strip.getBrightness();
  strip.setBrightness(255);
  boolean green=false,blue=false;
  for(int i=0; i < 256; i+=25) {
    green=!green;
    for(int j=0; j < 256; j+=25) {
      blue=!blue;
      for(int k=0; k < 256; k+=25) {
        setColor(strip.Color(i,green?j:255-j,blue?k:255-k));
      }
    }
  }
  strip.setBrightness(brightness);
}

// alles een kleur, langzaam oplichten
void onOff(uint32_t color, uint8_t wait) {
  uint8_t brightness = strip.getBrightness();
  for(int i=0;i<=MAX_BRIGHTNESS;i++) {
    strip.setBrightness(i);
    setColor(color);
    delay(wait);  
  }
  for(int i=MAX_BRIGHTNESS;i>=0;i--) {
    strip.setBrightness(i);
    setColor(color);
    delay(wait);  
  }
  strip.setBrightness(brightness);
  
}

// single bit with a color
void colorWalk(boolean startLeft, uint32_t c, uint8_t wait) {
  int startLed = 0;
  int stopLed  = strip.numPixels();
  int stepLed  = 1;
  if (!startLeft) {
     startLed = strip.numPixels() - 1;
     stopLed = -1;
     stepLed = -1;
  }
  for(uint16_t i = startLed; i< stopLed; i+= stepLed) {
      strip.setPixelColor(i, c);
      strip.show();
      strip.setPixelColor(i, OFF);
      delay(wait);
  }
}

void twoColors(int steps, uint32_t c1, uint32_t c2, int wait) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if (i%steps == 0) {
        strip.setPixelColor(i,c1);
      } else {
        strip.setPixelColor(i,c2);
      }
    }
    strip.show();
    delay(wait);
}

// single bit with a color
void twoColorWalk(boolean startLeft, uint32_t c, uint32_t backGround, uint8_t wait) {
  setColor(backGround);
  int startLed = 0;
  int stopLed  = strip.numPixels();
  int stepLed  = 1;
  if (!startLeft) {
     startLed = strip.numPixels() - 1;
     stopLed = -1;
     stepLed = -1;
  }
  for(uint16_t i = startLed; i< stopLed; i+= stepLed) {
      strip.setPixelColor(i, c);
      strip.show();
      strip.setPixelColor(i, backGround);
      delay(wait);
  }
}

void uren(int wait) {
  twoColors(5,RED,GREEN,wait);
}

void minuten(int wait) {
  twoColors(5,GREEN, RED, wait);
}

void seconden(int wait) {
    setColor(YELLOW);
    delay(wait);
}

void setColor(uint32_t color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
  }
  strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
      
      strip.clear();     
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        strip.clear();
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


