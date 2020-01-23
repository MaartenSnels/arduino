#include <PololuLedStrip.h>

#define LED_SIGNAL_PIN 13
#define LED_COUNT 60//509
PololuLedStrip<LED_SIGNAL_PIN> ledStrip;
rgb_color colors[LED_COUNT];
rgb_color RED = (rgb_color){255,0,0}; 
rgb_color GREEN = (rgb_color){0,255,0}; 
rgb_color BLUE = (rgb_color){0,0,255}; 
rgb_color OFF = (rgb_color){0,0,0}; 

void setup() {

}
boolean redFast = true;
int redFastCount = 0;

void loop() {
  fastLed(200);
}

void fastLed(int wait) {
  for(int i = redFast?1:0; i< LED_COUNT; i+=2) {
    colors[i] = RED;
  }
  for(int i = redFast?0:1; i< LED_COUNT; i+=2) {
    colors[i] = BLUE;
  }
  ledStrip.write(colors,LED_COUNT);
  delay(wait);
  
  if (++redFastCount%2==0) {
      redFast = !redFast;
      redFastCount=0;
  }
  
}
