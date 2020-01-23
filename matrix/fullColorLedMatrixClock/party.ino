// knipper
// politie
// radomMatrix
// linksRechts
// bovenBeneden
// diagonaalAanUit
// boxInOut
// propeller

#define PARTIES_MATRIX  15
#define PARTIES         15

////////////// ALL PARTIES /////////////////////
void doParty() {
  if (random(1001) < 500) {
    partiesPerMatrix();
  } else {
    oneBigParty();
  }
}

void partiesPerMatrix() {
  MatrixOn m = getRandomMatrixArray();
  oneParty(m);
}

void oneBigParty() {
  char party = random(PARTIES);
  Serial.print("party for one: ");
  Serial.println(party, DEC);
  switch (party) {
    case 0:
      politie(aantal, _delay * 3);
      break;
    case 1:
      knipper(aantal, rgb_color(0, dim, 0), _delay);
      break;
    case 2:
      linksRechts(rgb_color(dim, dim, dim), _delay);
      break;
    case 3:
      bovenBeneden(rgb_color(dim, dim, dim), _delay);
      break;
    case 4:
      perPixel(false, rgb_color(dim, 0, dim), _delay / 10);
      perPixel(false, UIT, _delay / 10);
      break;
    case 5:
      perPixel(true, rgb_color(dim, 0, dim), _delay / 10);
      uit(false);
      break;
    case 6:
      diagonaalAanUit(rgb_color(dim, dim, 0), _delay / 2);
      break;
    case 7:
      diagonalen(aantal, _delay / 2);
      break;
    case 8:
      boxes(_delay * 2);
      break;
    case 9:
      randomMatrix(dim, 1000, 0);
      uit(false);
      break;
    case 10:
      countUp(_delay/10);
      uit(false);
      break;
    case 11:
      race(_delay);
      uit(false);
      break;
    case 12:
      randomAanUit(true, randomColor(dim), 0);
      randomAanUit(false, UIT, 0);
      break;
    case 13:
      twist(25, _delay * 3);
      break;
    case 14:
      randomAanRandomColor(0);
      randomAanUit(false, UIT, 0);
      break;
  }
  
}

void oneParty(MatrixOn m) {
  char party = random(PARTIES_MATRIX);
  Serial.print("party for more: ");
  Serial.println(party, DEC);
  switch (party) {
    case 0:
      politie(m, aantal, _delay * 3);
      break;
    case 1:
      knipper(m, aantal, rgb_color(0, dim, 0), _delay);
      break;
    case 2:
      linksRechts(m, rgb_color(dim, dim, dim), _delay);
      break;
    case 3:
      bovenBeneden(m, rgb_color(dim, dim, dim), _delay);
      break;
    case 4:
      perPixel(m, false, rgb_color(dim, 0, dim), _delay / 10);
      perPixel(m, false, UIT, _delay / 10);
      break;
    case 5:
      diagonaalAanUit(m, rgb_color(dim, dim, 0), _delay / 2);
      break;
    case 6:
      diagonalen(m, aantal, _delay / 2);
      break;
    case 7:
      boxes(m, _delay * 2);
      break;
    case 8:
      propeller(m, aantal * 2, rgb_color(dim, dim, 0), _delay);
      break;
    case 9:
      randomMatrix(m, dim, 1000, 0);
      uit(m, false);
      break;
    case 10:
      countUp(m, _delay);
      uit(m, false);
      break;
    case 11:
      perPixel(m, true, rgb_color(dim, 0, dim), _delay / 10);
      break;
    case 12:
      randomAanUit(m, true, randomColor(dim), 0);
      randomAanUit(m, false, UIT, 0);
      break;
    case 13:
      twist(m, 25, _delay * 3);
      break;
    case 14:
      randomAanRandomColor(m, 0);
      randomAanUit(false, UIT, 0);
      break;

  }
}

void partiesMatrix(MatrixOn m) {
  politie(m, aantal, _delay * 3);
  knipper(m, aantal, rgb_color(0, dim, 0), _delay);
  linksRechts(m, rgb_color(dim, dim, dim), _delay);
  bovenBeneden(m, rgb_color(dim, dim, dim), _delay);
  perPixel(m, false, rgb_color(dim, 0, dim), _delay / 10);
  perPixel(m, false, UIT, _delay / 10);
  perPixel(m, true, rgb_color(dim, 0, dim), _delay / 10);
  diagonaalAanUit(m, rgb_color(dim, dim, 0), _delay / 2);
  diagonalen(m, aantal, _delay / 2);
  boxes(m, _delay * 2);
  propeller(m, aantal * 2, rgb_color(dim, dim, 0), _delay);
  randomMatrix(m, dim, 1000, 0);
  countUp(m, _delay);
  uit(m, false);
}

