/* LedStripColorTester: Example Arduino sketch that lets you
 * type in a color on a PC and see it on the LED strip.
 *
 * To use this, you will need to plug an Addressable RGB LED
 * strip from Pololu into pin 12.  After uploading the sketch,
 * select "Serial Monitor" from the "Tools" menu.  In the input
 * box, type a color and press enter.
 *
 * The format of the color should be "R,G,B!" where R, G, and B
 * are numbers between 0 and 255 representing the brightnesses
 * of the red, green, and blue components respectively.
 *
 * For example, to get green, you could type:
 *   40,100,0!
 *
 * You can leave off the exclamation point if you change the
 * Serial Monitor's line ending setting to be "Newline" instead
 * of "No line ending".
 *
 * Please note that this sketch only transmits colors to the
 * LED strip after it receives them from the computer, so if
 * the LED strip loses power it will be off until you resend
 * the color.
 */
 
#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<13> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 60           // aantal leds dat gebruikt wordt
#define STEP       2           // helderheid wordt elke keer met STEP procent verhoogd of verlaagd
rgb_color colors[LED_COUNT];
  
rgb_color   ON     = { 255, 255, 255};
rgb_color   OFF    = {   0,   0,   0};
rgb_color   red    = { 255,   0,   0};
rgb_color   blue   = {   0,   0, 255};
rgb_color   green  = {   0, 255,   0};
rgb_color   yellow = { 255, 255,   0};
int helderheid = 0;
int richting   = 1;

void setup()
{
  Serial.begin(250000);
}

void loop()
{

  kleurDimmer();
  delay(100);

//  helderheid += STEP * richting;
//  if (helderheid > 100) {
//    helderheid = 100;
//    richting *= -1;  
//  } else if (helderheid < 0) {
//    helderheid = 0;
//    richting *= -1;
//  }
//  kleurRed(helderheid);
//  kleurBlue(100 - helderheid);
//  int _delay = analogRead(A4) / 6;
//  delay(_delay);
//  Serial.print("delay: ");
//  Serial.println(_delay);

}

void kleurDimmer() {
  float input = analogRead(A4);
  int   leds       = (input * LED_COUNT * 1.0) / 1023.0;

  for (int i = 0; i <= leds; i++) {
    float helderheid = (i * 1.0      ) /   (LED_COUNT * 1.0);
    float r = 255.0 * helderheid;
    float g = 255.0 * (1.0 -  helderheid);
    rgb_color color = {r, g, 0};
    colors[i] = color; 
  }
  for (int i = leds + 1; i < LED_COUNT; i++) {
    colors[i] = OFF;
  }
  ledStrip.write(colors,LED_COUNT);

  Serial.print(input);
  Serial.print("\t--> ");
  Serial.println(leds);
}

void dimmer() {
  float input = analogRead(A4);
  int   helderheid = (input * 100.0    ) / 1023.0;
  int   leds       = (input * LED_COUNT) / 1023.0;
  setColor(0       , leds     , 1, ON , 100 - helderheid);  
  setColor(leds + 1, LED_COUNT, 1, OFF);  
  Serial.print(input);
  Serial.print("\t--> ");
  Serial.println(helderheid);
}

void kleurRed(int helderheid) {
  setColor(0,LED_COUNT,2,red, helderheid);
}

void kleurBlue(int helderheid) {
  setColor(1,LED_COUNT,2,blue, helderheid);
}

void setColor(int start, int stop, int step, rgb_color color, int helderheid) {
  int newRed   = color.red   * 1.0 * helderheid / 100.0;
  int newGreen = color.green * 1.0 * helderheid / 100.0;
  int newBlue  = color.blue  * 1.0 * helderheid / 100.0;
  rgb_color newColor =  {newRed,newGreen,newBlue};
  setColor(start, stop, step, newColor);
}

void setColor(int start, int stop, int step, rgb_color color) {
    for(uint16_t i = start; i <= stop; i+=step)
    {
      colors[i] = color;
    }
    ledStrip.write(colors, LED_COUNT);  
}
