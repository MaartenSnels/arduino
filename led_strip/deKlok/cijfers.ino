#define B_11111     B11111
#define B_11101     B11101
#define B_10111     B10111
#define B_10101     B10101
#define B_11100     B11100
#define B_00111     B00111
#define B_11100     B11100
#define B_10000     B10000
#define B_00001     B00001
#define B_00000     B00000
#define B_00100     B00100
#define B_10001     B10001

#define VERJAARDAGEN 4
char verjaardagen[VERJAARDAGEN][2] {
                          {29, 7},
                          {13, 4},
                          {10, 4},
                          {28,11}
                        };
                        
                        
bool isVerjaardag() {
  for(char i = 0; i < VERJAARDAGEN; i++) {
    if (verjaardagen[i][0] == dag && verjaardagen[i][1] == maand) {
      return true;
    }
  }
  return false;
}

char cijfersR[10][3] = {
                    {B_11111,B_10001,B_11111},  // 0
                    {B_11111,B_00000,B_00000},  // 1
                    {B_10111,B_10101,B_11101},  // 2
                    {B_11111,B_10101,B_10101},  // 3
                    {B_11111,B_00100,B_00111},  // 4
                    {B_11101,B_10101,B_10111},  // 5
                    {B_11101,B_10101,B_11111},  // 6
                    {B_11111,B_00001,B_00001},  // 7
                    {B_11111,B_10101,B_11111},  // 8
                    {B_11111,B_10101,B_10111}   // 9
                  };
                  
void digital(char getal, char startpos, rgb_color color) {
  if (startpos < 25) {
     for(int i = 0; i < 3; i++) {
       for (int j=0; j<5; j++) {
         colors[startpos + i * 60 + j] = ((cijfersR[getal][i] & 1 << j) == (1 << j)) ? color : OFF;
       }
     }
  }  
  else {
    for(int i = 0; i < 3; i++) {
      for (int j=0; j<5; j++) {
        colors[startpos +4 + i * 60 - j] = ((cijfersR[getal][2 - i] & 1 << j) == (1 << j)) ? color : OFF;
      }
    }
  }
}

void writeTemp(rgb_color color) {
      float tmp = RTC.getTemp();
      allesAan(OFF);
      digital(tmp / 10, 49, color);
      digital(((int)tmp) % 10, 42, color);
      digital(((int)(tmp*10))  % 10,  34, color);
      // punt plaatsen tussen hele en breuk
      colors[160 ] = color;
      // nu graden celcius
      
      colors[1  ] = color;
      colors[2  ] = color;
      colors[3  ] = color;
      colors[61 ] = color;
      colors[121] = color;
      colors[122] = color;
      colors[123] = color;
      
      colors[117] = color;
      colors[118] = color;
      colors[57 ] = color;
      colors[58 ] = color;
 
      klok.write(colors, LED_COUNT);

}

bool runShowTemp() {
  writeTemp(dimmLed(RED,dimm));
  unsigned char mills = 0;
  unsigned char secs = 10;
  while(secs > 0) {
    if (keypad.getKey()) {
      return false;
    }
    delay(10);
    if (mills++ > 100) {
      secs--;
      mills=0;
    }
  }
  return true;
}
  
bool runShowDate() {
  allesAan(OFF);
  showDate(dag,maand,jaar, dimm);
  unsigned char mills = 0;
  unsigned char secs = 10;
  while(secs > 0) {
    if (keypad.getKey()) {
      return false;
    }
    delay(10);
    if (mills++ > 100) {
      secs--;
      mills=0;
    }
  }
  return true;
}