void partiesOneBigMatrix() {
  politie(aantal, _delay * 3);
  knipper(aantal, rgb_color(0, dim, 0), _delay);
  linksRechts(rgb_color(dim, dim, dim), _delay);
  bovenBeneden(rgb_color(dim, dim, dim), _delay);
  perPixel(false, rgb_color(dim, 0, dim), _delay / 10);
  perPixel(false, UIT, _delay / 10);
  perPixel(true, rgb_color(dim, 0, dim), _delay / 10);
  diagonaalAanUit(rgb_color(dim, dim, 0), _delay / 2);
  diagonalen(aantal, _delay / 2);
  boxes(_delay * 2);
  randomMatrix(dim, 1000, 0);
  countUp(_delay/10);
  uit(false);
}

void diagonaalLR(rgb_color aan, rgb_color uit, boolean lr, int _delay) {
  int max = MATRIX_COUNT * COLS + ROWS - 2;
  for (int i = 0; i <= max; i++) {
    int d = i;
    if (!lr) {
      d = max - i;
    }
    diagonal(d, aan, true);
    delay(_delay);
    diagonal(d, uit, false);
  }
}

void  diagonaalAanUit(rgb_color color, int _delay) {
  diagonaalLR(color, color, true, _delay);
  diagonaalLR(color, UIT, true, _delay);
}

void diagonaalLR(MatrixOn matrix, rgb_color aan, rgb_color uit, boolean lr, int _delay) {
  int max = COLS + ROWS - 1;
  for (int i = 0; i <= max; i++) {
    int d = i;
    if (!lr) {
      d = max - i;
    }
    diagonal(matrix, d, aan, true);
    delay(_delay);
    diagonal(matrix, d, uit, false);
  }
}

void  diagonaalAanUit(MatrixOn matrix, rgb_color color, int _delay) {
  diagonaalLR(matrix, color, color, true, _delay);
  diagonaalLR(matrix, color, UIT, true, _delay);
}


void linksRechts(rgb_color color, int _delay) {
  for (int i = 0; i < COLS * MATRIX_COUNT; i++) {
    col(i, color, true);
    delay(_delay);
  }
  for (int i = COLS * MATRIX_COUNT - 1; i >= 0; i--) {
    col(i, UIT, true);
    delay(_delay);
  }
}

void linksRechts(MatrixOn matrix, rgb_color color, int _delay) {
  for (int i = 0; i < COLS; i++) {
    col(matrix, i, color, true);
    delay(_delay);
  }
  for (int i = COLS - 1; i >= 0; i--) {
    col(matrix, i, UIT, true);
    delay(_delay);
  }
}
  
void  bovenBeneden(rgb_color color, int _delay) {
  for (int i = 0; i < ROWS; i++) {
    row(i, color, true);
    delay(_delay);
  }
  for (int i = ROWS - 1; i >= 0; i--) {
    row(i, UIT, true);
    delay(_delay);
  }
}

void  bovenBeneden(MatrixOn matrix, rgb_color color, int _delay) {
  for (int i = 0; i < ROWS; i++) {
    row(matrix, i, color, true);
    delay(_delay);
  }
  for (int i = ROWS - 1; i >= 0; i--) {
    row(matrix, i, UIT, true);
    delay(_delay);
  }
}


void knipper(MatrixOn matrix, int aantal, rgb_color color, int _delay) {
  for (int i = 0; i < aantal; i++) {
    aan(matrix, color, true);
    delay(_delay);
    uit(matrix, true);
    delay(_delay);
  }
}
void knipper(int aantal, rgb_color color, int _delay) {
  for (int i = 0; i < aantal; i++) {
    aan(color, true);
    delay(_delay);
    uit(true);
    delay(_delay);
  }
}

void politie(int aantal, int _delay) {
  for(int i = 0; i < aantal; i++) {
    step(0,2,rgb_color(dim, 0, 0), true);
    delay(_delay);
    uit(false);
    step(1, 2, rgb_color(0, 0, dim), true);
    delay(_delay);
    uit(false);
  }
}

