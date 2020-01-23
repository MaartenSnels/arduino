/*\

      voor de klok:

      -------- -------- -------- --------
      |      | |      | |      | |      |  
      | M0   | | M1   | | M2   | | M3   |  
      |      | |      | |      | |      |  
      |      | |      | |      | |      |  
      -------- -------- -------- --------

Een enkele matrix:

    0   1  2  3  4  5  6  7 -> col
   ---------------------------   
0  | 00 01 02 03 04 05 06 07 |
1  | 08 09 10 11 12 13 14 15 |
2  | 16 17 18 19 20 21 22 23 |
3  | 24 25 26 27 28 29 30 31 |
4  | 32 33 34 35 36 37 38 39 |
5  | 40 41 42 43 44 45 46 47 |
6  | 48 49 50 51 52 53 54 55 |
7  | 56 57 58 59 60 61 62 63 |
   ---------------------------
row
*/

/******* TODO ***************

  - DONE  bordje draaien zodat ook 0 liks boven zit
  -       COLS -> COL_COUNT
  -       ROWS -> ROW_COUNT
  - DONE  alles relatief aan ROW_COUNT en COL_COUNT
  - BUSSY van enkele matrix naar matrix array
  -       evenAan en ovevenAan samnvoegen

*****************************/

/*

void uit() {
void uit(MatrixOn matrix)
void aan(rgb_color color)
void aan(MatrixOn matrix, rgb_color color)
void evenAan(rgb_color color)
void evenAan(MatrixOn matrix, rgb_color color)
void onevenAan(MatrixOn matrix, rgb_color color)
void onevenAan(rgb_color color)
void col(MatrixOn matrix, char column, rgb_color color)
void col(char column, rgb_color color)
void row(MatrixOn matrix, char row, rgb_color color)
void row(char row, rgb_color color)
void pixel(MatrixOn matrix, char col, char row, rgb_color color)
void pixel(char col, char row, rgb_color color)
void diagonal(MatrixOn matrix, char diagonal, rgb_color color)
void diagonal(char diagonal, rgb_color color)
void box(MatrixOn matrix, char row1, char col1, char row2, char col2, rgb_color color, boolean filled)
void box(char row1, char col1, char row2, char col2, rgb_color color, boolean filled)
void randomPixel(MatrixOn matrix, rgb_color color)
void randomPixel(rgb_color color)
void slash(MatrixOn matrix, bool forward, rgb_color color)


*/

void uit(boolean draw) {
  aan(UIT, draw);
}
void uit(MatrixOn matrix, boolean draw) {
  aan(matrix, UIT, draw);
}
void aan(rgb_color color, boolean draw) {
  for (int i = 0; i < LED_COUNT; i++) {
    colors[i] = color;    
  }
  if (draw) {
    doWrite();
  }
}
void aan(MatrixOn matrix, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      for (char i = 0; i < LEDS_PER_MATRIX; i++) {
        colors[m * LEDS_PER_MATRIX + i] = color;    
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void step(char start, char step, rgb_color color, boolean draw) {
  for (int i = start; i < LED_COUNT; i += step) {
    colors[i] = color;    
  }
  if (draw) {
    doWrite();
  }
}

void step(MatrixOn matrix, char start, char step, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      for (int i = start; i < LEDS_PER_MATRIX; i += step) {
        colors[i] = color;    
      }
    }
  }
  if (draw) {
    doWrite();
  }
}


void evenAan(rgb_color color, boolean draw) {
  step(0, 2, color, draw);
}

void evenAan(MatrixOn matrix, rgb_color color, boolean draw) {
  step(1, 2, color, draw);
}

