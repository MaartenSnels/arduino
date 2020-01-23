
/**
 * Lamp op de 220
 */
void lampAanUit(bool aan) {
  digitalWrite(LAMP_RELAIS, !aan);
  isLampActivated = aan;
  Serial.print("Action lamp aan = ");
  if (aan) {
    Serial.print("AAN");
  } else  {
    Serial.print("UIT");
  } 
}

/**
 * Openen of sluiten van de deur
 */
void deur() {
  Serial.println("Action deur");
  digitalWrite(DEUR_RELAIS, LOW);
  lampAanUit(true);
  lampActivated = millis();
  scannerLedRed(10,40);
  delay(2000);
  digitalWrite(DEUR_RELAIS, HIGH);
}

void lampDeur(bool aan) {
  digitalWrite(SCANNER_LED_RED, !aan);
}

void scannerLedRed(int _delay, byte count) {
  Serial.println("Action deurErr");
  for (int i = 0; i < count; i++) {
    lampDeur(true);
    delay(_delay);
    lampDeur(false);
    delay(_delay);
  }
}

void geenActie() {
  Serial.println("Action geen");
}

void lampToggle() {
  // voorkom lang indrukken en check:
  // meer dan 10 seconden indrukken, haal keymemory leeg
  unsigned long start = millis();
  while (digitalRead(LAMP_MANUAL_BTN) == LOW) {
    delay(10);
    if (millis() - start > CLEAR_MEMORY_HOLD_MILLS) {
      for (int i = 0; i< 3; i++) {
        scannerLedRed(200,20);
      }
      keyStore.reset();
    }
    // doe niets
  }

  // Wat te doen als de lamp door de deur aan gaat?
  // toggle == high -> lamp uit -> toggle low
  // toggle == low  -> lamp uit -> toggle low
  if (isLampActivated) {
    lampToggleState = false;
    lampAanUit(false);
    Serial.println("Action Manual lamp uit na deur");
    return;
  }
  lampToggleState = !lampToggleState;
  if (lampToggleState) {
    Serial.println("Action Manual lamp aan");
    lampAanUit(true);
  } else {
    lampAanUit(false);
    Serial.println("Action Manual lamp uit");
  }
}

/**
 * Als remove knop ingedrukt samen met master dan kom
 * je in de remove mode. Als je al in add or remove zit
 * en de kaart komt nog eens langs, dan gaat de modes uit
 */
void mayAddCard() {
  Serial.print("Action mayAddCard ");
  addCardMode = !addCardMode;
  if (addCardMode) {
    if (digitalRead(REMOVE_KEY_BTN) == LOW) {
      removeMode = true;
      removeCardLightOn = true;
      Serial.print("remove mode ");
    } else {
      removeMode = false;
      Serial.print("add mode ");
    }
    
    Serial.println("START");
    lampDeur(true);
    cardAddActivated = millis();
  } else {
    Serial.println("END");
    lampDeur(false);
    cardAddActivated = 0;
    removeMode = false;
  }
}

void addCard(id_type* card) {
  Serial.println("Action addCard");
  cardAddActivated = millis();
  if (removeMode) {
    keyStore.removeKey(card);
    return;
  }
  if (keyStore.keyPresent(card) >= 0) {
    // deze controle vindt ook al plaats in KeyStore
    scannerLedRed(50,20);
    lampDeur(true);
    Serial.println("Kaart is al geregistreerd");
    return;
  }
  if (keyStore.addKey(card)) {
    scannerLedRed(20,20);
  } else {
    scannerLedRed(50, 20);
  }
  
}

