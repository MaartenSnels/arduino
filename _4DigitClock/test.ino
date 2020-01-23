//void handleButtonPressedInterrupt() {
//  runTest = false;
//  Serial.println("Test interrupted");
//}
//
//
//void test () {
//  Serial.println("Beginnen met testen");
//
//  // vang knopppen op
//  attachInterrupt(digitalPinToInterrupt(BTN_1), handleButtonPressedInterrupt, FALLING);
//  attachInterrupt(digitalPinToInterrupt(BTN_2), handleButtonPressedInterrupt, FALLING);
//  leeg();
//  runTest = true;
//  draai(20);
//  testKnoppen(100);
//  blokken();
//  testMatrix();
//  randomLeds(3000, true);
//  runTest = false;
//
//  // haal interrupts weer weg
//  detachInterrupt(digitalPinToInterrupt(BTN_1));
//  detachInterrupt(digitalPinToInterrupt(BTN_2));
//
//  // wat er ook gebeurt, alles weer leeg en intenciteit weer naar normaal
//  // zorg dat als je klaar bent de tijd weer goed staa
//  maxAll(max7219_reg_displayTest, 0x00);
//  maxOne(VIER, max7219_reg_intensity, intensity);
//  writeTime = true;
//  // zorg dat eerste druk van knop niet doorkomt
//  delay(200); 
//  Serial.println("Einde testen");
//}
//
//void blokken() {
//  
//}
//
//void draai(int _delay) {
//  leeg();
//  for(int i=990; i<1001 && runTest;  i++) {
//    draaiVanNaar(i, i+1, _delay);
//  }
//  leeg();
//  
////  draaiDoor(EEN , LEEG, CIJFERS[0] , _delay);
////  for(byte i = 0; i < 9 && runTest; i++) {
////    draaiDoor(EEN , CIJFERS[i], CIJFERS[i + 1] , _delay);
////  }
////  draaiDoor(EEN , CIJFERS[9], LEEG, _delay);
//}
//
//void testKnoppen(int _delay) {
//  Serial.println("Begin test knoppen");
//  bool on = true;
//  for(int i = 0; i < 10 && runTest; i++) {
//    digitalWrite(BTN_1_LED, on);
//    digitalWrite(BTN_2_LED, !on);
//    on = !on;
//    delay(_delay);
//  }
//  Serial.println("Einde test knoppen");
//}
//
///////////////////////////////// Testen en mooie patronen /////////////////////////////////
//void testMatrix() {
//  Serial.println("Begin testMatrix");
//
//  knipper(10,50);
//  doLoop(5, true);
//  doLoop(5, false);
//  vul(5, true);
//  vul(5, false);
//  lichtSterkte(250);
//  Serial.println("Einde testMatrix");
//}
//
//void knipper(int aantal, int _delay) {
//  for (int i= 0;  i< aantal && runTest; i++) {
//    maxAll(max7219_reg_displayTest, 0x01);
//    delay(_delay);
//    maxAll(max7219_reg_displayTest, 0x00);
//    delay(_delay);
//  }
//}
//
///**
// * Lampje dat van links naar rechts gaat
// * links - rechts
// * een naar beneden en dan
// * rechts links
// * 
// * int delay: aantal ms tussen de opdrachten door
// * boolean heen: begin boven ov beneden
// * 
// */
//void doLoop(int _delay, boolean heen) {
//  for(int c = 1; c < 9 && runTest; c++) {
//    int col = (heen ? c : 9 - c);
//    boolean lr = c % 2 == 0; // left -> right
//    for(int max = 1; max <=  MAX_IN_USE; max++) {
//      int useMax = lr ? max : MAX_IN_USE - max + 1;
//      int led = lr ? 1 : 128;
//      for(int row = 0; row < 8; row++) {
//        maxOne(useMax, col , led);
//        delay(_delay);
//        if (lr) {
//          led = led << 1;
//        } else {
//          led = led >> 1;
//        }
//      }
//      // gooi de boel maar leeg
//      maxOne(useMax, col , 0);
//    }
//  }
//}
//
//
///**
// * VUl alle maxen doe dit per rij, van rechts naar links
// */
//void vul(int _delay, boolean on) {
//  for(int col = 1; col < 9 && runTest; col++) {
//    for(int max = 1; max <=  MAX_IN_USE; max++) {
//      int led = 2;
//      for(int row = 0; row < 8; row++) {
//        int leds = led - 1;
//        if (!on) {
//          leds = ~leds;
//        }
//        led = led << 1;
//        maxOne(max, col , leds);
//        delay(_delay);
//      }
//    }
//  }
//}
//
///**
// * Laat alle lichtsterktes zien
// */
//void lichtSterkte(int _delay) {
//
//  leeg();
//  tekenPatroon(VIER, VOL);
//  for (byte i = 0; i <= 0x0f && runTest; i++) {  
//    maxOne(VIER, max7219_reg_intensity, i);
//    tekenPatroon(EEN,  CIJFERS[i / 10]);
//    tekenPatroon(TWEE, CIJFERS[i % 10]);
//    delay(_delay);
//  } 
//  maxOne(VIER, max7219_reg_intensity, intensity);
//}
//
//void randomLeds(int aantal, bool clear) {
//  Serial.println("random leds");
//  if (clear) {
//    leeg();
//  }
//  randomSeed(analogRead(0));
//  for(int i = 0; i<aantal && runTest; i++) {
//    maxOne(random(1,5),random(1,9),random(0,256));    
//  }
//  if (clear) {
//    leeg();
//  }
//}
//
//

