
// library to store the keys in EEPROM
// written by Jason Hamilton


// The microcontrollers on the various Arduino boards have different 
// amounts of EEPROM: 1024 bytes on the ATmega328, 512 bytes on the 
// ATmega168 and ATmega8, 4 KB (4096 bytes) on the ATmega1280 and ATmega2560.
// arduino.cc/en/Reference/EEPROM
//
// bron: https://bitbucket.org/jason955/arduino-access-control/src/f1389a89ffae450f4e7a3af1bb3600ed8e1feff8/ArduinoAccessControl/?at=master



#define EEPROM_SIZE (1024) // pos 0 is voor aantal keys, pos 1-1023 voor het opslaan van keys
                           // 7 bits voor ID en 1 voor ID lenth = 8 bits/key
                           // er is dus uimte voor 126 keys. Maken we maar even 100 van voor de UNO / NANO
                           // als we meg gebruikern:  511 keys.
#define MAX_KEYS 100       // max aantal keys. Mag nooit groter dan 255 zijn omdat het in een byte staat
                           // anders programma goed herstructureeren


//////////////////
/// DEBUG ////////
//////////////////
#define __DEBUG__

//////////////////
//  HEADER////////
//////////////////

#define MAX_ID_SIZE 7             // maximale grootte van ID
#define ID_SIZE     8             // in dit geval het aantal id's en size. Als type aanpast, dit ook aanpassen
typedef struct
 {
     byte id[MAX_ID_SIZE];
     byte size;
 }  id_type;

class KeyMemory{
  public:
    KeyMemory(void);
    void   reset();
    bool   addKey(id_type* card);
    bool   removeKey(id_type* card);
    int    keyPresent(id_type* card);
    void   dumpStore();
    String getDump(String newLine);
  private:
    int    _keyCount;
    void   _incrementKeyCount();
    void   _decrementKeyCount();
    bool   _copyKey(int from, int to);
};


//////////////////////
/// IMPLEMENTATION ///
//////////////////////

KeyMemory::KeyMemory(){
  int keyCount = EEPROM.read(0);
  if (keyCount > MAX_KEYS + 1){
    reset();
  } else {
    _keyCount = keyCount;
  }
}

void KeyMemory::reset(){
  EEPROM.write(0,0);
  _keyCount = 0;
  Serial.println("Key geheugen gewist");
}

//remove key from the EEPROM list
bool KeyMemory::removeKey(id_type* card){
  Serial.println("remvoe card card");
  
  if (_keyCount <= 0 ){
    Serial.println("Geen kaarten aanwezig");
    return false;
  }

  int removeCard = keyPresent(card);
  if (removeCard < 0 ) {
    Serial.println("kaart staat niet in de lijst");
    return false;
  }

  // er  staat er maar een in de lijst
  if (_keyCount == 1) {
    Serial.println("Laatste ID uit de set verwijderd.");
    reset();
    return true;
  }
  // toegevoegde staat op de laatst plek
  if (removeCard + 1 == _keyCount) {
    Serial.println("ID met hoogste nummer verwijdferd.");
    _decrementKeyCount();
    return true;
  }

  // zet laatste element op plek van de te verwijderen key
  // haal keycount met 1 naar beneden

 if ( _copyKey(_keyCount, removeCard)) {
    _decrementKeyCount();
    return true;
 }
}

//add key to last spot in EEPROM
bool KeyMemory::addKey(id_type* card){
  Serial.println("Add card");
  
  if (_keyCount > MAX_KEYS ){
    Serial.print("Maximal number of keyz reached: ");
    Serial.println(_keyCount);
    return false;
  }

  if (keyPresent(card) >= 0 ) {
    Serial.println("kaart staat al in de lijst");
    return false;
  }
  
  _incrementKeyCount();
  int pos = (_keyCount - 1)* ID_SIZE + 1;
  
  EEPROM.write(pos++, card->size);
  for(int i = 0; i < MAX_ID_SIZE; i++) {
    EEPROM.write(pos++, card->id[i]);
  }
  
  Serial.println("Card added");
  return true;

}


int KeyMemory::keyPresent(id_type* card){
  Serial.print("Check if key present in ");
  Serial.print(_keyCount,DEC);
  Serial.println(" stored keys ");
  if (_keyCount>0){
    for(int i = 0; i < _keyCount; i++){
      boolean found = true;
#if defined (__DEBUG__)
        Serial.print("\tCheck key ");
        Serial.println(i);
#endif        
      int pos = i * ID_SIZE + 1;
      byte size = EEPROM.read(pos);
      if (card->size != size) {
#ifdef __DEBUG__
        Serial.print("\t\tKeysize komt niet overeen: ");
        Serial.print(card->size,DEC);
        Serial.print(" != ");
        Serial.println(size);
#endif        
        found = false; // niet dezelfde grootte
      }
      pos++;
      for(int j = 0; j < size && found; j++) {
        byte id = EEPROM.read(pos + j);
        if (id != card->id[j]) {
#if defined (__DEBUG__)
          Serial.print("\t\tCode van id klopt niet, pos ");
          Serial.print(j);
          Serial.print(" ID ");
          Serial.print(card->id[j],DEC);
          Serial.print(" != ");
          Serial.println(id);
#endif    
          found = false;    
        }
      }
      // size klopt en id's kloppen, dit moet hem wel zijn
      if (found) {
        Serial.print("Key present, card nr "); 
        Serial.println(i);
        return i;
      }
    }
  }
  Serial.println("Key not present");
  return -1;
}

String KeyMemory::getDump(String newLine) {
  String dump = "";
  dump += "-------- KEY DUMP -------------------------------";
  dump += newLine;
  dump += "\t\tAantal opgeslagen keys: ";
  dump += EEPROM.read(0);
  dump += newLine;
  dump += "|-------|-------|-------------------------------|";
  dump += newLine;
  dump += "| NR    | SIZE  | ID                            |";
  dump += newLine;
  dump += "|-------|-------|-------------------------------|";
  dump += newLine;
  

  for(int i = 0; i < _keyCount; i++){
    int pos = i * ID_SIZE + 1;
    dump += "| ";
    dump += i;
    dump += " \t| ";
    dump += EEPROM.read(pos++);
    dump += " \t| ";
    for (int j = 0; j < MAX_ID_SIZE; j++) {
      dump += EEPROM.read(pos + j);
      dump += " \t";
    }
    dump += "|";
    dump += newLine;
  }
  dump += "|-------|-------|-------------------------------|";
  dump += newLine;
  return dump;
}
void KeyMemory::dumpStore(){
  Serial.println(getDump("\n"));

}

/**
 * Kopieer ID van de TO naar de FROM. TO blijft bestaan
 */
bool KeyMemory::_copyKey(int from, int to) {
  Serial.print("copy card ");
  Serial.print(from);
  Serial.print(" to ");
  Serial.println(to);
  int posFrom = from * ID_SIZE + 1;
  int posTo   = to * ID_SIZE + 1;
  for (int i = 0; i < ID_SIZE; i++) {
    EEPROM.write(posTo + i, EEPROM.read(posFrom + i));
  }
  return true;
}

void KeyMemory::_incrementKeyCount(){
  _keyCount++;
  EEPROM.write(0,_keyCount);
  Serial.print("Inclrease keycount to: ");
  Serial.println(_keyCount);
}

void KeyMemory::_decrementKeyCount(){
  _keyCount--;
  EEPROM.write(0,_keyCount);
  Serial.print("Declrease keycount to: ");
  Serial.println(_keyCount);
}
