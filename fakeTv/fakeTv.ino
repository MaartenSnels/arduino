
/*

An Arduino "Fake TV"
This is an Arduino simulation of the product called "Fake TV" 
It simulates the action of a TV being viewd by a person at night.
The purpose is to fool burglars into thinking someone is at home watching a TV at night.
Place it near a window behind blinds or curtains so as to not directly show the LEDs.
You only want the aggragate effect of all the LEDs to be seen.

I used 2 green, 2 red, one bright white and one bright blue.
I neede more red/green because the apparent brightness of the white and blue is greater than the red/greens
Use a diffuser over the board such as vellum papar to help scene quality.
Of course all these parameters can be changed to suit your needs.
Check at night near a window and adjust to suit your preferences.
Free to modify, the possible scene algorithms are endless.

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


#define BLUE    0
#define RED_1   1
#define GREEN_1 2
#define GREEN_2 3
#define RED_2   4
#define WHITE   5

#define LEDS    6

int blu  = BLUE;
int red2 = RED_1;
int grn1 = GREEN_1;
int grn2 = GREEN_2;
int red1 = RED_2;
int wht  = WHITE;
int randpwm = 0;

void setup() 
{   
  Serial.begin(250000);  
}

void loop() 
{
  for(int i=0;i<10;i++)  //play scene 1 multiple times
  {
    scene1();
  }
  if (random(2) == 1)  //Possibly call scene 2
  {
    scene2();
  }
  if (random(2) == 1)  //Possibly call scene 3
  {
    scene3();
  }
  if (random(2) == 1)  //Possibly call scene 4
  {
    scene4();
  }
  if (random(4) == 1)  //Possibly do a commercial break
  {
    commercial();
  }
}

/*
These are the main scene algorithms
*/

void scene1()
// Changes random light levels and linger-times 
// of all colors to simulate "normal" TV action
{
  randpwm = random(20,255);
  _analogWrite(red1,randpwm); 
  _analogWrite(red2,randpwm); 
  randpwm = random(20,255);
  _analogWrite(grn1,randpwm); 
  _analogWrite(grn2,randpwm); 
  _analogWrite(blu,random(10,225)); 
  _analogWrite(wht,random(10,175));
  delay(random(500,2000));
}

void scene2()
// Increases intensity of wht,blu (fade-in)
{
  delay(1000);
  for(int i=2;i<255;i++)
  {
    _analogWrite(blu,i); 
    _analogWrite(wht,i);
    delay(20);
  }
}

void scene3()
// Flickers wht,blu for a flickeriing scene effect
{
  boolean sw = HIGH;
  for(int i=0;i<30;i++)
  {
    digitalWrite(wht,sw);
    digitalWrite(blu,sw);
    sw = !sw;
    delay(random(50,300));
  }
}

void scene4()
// Changes red/grn light levels only
// wht/blu are off
{
  //don't use wht/blu
  digitalWrite(wht,LOW);
  digitalWrite(blu,LOW);
  for(int i=0;i<12;i++)
  {
    randpwm = random(20,255);
    _analogWrite(red1,randpwm); 
    _analogWrite(red2,randpwm); 
    randpwm = random(20,255);
    _analogWrite(grn1,randpwm); 
    _analogWrite(grn2,randpwm); 
    delay(random(200,2000));
  }
}

void commercial()
// Simulates a switch to or from a commercial break 
{
  _analogWrite(red1,2); 
  _analogWrite(red2,2); 
  _analogWrite(grn1,2);
  _analogWrite(grn2,2);
  _analogWrite(blu,0);
  _analogWrite(wht,0);
  delay(random(1000,2500));
}


void _analogWrite(int led, int helderheid) {
  rgb_color color;
  switch(led) {
    case RED_1:
    case RED_2:
      color = red;
      break;
    case GREEN_1:
    case GREEN_2:
      color = green;
      break;
    case BLUE:
      color = blue;
      break;
    case WHITE:
      color = ON;
      break;
  }

  setColor(led, color, helderheid);
}

void setColor(int led, rgb_color color, int helderheid) {
  float aantal = (helderheid *1.0) / 255.0;
  int newRed   = (color.red   * 1.0) * aantal;
  int newGreen = (color.green * 1.0) * aantal;
  int newBlue  = (color.blue  * 1.0) * aantal;
  rgb_color newColor =  {newRed,newGreen,newBlue};

  for (int i = led; i < LED_COUNT; i+=LEDS) {
    colors[i] = newColor;
  }
  ledStrip.write(colors, LED_COUNT);  
  Serial.println(aantal);
}

