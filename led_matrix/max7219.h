#ifndef MAX7219_h
#define MAX7219_h

#define dataIn     2    // pin met input data voor de max7219 chips
#define load       3    // pin voor load 
#define clock      4    // pin voor klok signaal
#define maxInUse   3    // change this variable to set how many MAX7219's you'll use

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


#endif
