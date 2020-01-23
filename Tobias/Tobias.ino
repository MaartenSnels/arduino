// define max7219 registers
byte max7219_reg_noop         =0x00;
byte max7219_reg_digit0       =0x01;
byte max7219_reg_digit1       =0x02;
byte max7219_reg_digit2       =0x03;
byte max7219_reg_digit3       =0x04;
byte max7219_reg_digit4       =0x05;
byte max7219_reg_digit5       =0x06;
byte max7219_reg_digit6       =0x07;
byte max7219_reg_digit7       =0x08;
byte max7219_reg_decodeMode   =0x09;
byte max7219_reg_intensity    =0x0a;
byte max7219_reg_scanLimit    =0x0b;
byte max7219_reg_shutdown     =0x0c;
byte max7219_reg_displayTest  =0x0f;


byte dataIn    = 2;    // pin met input data voor de max7219 chips
byte load      = 3;    // pin voor load 
byte clock     = 4;    // pin voor klok signaal
byte maxInUse  = 2;    // change this variable to set how many MAX7219's you'll use

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



void setup() {

  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);

  //beginSerial(9600);
  digitalWrite(13, HIGH);  

//initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
  for (int e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x00 );    // the first 0x0f is the value you can set
                                                 // range: 0x00 to 0x0f

}

// patroon voor aantla lampjes in een rij
byte LAMPJES_ENKEL[9] = {
   0x00,      // 00000000
   0x80,      // 10000000
   0x40,      // 01000000
   0x20,      // 00100000
   0x10,      // 00010000
   0x08,      // 00001000
   0x04,      // 00000100
   0x02,      // 00000010  
   0x01       // 00000001
};


byte LETTER_T[8] = {
  0xFF,      // 11111111    -> lampje 01 t/m 08
  0xFF,      // 11111111    -> lampje 09 t/m 16
  0x18,      // 00011000    -> lampje 17 t/m 24
  0x18,      // 00011000    -> lampje 25 t/m 32
  0x18,      // 00011000    -> lampje 33 t/m 40
  0x18,      // 00011000    -> lampje 41 t/m 48
  0x18,      // 00011000    -> lampje 49 t/m 56
  0x18       // 00011000    -> lampje 57 t/m 64
};
byte SMILY_BLIJ[8] = {
  0xFF,      // 11111111    -> lampje 01 t/m 08
  0x81,      // 10000001    -> lampje 09 t/m 16
  0xA5,      // 10100101    -> lampje 17 t/m 24
  0x81,      // 10000001    -> lampje 25 t/m 32
  0xA5,      // 10100101    -> lampje 33 t/m 40
  0x99,      // 10011001    -> lampje 41 t/m 48
  0x81,      // 10000001    -> lampje 49 t/m 56
  0xFF       // 11111111    -> lampje 57 t/m 64
};
byte LETTER_H[8] = {
  0xC3,      // 11000011    -> lampje 01 t/m 08
  0xC3,      // 11000011    -> lampje 09 t/m 16
  0xC3,      // 11000011    -> lampje 17 t/m 24
  0xFF,      // 11111111    -> lampje 25 t/m 32
  0xFF,      // 11111111    -> lampje 33 t/m 40
  0xC3,      // 11000011    -> lampje 41 t/m 48
  0xC3,      // 11000011    -> lampje 49 t/m 56
  0xC3       // 11000011    -> lampje 57 t/m 64
};
byte LETTER_A[8] = {
  0xFF,      // 11111111    -> lampje 01 t/m 08
  0xFF,      // 11111111    -> lampje 09 t/m 16
  0xC3,      // 11000011    -> lampje 17 t/m 24
  0xC3,      // 11000011    -> lampje 25 t/m 32
  0xFF,      // 11111111    -> lampje 33 t/m 40
  0xFF,      // 11111111    -> lampje 41 t/m 48
  0xC3,      // 11000011    -> lampje 49 t/m 56
  0xC3       // 11000011    -> lampje 57 t/m 64
};




void tekenPatroon(byte matrix,  byte patroon[]) {
  for (int i = 1; i < 9 ; i++ ) {
    maxOne(matrix, i, patroon[i-1]);
  }
}


void allesAanUit(byte matrix, bool aan) {
  byte patroon = 0x00;
  if (aan)
    patroon = 0xff;
  
  byte i = 1;
  while (i <=8) {
    maxOne(matrix, i, patroon);
    i = i + 1;
  }
}

void rondje(byte ring) {
  for (int rij = 1; rij < 9; rij++) {
    for (int kolom = 1; kolom < 9; kolom++) {
      maxOne(ring, rij, LAMPJES_ENKEL[kolom]);
      delay(25);
    }
    // laatste lampje in rij uit
    maxOne(ring, rij, 0x00);
  }
}


void loop() {
//  rondje(1);
//  rondje(2);
//  allesAanUit(1,true);
  allesAanUit(2,false);
  allesAanUit(1,false);
  delay(1000);
//  tekenPatroon(1,LETTER_T);
  tekenPatroon(2,SMILY_BLIJ);
  tekenPatroon(1,SMILY_BLIJ);
  delay(1000);
  tekenPatroon(1,LETTER_H);
  tekenPatroon(2,LETTER_A);
  delay(1000);
  allesAanUit(2,false);
  allesAanUit(1,false);
  delay(1000);
  tekenPatroon(1,LETTER_H);
  tekenPatroon(2,LETTER_A);
  delay(1000);
  tekenPatroon(2,SMILY_BLIJ);
  tekenPatroon(1,SMILY_BLIJ);
  delay(1000);
}