void politie(MatrixOn matrix, int aantal, int _delay) {
  for(int i = 0; i < aantal; i++) {
    evenAan(matrix, rgb_color(dim, 0, 0), true);
    delay(_delay);
    uit(matrix, false);
    onevenAan(matrix, rgb_color(0, 0, dim), true);
    delay(_delay);
    uit(matrix, false);
  }
}

void randomMatrix(MatrixOn matrix, int maxBrightness, int aantal, int _delay) {
  for (int i = 0; i < aantal; i++) {
    rgb_color color = UIT;
    if (random(3) == 0) {
        color = randomColor(maxBrightness);
    }
    randomPixel(matrix, color, true);
    delay(_delay);
  }
}

void randomMatrix(int maxBrightness, int aantal, int _delay) {
  for (int i = 0; i < aantal; i++) {
    rgb_color color = UIT;
    if (random(3) == 0) {
        color = randomColor(maxBrightness);
    }
    randomPixel(color, true);
    delay(_delay);
  }
}

void perPixel(MatrixOn matrix, boolean uit, rgb_color color, int _delay) {
  for(int p = 0; p < LEDS_PER_MATRIX; p++) {
    for (char m = 0; m < MATRIX_COUNT; m++) {
      if (matrix.m[m]) {
        colors[p + LEDS_PER_MATRIX * m] = color; 
      }
    }
    doWrite();
    delay(_delay);
    for (char m = 0; m < MATRIX_COUNT; m++) {
      if (matrix.m[m] && uit) {
        colors[p + LEDS_PER_MATRIX * m] = UIT; 
      }
    }
    doWrite();
    delay(_delay);
  }
}

void perPixel(boolean uit, rgb_color color, int _delay) {
  for(char r = 0; r < ROWS; r++) {
    for(char c = 0; c < COLS * MATRIX_COUNT; c++) {
      pixel(c, r, color, true);
      delay(_delay);
      if (uit) {
        pixel(c, r, UIT, true);
        delay(_delay);
      }
    }
  }
}

void boxes(MatrixOn matrix, int _delay) {
  boxInOut(matrix, true, rgb_color(dim, 0, 0), _delay);
  boxInOut(matrix, true, UIT, _delay);
  boxInOut(matrix, true, rgb_color(0, dim, 0), _delay);
  boxInOut(matrix, false, UIT, _delay);
  boxInOut(matrix, false, rgb_color(0, 0, dim), _delay);
  boxInOut(matrix, true, UIT, _delay);
  boxInOut(matrix, false, rgb_color(dim, 0, dim), _delay);
  boxInOut(matrix, false, UIT, _delay);
}

void boxes(int _delay) {
  boxInOut(true, rgb_color(dim, 0, 0), _delay);
  boxInOut(true, UIT, _delay);
  boxInOut(true, rgb_color(0, dim, 0), _delay);
  boxInOut(false, UIT, _delay);
  boxInOut(false, rgb_color(0, 0, dim), _delay);
  boxInOut(true, UIT, _delay);
  boxInOut(false, rgb_color(dim, 0, dim), _delay);
  boxInOut(false, UIT, _delay);
}


void boxInOut(MatrixOn matrix, bool uitIn, rgb_color color, int _delay) {
  for(int i = 0; i <= 3 ; i++) {
    char j = uitIn ? i : 3 - i;
    box(matrix, j, j, ROWS - 1 - j, COLS - 1 - j, color, false, true);
    delay(_delay);
  }
}

void boxInOut(boolean uitIn, rgb_color color, int _delay) {
  for(int i = 0; i < 4; i++) {
    char j = uitIn ? i : 3 - i;
    box(3 - j, 3 - j, 4 + j, COLS * MATRIX_COUNT - 4 + j, color, true, true);
    delay(_delay);
  }
}

void countUp(int _delay) {
  int start = random(9900);
  rgb_color color = randomColor(dim);
  for (int i = 0; i<= 99; i++) {
    if (i % 10 == 0) {
      color = randomColor(dim);
    }
    getal(start + i, color, _delay);
    delay(_delay );
  }
  uit(true);
}

