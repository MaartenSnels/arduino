/*

    Hier party's definieren. 
    Mogelijk om ze zelf te keizen of ramdom

*/


void runParty() {
  lamp();
}


void lamp() {
  allesAan(ON);
  char key = keypad.getKey();
  while(!key) {
    delay(5);
    key = keypad.getKey();  
  }
  allesAan(OFF);
}

