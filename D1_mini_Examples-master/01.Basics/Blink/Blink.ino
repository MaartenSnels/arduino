/*
 * Blink
 * Turns on the onboard LED on for one second, then off for one second, repeatedly.
 * This uses delay() to pause between LED toggles.
 */


// for info on defice: https://gregwareblog.wordpress.com/2016/01/10/esp-witty/

#include <ESP8266WiFi.h>


#define PIN_BUTTON         4
#define PIN_LED_RED       15
#define PIN_LED_GREEN     12
#define PIN_LED_BLUE      13
#define PIN_LED_INTERNAL   1
#define PIN_LDR           A0

#define DELAY_MIN         50
#define DELAY_MAX       1000


// declartie gebruikte functies
void  switchColor(char ledOn, char ledOff, int __delay);
void changeDelay();
void setAllLeds(boolean on);
void dim(char led, int intense, int __delay);
// einde declaratie gebruikte functies

boolean snelBijDonker = true;
boolean snel = false;
void setup() {
  pinMode(PIN_BUTTON      , INPUT );  // Lees input van knop
  pinMode(PIN_LED_RED     , OUTPUT);  // Led rood is output
  pinMode(PIN_LED_GREEN   , OUTPUT);  // Led groen is output
  pinMode(PIN_LED_BLUE    , OUTPUT);  // Led blauw is output
  pinMode(PIN_LED_INTERNAL, OUTPUT);  // Led blauw is output

  // interups als je op knopje drukt.
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), changeDelay, FALLING);
  
  Serial.begin(115200);
}

void loop() {
  if (snel) {
    switchColor(PIN_LED_RED  , PIN_LED_BLUE , analogRead(PIN_LDR));
    switchColor(PIN_LED_GREEN, PIN_LED_RED  , analogRead(PIN_LDR));
    switchColor(PIN_LED_BLUE , PIN_LED_GREEN, analogRead(PIN_LDR));
  } else {
    dim(PIN_LED_RED  , analogRead(PIN_LDR), 500);
    dim(PIN_LED_GREEN, analogRead(PIN_LDR), 500);
    dim(PIN_LED_BLUE , analogRead(PIN_LDR), 500);
  }
}

void  switchColor(char ledOn, char ledOff, int __delay) {
  digitalWrite(ledOn , HIGH);
  digitalWrite(ledOff, LOW);
  if (__delay < DELAY_MIN) {
    __delay = DELAY_MIN;
  } else if (__delay > DELAY_MAX) {
    __delay = DELAY_MAX;
  }
  if (!snelBijDonker) {
    __delay = DELAY_MAX + DELAY_MIN - __delay; 
  }
  delay(__delay);
  Serial.print  ("Lichtgevoeligheid: ");
  Serial.print  (__delay);
  Serial.print  ("\tLed ");
  Serial.print  (ledOn,DEC);
  Serial.print  ("\taan en led  ");
  Serial.print  (ledOff,DEC);
  Serial.println("\tuit");
}

void changeDelay() {
  digitalWrite(PIN_LED_INTERNAL, snelBijDonker);
  while(!digitalRead(PIN_BUTTON)){
    // doe niets
  }
//  digitalWrite(PIN_LED_INTERNAL, false);
  Serial.print  ("Toggel snelBijDonker, wordt: ");
  Serial.println(snelBijDonker);
  snelBijDonker = !snelBijDonker; 
  setAllLeds(false);
}

void setAllLeds(boolean on) {
  int value = 0;
  if (on) {
    value = 1023;
  }
  analogWrite(PIN_LED_RED  , value);
  analogWrite(PIN_LED_GREEN, value);
  analogWrite(PIN_LED_BLUE , value);
}

void dim(char led, int intense, int __delay) {
  Serial.print("dim led op poot");
  Serial.println(led, DEC);
  setAllLeds(false);
  int value = intense;
  if (!snelBijDonker) {
    value = 1023 - value;
  }
  analogWrite(led,value);
  delay(__delay);
}