void onevenAan(MatrixOn matrix, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      for (int i = m * LEDS_PER_MATRIX + 1; i < (m + 1) * LEDS_PER_MATRIX; i += 2) {
        colors[i] = color;    
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void onevenAan(rgb_color color, boolean draw) {
  step(1, 2, color, draw);
}

void col(MatrixOn matrix, char column, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      int start =  m * LEDS_PER_MATRIX + column; 
      for(int i = start; i < start + LEDS_PER_MATRIX; i += ROWS) {
        colors[i] = color;    
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void col(char column, rgb_color color, boolean draw) {
  int c = column % COLS;
  MatrixOn matrix = getMatrixArray(false);
  matrix.m[column / COLS] = true;
  col(matrix, c, color, draw);
}

void row(MatrixOn matrix, char row, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      int start =  m * LEDS_PER_MATRIX + row * COLS; 
      for(int i = start; i < start + COLS; i++) {
        colors[i] = color;    
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void row(char row, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    for(int r = row * COLS; r < (row + 1) * COLS; r++) {
      colors[r + m * LEDS_PER_MATRIX] = color;    
    }
  }
  if (draw) {
    doWrite();
  }
}

void pixel(MatrixOn matrix, char col, char row, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      colors[mrc(m, row, col)] = color;
    }
  }
  if (draw) {
    doWrite();
  }
}

void pixel(char col, char row, rgb_color color, boolean draw) {
  MatrixOn matrix = getMatrixArray(false);
  matrix.m[col / COLS] = true;
  char c = col % COLS;
  pixel(matrix, c, row, color, draw);
}

void pixel(char matrix, char col, char row, rgb_color color, boolean draw) {
  MatrixOn m = getMatrixArray(false);
  m.m[matrix] = true;
  pixel(m, col, row, color, draw);
}

void diagonal(MatrixOn matrix, char diagonal, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      int start =  m * LEDS_PER_MATRIX; 
      if (diagonal < 8) {
        start = start + diagonal;
        for(int i = 0; i <= diagonal; i++) {
          colors[start + i * 7] = color;
        }
      } else {
        start = start + (diagonal - 6) * 8 - 1; 
        for (int i = start; i < (m + 1) * LEDS_PER_MATRIX; i += 7) {
          colors[i] = color; 
        }
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void diagonal(char diagonal, rgb_color color, boolean draw) {
  char matrix = diagonal / COLS;
  char d = diagonal % COLS; 
  int led = matrix * LEDS_PER_MATRIX + d;
  char shift = false;
  for (int i = 0 ; i <= ROWS; i++) {
    if (led >= 0 && led < LED_COUNT) {
      colors[led] = color;
    }
    led += COLS - 1;
    if (i >= d  && !shift) {
      led -= (LEDS_PER_MATRIX - COLS);
      shift = true;
    }
  }
  if (draw) {
    doWrite();
  }
}

void box(MatrixOn matrix, char row1, char col1, char row2, char col2, rgb_color color, boolean filled, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      for (int r = row1; r <= row2; r++) {
        for (int c = col1; c <= col2; c++) {
          if (filled || r == row1 || r == row2 || c == col1 || c == col2) {
            colors[mrc(m, r, c)] = color;
          }
        }
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void box(char row1, char col1, char row2, char col2, rgb_color color, boolean filled, boolean draw) {
  char matrixStart = col1 / COLS;
  char matrixStop = col2 / COLS;
  for (char m = matrixStart; m <= matrixStop; m++) {
    for(char r = row1; r <= row2; r++) {
      char colStart = m == matrixStart ? col1 - m * COLS : 0; 
      char colStop  = m == matrixStop  ? col2 - m * COLS : COLS - 1;
      for (char c = colStart; c <= colStop; c++) {
          boolean draw =  filled                         || 
                         (r == row1 && m == matrixStart) || 
                         (r == row2 && m == matrixStop)  || 
                          c + m * COLS == col1           || 
                          c + m * COLS == col2;
          if (draw) {
            colors[mrc(m, r, c)] = color;
          }
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

void randomPixel(MatrixOn matrix, rgb_color color, boolean draw) {
  int pixel = random(LEDS_PER_MATRIX);
  for (char m = 0; m < MATRIX_COUNT; m++) {
    if (matrix.m[m]) {
      colors[pixel + m * LEDS_PER_MATRIX] = color;
    }
  }
  if (draw) {
    doWrite();
  }
}

void randomPixel(rgb_color color, boolean draw) {
  colors[random(LED_COUNT)] = color;
  if (draw) {
    doWrite();
  }
}

void slash(MatrixOn matrix, bool forward, rgb_color color, boolean draw) {
  for (char m = 0; m < MATRIX_COUNT; m++) {
    if (matrix.m[m]) {
      for (char r = 0; r < ROWS; r++) {
          char c = forward ? r : COLS - 1 - r;   
          colors[mrc(m, r, c)] = color;
      }
    }
  }
  if (draw) {
    doWrite();
  }
}

int mrc(char m, char r, char c) {
  return m * LEDS_PER_MATRIX + r * COLS + c;
}

void randomAanUit(MatrixOn matrix, boolean aan, rgb_color color, int _delay) {
  if (aan && isSameColor(color, UIT)) {
    Serial.println("gebruik kleur UIT niet als je randmAanUit gebruikt met de optie aan = true");
    return;
  }
  
  int number = 0;
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      for( int i = 0; i < LEDS_PER_MATRIX; i++) {
        if (isSameColor(colors[LEDS_PER_MATRIX * m + i], UIT) == aan) {
          number++;
        }
      }
    }
  }
  int pixel = 0;
  while (number > 0) {
    do {
      pixel = random(LED_COUNT);
      int inMatrix = (int)pixel / (int)LEDS_PER_MATRIX;
    } while (isSameColor(colors[pixel], UIT) != aan || !matrix.m[pixel / LEDS_PER_MATRIX]);
    number--;
    colors[pixel] = aan ? color : UIT;
    doWrite();
    delay(_delay);
  }
}

void randomAanRandomColor(MatrixOn matrix, int _delay) {
  int number = 0;
  for (char m = 0; m < MATRIX_COUNT ; m++) {
    if(matrix.m[m]) {
      for( int i = 0; i < LEDS_PER_MATRIX; i++) {
        if (isSameColor(colors[LEDS_PER_MATRIX * m + i], UIT)) {
          number++;
        }
      }
    }
  }
  int pixel = 0;
  while (number > 0) {
    do {
      pixel = random(LED_COUNT);
      int inMatrix = (int)pixel / (int)LEDS_PER_MATRIX;
    } while (!isSameColor(colors[pixel], UIT) || !matrix.m[pixel / LEDS_PER_MATRIX]);
    number--;
    colors[pixel] = randomColor(dim);
    doWrite();
    delay(_delay);
  }
}

void randomAanUit(boolean aan, rgb_color color, int _delay) {
  randomAanUit(getMatrixArray(true), aan, color, _delay);
}

void randomAanRandomColor(int _delay) {
  randomAanRandomColor(getMatrixArray(true), _delay);
}



