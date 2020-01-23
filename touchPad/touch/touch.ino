#define BUTTONS_LED_PIN      5  // Pin 7 van keypad 

#define BUTTON_AUTO_PIN      0  // Pin 1 van keypad 
#define BUTTON_MENU_PIN      1
#define BUTTON_LEFT_PIN      2  // pin 3 van keypad   
#define BUTTON_POWER_PIN     3  // pin 4 van keypad
#define BUTTON_RIGHT_PIN     4  // pin 6 van keypad
                                // Pin 5 van keypad is ground 
char pins[] = {8, 9, 10, 11};
char touchPanels = 4;
void setup() {
  Serial.begin(250000);
  Serial.println("setup");
  for(int i = 0; i < touchPanels; i++) {
    pinMode(pins[i], INPUT);
  }
  pinMode(BUTTONS_LED_PIN,  OUTPUT);
  pinMode(BUTTON_POWER_PIN, INPUT_PULLUP);
  pinMode(BUTTON_MENU_PIN,  INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN,  INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_AUTO_PIN,  INPUT_PULLUP);
  
  Serial.println("Touch please");
}

void loop() {
    bool touched = false;
    for(int i = 0; i < touchPanels; i++) {
      if (digitalRead(pins[i])) {
        touched = true;
        Serial.print(i+1);  
        Serial.print("\t");  
      }
    }
    if (touched) {
      Serial.println("\t touched");  
    }
    digitalWrite(BUTTONS_LED_PIN, !touched);

    if (!digitalRead(BUTTON_POWER_PIN)) {
      Serial.println("POWER PRESSED");
    }
    if (!digitalRead(BUTTON_MENU_PIN)) {
      Serial.println("MENU PRESSED");
    }
    if (!digitalRead(BUTTON_LEFT_PIN)) {
      Serial.println("LEFT PRESSED");
    }
    if (!digitalRead(BUTTON_RIGHT_PIN)) {
      Serial.println("RIGHT PRESSED");
    }
    if (!digitalRead(BUTTON_AUTO_PIN)) {
      Serial.println("AUTO PRESSED");
    }
    Serial.println("--------------------------------");
    delay(1000);
}
