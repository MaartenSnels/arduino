#define __DEBUG__


void ShowReaderDetails() {
  // Get the MFRC522 software version
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown)"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  }
}

void handleCard() {
    // Dump debug info about the card; PICC_HaltA() is automatically called
  // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  id_type card = getId();
  if (isMaster(card)) {
    Serial.println("MASTER scanned");
    mayAddCard();
  } 
  else if (addCardMode) {
      addCard(&card);
  }
  else if (keyStore.keyPresent(&card) >= 0) {
      deur();
  } 
  else {
    scannerLedRed(100,20);
  }
  
}


id_type getId() {
  id_type card;
  Serial.println("Scanned PICC's UID:");
  card.size = mfrc522.uid.size;
  for (int i = 0; i <  MAX_ID_SIZE; i++) {
    if (i < card.size) {
      card.id[i] = mfrc522.uid.uidByte[i];
#ifdef __DEBUG__
      Serial.print(card.id[i], HEX);
#endif
    } else {
      card.id[i] = 0;
    }
  }
#ifdef __DEBUG__
  Serial.println("");
#endif
  mfrc522.PICC_HaltA(); // Stop reading
  addToList(cardId);
  return card;
}

int listTeller = 0; // Houdt de kop van de lijst bij
void addToList(id_type id) {
  ids[listTeller] = id;
  listTeller = (listTeller + 1) % MAX_KEYS_HISTORY;
  lastAction = SCAN;
}

boolean isMaster(id_type id) {

  if (id.size != masterId.size) {
#if defined (__DEBUG__)
    Serial.println("Check master");
    Serial.println("grootte van sleutels is niet gelijk");
#endif
    return false;
  }
  for(int i = 0; i < masterId.size; i++) {
    if (id.id[i] != masterId.id[i]) {
#ifdef __DEBUG__
      Serial.print("Check master: ID's niet gelijk voor element: ");
      Serial.print(i, DEC);
      Serial.print(" ");
      Serial.print(id.id[i], DEC);
      Serial.print(" ");
      Serial.println(masterId.id[i], DEC);
#endif
      return false; // eigelijk moet je doorgaan over alle 7 elementen. Nu is de boel veel sneller te kraken
    }
  }

  return true;
}



