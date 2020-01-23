boolean isSameColor(rgb_color c1,rgb_color c2) {
  return c1.red   == c2.red   &&
         c1.green == c2.green &&
         c1.blue  == c2.blue;
}

rgb_color randomColor(int maxBrightness) {
  char r = 0, g = 0, b = 0;
  while(r == 0 && g == 0 & b == 0) {
    r = random(min(maxBrightness + 1, 255)); 
    g = random(min(maxBrightness + 1, 255)); 
    b = random(min(maxBrightness + 1, 255)); 
  }
  return rgb_color(r, g, b);
}



MatrixOn getMatrixArray(boolean value) {
  MatrixOn m;
  for(int i = 0; i < MATRIX_COUNT; i++) {
    m.m[i] = value;
  }
  return m;
}

MatrixOn getRandomMatrixArray() {
  MatrixOn m = getMatrixArray(false);
  while(allMatrixesOf(m)) {
    for(int i = 0; i < MATRIX_COUNT; i++) {
      m.m[i] = random(1001) >= 500;
      Serial.print(i, DEC);
      Serial.print(" wordt ");
      Serial.println(m.m[i], DEC);
    }
  }
  Serial.print("random matrix: ");
  for(int i = 0; i < MATRIX_COUNT; i++) {
    Serial.print(" matrix ");
    Serial.print(i, DEC);
    Serial.print(m.m[i] ? " ON  " : " OFF ");
  }
  Serial.println("");

  return m;
}

boolean allMatrixesOf(MatrixOn m) {
  for(int i = 0; i < MATRIX_COUNT; i++) {
    if (m.m[i]) {
      return false;
    }
  }
  return true;
}

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

void cijfer(MatrixOn matrix, char cijfer, rgb_color color, boolean draw) {
byte *patroon = (CIJFERS[cijfer]);
  for (char m = 0; m < MATRIX_COUNT; m++) {
    if(matrix.m[m]) {
      for(byte r = 0; r < 9; r++) {
        byte row = patroon[r];
        for (int c = 0; c < 8; c++) {
          rgb_color aanUit = UIT;
          if (((int)pow(2,c) & (int)row) == (int)pow(2,c)) {
            aanUit = color;
          }
          // LET OP 1ste bit is links boven, in tegenstelling tot de bits B0000001
          colors[m * LEDS_PER_MATRIX + r * COLS + 7 - c] = aanUit;
        } 
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void telAf(MatrixOn  matrix, int aantal) {
  for (int i = aantal; i > 0; i--) {
    cijfer(matrix, i, rgb_color(dim, 0, 0), true);
    delay(1000);
  }
  uit(matrix, true);
}

void getal(int in, rgb_color color, boolean draw) {
  MatrixOn matrix[MATRIX_COUNT];
  char out[MATRIX_COUNT];
  int deler = 1;  
  for(char i = 0; i < MATRIX_COUNT; i++) {
    matrix[i] = getMatrixArray(false);
    matrix[i].m[i] = true;
    out[MATRIX_COUNT - 1 - i] = (in / deler) % 10;
    deler *= 10; 
  }
  for(char i = 0; i < MATRIX_COUNT; i++) {
    cijfer(matrix[i], out[i], color, draw && i == MATRIX_COUNT -1);
  }
}


// functie losgetrokken om de boel makkelijker te kunnen manipuleren.
void doWrite() {
  ledStrip.write(colors, LED_COUNT);
}



