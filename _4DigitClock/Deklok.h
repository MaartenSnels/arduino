#ifndef DEKLOK_H
#define DEKLOK_H

// defenities van de rijen
const int UUR_RING = 3;
const int SEC_RING = 1;
const int MIN_RING = 2;


// patroon voor aantla lampjes in een rij
const int LAMPJES_RIJ[9] = {
   0x0,       // 00000000
   0x80,      // 10000000
   0xc0,      // 11000000
   0xe0,      // 11100000
   0xf0,      // 11110000
   0xf8,      // 11111000
   0xfc,      // 11111100
   0xfe,      // 11111110  
   0xff       // 11111111
};

// patroon voor aantla lampjes in een rij
const int LAMPJES_ENKEL[9] = {
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

const int ROOD_MIN_GROEN_UUR[8] = {
  0x7B,      // 01111011    -> lampje 01 t/m 08
  0xDE,      // 11011110    -> lampje 09 t/m 16
  0xF7,      // 11110111    -> lampje 17 t/m 24
  0xBD,      // 10111101    -> lampje 25 t/m 32
  0xEF,      // 11101111    -> lampje 33 t/m 40
  0x7B,      // 01111011    -> lampje 41 t/m 48
  0xDE,      // 11011110    -> lampje 49 t/m 56
  0xF0       // 11110000    -> lampje 57 t/m 64
};



const int GROEN_MIN_ROOD_UUR[8] = {
  0x84,      // 10000100    -> lampje 01 t/m 08
  0x21,      // 00100001    -> lampje 09 t/m 16
  0x08,      // 00001000    -> lampje 17 t/m 24
  0x42,      // 01000010    -> lampje 25 t/m 32
  0x10,      // 00010000    -> lampje 33 t/m 40
  0x84,      // 10000100    -> lampje 41 t/m 48
  0x21,      // 00100001    -> lampje 49 t/m 56
  0x00       // 00000000    -> lampje 57 t/m 64
};


#endif

