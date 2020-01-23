
void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (--i);              // get bitmask
    digitalWrite( clock, LOW);         // tick
    digitalWrite(dataIn, data & mask);
    digitalWrite(clock, HIGH);         // tock
  }
}

void maxSingle( byte reg, byte col) {    
//maxSingle is the "easy"  function to use for a     //single max7219

  digitalWrite(load, LOW);       // begin     
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data   
  digitalWrite(load, LOW);       // and load da shit
  digitalWrite(load,HIGH); 
}




void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin     
  for ( c =1; c<= maxInUse; c++) {
    putByte(reg);  // specify register
    putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}

void maxOne(byte maxNr, byte reg, byte col) {    
//maxOne is for adressing different MAX7219's, 
//whilele having a couple of them cascaded

  int c = 0;
  digitalWrite(load, LOW);  // begin     

  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data 

  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(load, LOW); // and load da shit
  digitalWrite(load,HIGH); 
}