void countUp(MatrixOn matrix, int _delay) {
  rgb_color color = randomColor(dim);
  for (int i = 0; i<= 9; i++) {
    cijfer(matrix, i, color, true);
    delay(_delay );
  }
  uit(matrix, false);
}

void propeller(MatrixOn matrix, int aantal, rgb_color color, int _delay) {
  for (int i = 0; i<aantal; i++) {
    slash(matrix, true, color, true);
    delay(_delay);
    slash(matrix, true, UIT, false);
    row(matrix, 3, color, false);
    row(matrix, 4, color, true);
    delay(_delay);
    row(matrix, 3, UIT, false);
    row(matrix, 4, UIT, false);
    slash(matrix, false, color, true);
    delay(_delay);
    slash(matrix, false, UIT, false);
    col(matrix, 3, color, false);
    col(matrix, 4, color, true);
    delay(_delay);
    col(matrix, 3, UIT, false);
    col(matrix, 4, UIT, false);
  }
}

void diagonalen(int aantal, int _delay) {
  for(int i = 0; i < aantal; i++) {
    diagonaalLR(randomColor(dim), randomColor(dim), random(2), _delay);
  }
  diagonaalLR(randomColor(dim), UIT, random(2), _delay);
}

void diagonalen(MatrixOn matrix, int aantal, int _delay) {
  for(int i = 0; i < aantal; i++) {
    diagonaalLR(matrix, randomColor(dim), randomColor(dim), random(2), _delay);
  }
  diagonaalLR(matrix, randomColor(dim), UIT, random(2), _delay);
}

void race(int _delay) {
  rgb_color deelnemers[ROWS];
  deelnemers[0] = rgb_color(dim, dim/2, dim/2 );
  deelnemers[1] = rgb_color(dim, 0    ,   0   );
  deelnemers[2] = rgb_color(0  , dim  ,   0   );
  deelnemers[3] = rgb_color(0  , 0    , dim   );
  deelnemers[4] = rgb_color(dim, dim  ,   0   );
  deelnemers[5] = rgb_color(dim, 0    , dim   );
  deelnemers[6] = rgb_color(0  , dim  , dim   );
  deelnemers[7] = rgb_color(dim, dim  , dim   );
  uit(false);
  for (int i = 0; i < ROWS; i++) {
    pixel(0, i, deelnemers[i], i == ROWS - 1);
  }
  MatrixOn matrix = getMatrixArray(false);
  matrix.m[MATRIX_COUNT - 1] = true;
  telAf(matrix, 3);
  int rows[ROWS];
  for (int i = 0; i < ROWS; i++) {
    rows[i]=0;
  }
  int winner = -1;
  while(winner < 0) {
    char r = random(ROWS);
    int pos = rows[r] + 1;
    rows[r] = pos;
    pixel(pos, r, deelnemers[r], true);
    delay(_delay);
    if (pos == ROWS * MATRIX_COUNT -1) {
      winner = r;
    }
  }
  for (int i = 0; i < ROWS; i++) {
    if (i != winner) {
      row(i, UIT, true);
      delay(100);
    }
  }
  for (int i = 0; i < 20; i++) {
      rgb_color color = i % 2 == 0 ? deelnemers[winner] : UIT;
      row(winner, color, true);
      delay(100);
  }
  row(winner, deelnemers[winner], true);
  randomAanUit(false, UIT, _delay * 2);
}

void twist(int aantal, int _delay) {
  rgb_color color[3];
  color[0] = rgb_color(dim, 0,   0);
  color[1] = rgb_color(0,   dim, 0);
  color[2] = rgb_color(dim, dim, 0);

  for (int i = 0; i < aantal; i++) {
    step(0, 3, color[i % 3      ], false);
    step(1, 3, color[(i + 1) % 3], false);
    step(2, 3, color[(i + 2) % 3], true);
    delay(_delay);
  }
  uit(false);
}



void twist(MatrixOn matrix, int aantal, int _delay) {
  rgb_color color[3];
  color[0] = rgb_color(dim, 0,   0);
  color[1] = rgb_color(0,   dim, 0);
  color[2] = rgb_color(dim, dim, 0);

  for (int i = 0; i < aantal; i++) {
    step(matrix, 0, 3, color[i % 3      ], false);
    step(matrix, 1, 3, color[(i + 1) % 3], false);
    step(matrix, 2, 3, color[(i + 2) % 3], true);
    delay(_delay);
  }
  uit(false);
}








