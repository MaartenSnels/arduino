#include <Keypad.h>           // uitlezen van het toetsenbord

#define HOLD_TIME          200
#define KB_COLS              4  //four rows
#define KB_ROWS              1  //four columns
char keys[KB_COLS][KB_ROWS] = {
  {'2'} , {'1'} , {'4'} , {'3'}
};
// aansluiting van matrix van rechts -> links (meest rechts -> 2, meest links -> 9)
byte rowPins1[KB_ROWS] = {4}; //connect to the row pinouts of the keypad
byte colPins1[KB_COLS] = {5, 6, 7, 8}; //connect to the column pinouts of the keypad
byte rowPins2[KB_ROWS] = {9}; //connect to the row pinouts of the keypad
byte colPins2[KB_COLS] = {10, 11, 12, 13}; //connect to the column pinouts of the keypad
Keypad kpd1 = Keypad( makeKeymap(keys), rowPins1, colPins1, KB_ROWS, KB_COLS );
Keypad kpd2 = Keypad( makeKeymap(keys), rowPins2, colPins2, KB_ROWS, KB_COLS );


String msg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  Serial.println("Drukken maar");
  // kpd1.setHoldTime(HOLD_TIME);
}

void loop() {
  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (kpd1.getKeys() || kpd2.getKeys())
  {
    verwerkKeys(&kpd1);
  }
}

void verwerkKeys (Keypad *keypad) {
    for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
    {
      if ( keypad->key[i].stateChanged || keypad->key[i].kstate == HOLD)   // Only find keys that have changed state.
      {
        switch (keypad->key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            msg = " PRESSED.";
            break;
          case HOLD:
            msg = " HOLD.";
            break;
          case RELEASED:
            msg = " RELEASED.";
            break;
          case IDLE:
            msg = " IDLE.";
        }
        Serial.print("Key ");
        Serial.print(keypad->key[i].kchar);
        Serial.println(msg);
      }
    }
}


