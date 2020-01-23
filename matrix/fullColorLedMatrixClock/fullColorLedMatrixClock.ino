/* LedStripRainbow: Example Arduino sketch that shows
 * how to make a moving rainbow pattern on an
 * Addressable RGB LED Strip from Pololu.
 *
 * To use this, you will need to plug an Addressable RGB LED
 * strip from Pololu into pin 12.  After uploading the sketch,
 * you should see a moving rainbow.
 */

#include <PololuLedStrip.h>

#define UIT rgb_color(0, 0, 0) // uit
#define AAN rgb_color(255, 255, 255) // uit

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

#define MATRIX_COUNT      4
#define ROWS              8
#define COLS              8
#define LEDS_PER_MATRIX   (ROWS * COLS)
#define LED_COUNT         (LEDS_PER_MATRIX * MATRIX_COUNT)
#define ENCODER_0_PIN_A   11
#define ENCODER_0_PIN_B   10

struct MatrixOn {
  boolean m[MATRIX_COUNT];
};

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[LED_COUNT];

////////// PARTY CONSTANTEN
volatile int _delay = 25;
volatile int dim = 32;
int aantal = 10;



void setup()
{
  Serial.begin(115200);
  Serial.println("init complete");
  Serial.print("Led count: ");
  Serial.println(LED_COUNT);
  randomSeed(analogRead(0));
  pinMode (ENCODER_0_PIN_A, INPUT);
  pinMode (ENCODER_0_PIN_B, INPUT);
  // de arduino due kun je rechtstreeks aanpsreken
//  attachInterrupt(digitalPinToInterrupt(ENCODER_0_PIN_A), handleEncoder0, CHANGE);
//  attachInterrupt(ENCODER_0_PIN_A, handleEncoder0, CHANGE);
  Serial.println("setup complete");
  uit(true);
}

void loop()
{
  doParty();
}





