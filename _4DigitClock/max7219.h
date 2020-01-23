#ifndef MAX7219_h
#define MAX7219_h


#define DATA_IN    10    // pin met input data voor de max7219 chips
#define LOAD       11    // pin voor load 
#define CLOCK      12    // pin voor klok signaal
#define MAX_IN_USE 4    // change this variable to set how many MAX7219's you'll use

// define max7219 registers
#define max7219_reg_noop         0x00
#define max7219_reg_digit0       0x01
#define max7219_reg_digit1       0x02
#define max7219_reg_digit2       0x03
#define max7219_reg_digit3       0x04
#define max7219_reg_digit4       0x05
#define max7219_reg_digit5       0x06
#define max7219_reg_digit6       0x07
#define max7219_reg_digit7       0x08
#define max7219_reg_decodeMode   0x09
#define max7219_reg_intensity    0x0a
#define max7219_reg_scanLimit    0x0b
#define max7219_reg_shutdown     0x0c
#define max7219_reg_displayTest  0x0f

#define EEN  4
#define TWEE 3
#define DRIE 2
#define VIER 1


byte LEEG[8]    = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte VOL[8]     = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
byte GRADEN[8]  = {0x1E,0x12,0x12,0x1E,0x00,0x00,0x00,0x00};
byte CELCIUS[8] = {0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF};

byte A[8] = {B11111111, 
             B10000001,
             B10000001,
             B11111111,
             B10000001,
             B10000001,
             B10000001,
             B10000001};
byte B[8] = {B11111100, 
             B10000010,
             B10000001,
             B11111110,
             B10000010,
             B10000001,
             B10000001,
             B10000010};
byte C[8] = {B11111100, 
             B10000000,
             B10000000,
             B10000000,
             B10000000,
             B10000000,
             B10000000,
             B11111111};
byte D[8] = {B00000001, 
             B00000001,
             B00000001,
             B00000001,
             B11111111,
             B10000001,
             B10000001,
             B11111111};
byte E[8] = {B00000000,  
             B11111111,
             B10000000,
             B10000000,
             B11111111,
             B10000000,
             B10000000,
             B11111111};
byte I[8] = {B00000001, 
             B00000000,
             B00000001,
             B00000001,
             B00000001,
             B00000001,
             B00000001,
             B00000001};
byte J[8] = {B00000001, 
             B00000000,
             B00000001,
             B00000001,
             B00000001,
             B00100001,
             B00100001,
             B00111111};
byte M[8] = {B10000001,  
             B11000011,
             B10100101,
             B10011001,
             B10000001,
             B10000001,
             B10000001,
             B10000001};
byte N[8] = {B10000001,  
             B11000001,
             B10100001,
             B10010001,
             B10001001,
             B10000101,
             B10000011,
             B10000001};
byte O[8] = {B00111100,  
             B01000010,
             B10000001,
             B10000001,
             B10000001,
             B10000001,
             B01000001,
             B00111100};
byte P[8] = {B11111111,  
             B10000001,
             B10000001,
             B11111111,
             B10000000,
             B10000000,
             B10000000,
             B10000000};
byte R[8] = {B11111111,  
             B10000001,
             B10000001,
             B11111111,
             B10100000,
             B10010000,
             B10001000,
             B10000100};
byte S[8] = {B11111111,  
             B10000000,
             B10000000,
             B11110000,
             B00001111,
             B00000001,
             B00000001,
             B11111111};
byte T[8] = {B11111111,  
             B00010000,
             B00010000,
             B00010000,
             B00010000,
             B00010000,
             B00010000,
             B00010000};
byte U[8] = {B10000001,
             B10000001,
             B10000001,
             B10000001,
             B10000001,
             B10000001,
             B10000001,
             B11111111};

byte CIJFERS[10][8]  = {
  {0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF}, // 0
  {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03}, // 1
  {0xFF,0x03,0x03,0xFF,0xFF,0xC0,0xC0,0xFF}, // 2
  {0xFF,0x03,0x03,0xFF,0xFF,0x03,0x03,0xFF}, // 3
  {0xC3,0xC3,0xC3,0xFF,0xFF,0x03,0x03,0x03}, // 4
  {0xFF,0xC0,0xC0,0xFF,0xFF,0x03,0x03,0xFF}, // 5
  {0xFF,0xC0,0xC0,0xFF,0xFF,0xC3,0xC3,0xFF}, // 6
  {0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03}, // 7
  {0xFF,0xC3,0xC3,0xFF,0xFF,0xC3,0xC3,0xFF}, // 8
  {0xFF,0xC3,0xC3,0xFF,0xFF,0x03,0x03,0xFF}  // 9
};


#endif
