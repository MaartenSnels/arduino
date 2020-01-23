#define RED_LED   12  // red led
#define BLUE_LED  11  // blue led
#define RED_BTN   10  // red button
#define BLUE_BTN  9   // blue button
#define DELAY_MIN 40
#define DELAY_MAX 100

bool on = true;
void setup() {
  pinMode(RED_LED,  OUTPUT        );
  pinMode(BLUE_LED, OUTPUT        );
  pinMode(RED_BTN,  INPUT_PULLUP  );
  pinMode(BLUE_BTN, INPUT_PULLUP  );

  randomSeed(analogRead(0));
}

int step = 0;
void loop() {
//  blink(random(1000) > 500, 
//        random(1000) > 500, 
//        random(DELAY_MIN, DELAY_MAX));
 // doBlink(step, 300);
  step = (step + 1) %  3;
  checkButtons();
}

void doBlink(int blinkStep, int _delay) {
  switch(blinkStep) {
    case 0:   blink(true,false, _delay); break;
    case 1:   blink(false,true, _delay); break;
    case 2:   blink(true,true,  _delay); break;
  }
  blink(false,false,100);

}

void blink(bool red, bool blue, long _delay) {
  digitalWrite(RED_LED,  red);
  digitalWrite(BLUE_LED, blue);
  delay(_delay);

}

void checkButtons() {
  bool redPressed  = digitalRead(RED_BTN)  == LOW;
  bool bluePressed = digitalRead(BLUE_BTN) == LOW;

  while (redPressed || bluePressed) {
    blink(redPressed, bluePressed, 1000);
    
    redPressed  = digitalRead(RED_BTN)  == LOW;
    bluePressed = digitalRead(BLUE_BTN) == LOW;
  }
  digitalWrite(RED_LED,  LOW);
  digitalWrite(BLUE_LED, LOW);
}

