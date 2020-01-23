
/////////////////// BEGIN declaratie functies //////////////////////

/////////////////// EIND declaratie functies //////////////////////


void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (--i);              // get bitmask
    digitalWrite( CLOCK, LOW);         // tick
    digitalWrite(DATA_IN, data & mask);
    digitalWrite(CLOCK, HIGH);         // tock
  }
}

/**
 *    maxSingle is the "easy"  function to use for a      
 *    single max7219
 */
void maxSingle( byte reg, byte col) {    
  digitalWrite(LOAD, LOW);       // begin     
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data   
  digitalWrite(LOAD, LOW);       // and load da shit
  digitalWrite(LOAD,HIGH); 
}


/**
 * initialize  all  MAX7219's in the system
 */
void maxAll (byte reg, byte col) {
  int c = 0;
  digitalWrite(LOAD, LOW);  // begin     
  for ( c =1; c<= MAX_IN_USE; c++) {
    putByte(reg);  // specify register
    putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  }
  digitalWrite(LOAD, LOW);
  digitalWrite(LOAD,HIGH);
}


/**
 * maxOne is for adressing different MAX7219's, 
 * whilele having a couple of them cascaded
 */
void maxOne(int maxNr, int reg, int col) {    

  byte c = 0;
  digitalWrite(LOAD, LOW);  // begin     

  for ( c = MAX_IN_USE; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data 

  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(LOAD, LOW); // and load da shit
  digitalWrite(LOAD,HIGH); 
}

//////////////////////////// routines voor teken van patronen e.d. ////////////////////////
void tekenPatroon(byte matrix,  byte patroon[]) {
  for (int i = 1; i < 9 ; i++ ) {
    maxOne(matrix, i, patroon[i-1]);
  }
}

void tekenPatroon(byte patroon[]) {
  for (byte j = 1; j<5; j++) {
    for (byte i = 1; i < 9 ; i++ ) {
      maxOne(j, i, patroon[i-1]);
    }
  }
}

void leeg() {
  for (int i = 1; i <= MAX_IN_USE; i++) {
    tekenPatroon(i, LEEG);
  }
}

void getDoorDraaiPatroon(byte *van, byte *naar, byte *scherm ) {
  for (byte i = 0; i < AANTAL_LEEG; i++) {
    scherm[8 + i] = 0;
  }
  
  // let op, er zit ook nog een lege regel in
  for(byte i = 0; i < 8; i++) {
    scherm[i] = van[i];
    scherm[8 + AANTAL_LEEG + i] = naar[i];
  }
}

void printByte(byte in) {
  Serial.print("|");
  Serial.print((in & B10000000 ) == B10000000 ? "X" : " ");
  Serial.print((in & B1000000  ) == B1000000  ? "X" : " ");
  Serial.print((in & B100000   ) == B100000   ? "X" : " ");
  Serial.print((in & B10000    ) == B10000    ? "X" : " ");
  Serial.print((in & B1000     ) == B1000     ? "X" : " ");
  Serial.print((in & B100      ) == B100      ? "X" : " ");
  Serial.print((in & B10       ) == B10       ? "X" : " ");
  Serial.print((in & B1        ) == B1        ? "X" : " ");
  Serial.print("|");
}

void draaiDoor(byte matrix, byte *van, byte *naar, int _delay) {
  byte scherm[16 + AANTAL_LEEG];
  getDoorDraaiPatroon(van, naar, &scherm[0]);
  
  for (byte i = 0; i < 8 + AANTAL_LEEG; i++) {
    for (byte j = 1; j <= 8; j++) {
      maxOne(matrix, j, scherm[i + j]);
    }
    delay(_delay);
  }
}

void draaiVanNaar(int van, int naar, int _delay) {
  if (DEBUG) {
    Serial.print("old: ");
    Serial.print(van);
    Serial.print("\t new: ");
    Serial.println(naar);
  }

  byte was[4] = {van % 10,
                 (van / 10) % 10,
                 (van /100) % 10,
                 (van /1000)};
  
  byte is[4] = {naar % 10,
                (naar / 10) % 10,
                (naar /100) % 10,
                (naar /1000)};

  byte patroon[4][16 + AANTAL_LEEG]; 
  for (byte i = 0; i< 4; i++) {
    if (was[i] != is[i]) {
      getDoorDraaiPatroon(CIJFERS[was[i]], CIJFERS[is[i]], &patroon[i][0]);
    } else {
      tekenPatroon(i+1, CIJFERS[was[i]]);
    }
  }

  for (byte i = 0; i < 8 + AANTAL_LEEG; i++) {
    for (byte j = 1; j <= 8; j++) {
      for (byte k = 0; k< 4; k++) {
        if (was[k] != is[k]) {
          maxOne(k + 1, j, patroon[k][i + j]);
        }
      }
    }
    delay(_delay);
  }
  
}





