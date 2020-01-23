
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include "DHT.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// voorlopig geen touchscreen, werkt nog niet goed

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

#define DISPLAY_DELAY 10000

#define SUMMER_COLD   23 // minimaal in de zomer
#define SUMMER_HOT    26 // maximaal in de zomer
#define WINTER_COLD   20 // minimaal in de winter
#define WINTER_HOT    23 // maximaal in de winter
#define HUMIDITY_MAX  70
#define HUMIDITY_MIN  30

// Assign human-readable names to some common 16-bit color values:
#define BACKGROUND  0x0000
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFDF0

// locaties van teksten
#define LEFT        30
#define LEFT_TEMP   20
#define REGEL_1     30
#define REGEL_2     70  // totaal r1
#define REGEL_3     80  // totaal, r2
#define REGEL_4    110  // vocht
#define REGEL_5    165  // temp



#define DHTPIN_0 54     // what digital pin we're connected to
#define DHTPIN_1 53     // what digital pin we're connected to
#define UNABLE_TO_READ_SENSOR "Unable to read sensor!"

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht0(DHTPIN_0, DHTTYPE);
DHT dht1(DHTPIN_1, DHTTYPE);


/**
 * Struct om alles in te beware
 */
struct DHT_STRUCT {
  float humidity;
  float tempCelcius;
  float tempFahrenheit;
  float heatIndexCelcius;
  float heatIndexFahrenheit;
  String humidityString;
  String tempCelciusString;
  String tempFahrenheitString;
  String heatIndexCelciusString;
  String heatIndexFahrenheitString;
};

void setup() {
  /*
    Als serial.begin aangeroepen wordt dan kan de sensor op poort 1 niet gelezen worden
  */
  Serial.begin(250000);
  Serial.println(F("Paint!"));
  
  tft.reset();

  // *** SPFD5408 change -- Begin
  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
  // geen idee, maar -45 en 45 zijn de liggende varianten en 90 en -90 de staande
  tft.setRotation(-45); // Need for the Mega, please changed for your choice or rotation initial

  // Border
  drawBorder();
  setText(LEFT, REGEL_1, 3, GREEN, "Vocht en temp");

  dht0.begin();
  dht1.begin();


}

String humidity = "";
String temprature = "";
String heatIndex = "";
String total = "";

DHT_STRUCT d1, d2;

String humidityOld, tempOld;
bool hasError = false;

void loop() {
  // Wait a few seconds between measurements.
  boolean res = readDht(&dht1, &d1);
  
  if (!res) {
    if (hasError) {
      delay(1000);
      return;
    }
    hasError = true;
    cleanAll();
    setText(LEFT, REGEL_2, 2, RED, UNABLE_TO_READ_SENSOR);
//    String msg = "check INPUT (port nr. " + DHTPIN_1 + ")";
//    setText(LEFT, REGEL_3, 2, msg);
    return;
  }

  Serial.println("Old temp = " + temprature + "\toldHumidity = " + humidity);
  if (humidity != d1.humidityString || temprature != d1.tempCelciusString) {
    cleanAll();
    hasError = false;
    humidity   = d1.humidityString;
    temprature = d1.tempCelciusString;
    Serial.println("Ongelijk, dus tekenen maar: temp = " + temprature + "\toldHumidity = " + humidity);

  // is het te harden
  fillTrafficLight(d1.tempCelcius, d1.humidity);
    
    setText(LEFT_TEMP, REGEL_4, 4, humidityColor(d1.humidity), humidity);
    setText(LEFT_TEMP, REGEL_5, 4, tempColor(d1.tempCelcius), temprature);
  
    total = "";
    total += "Temp       : ";
    total += d1.tempCelciusString;
    total += ", ";
    total += d1.tempFahrenheitString;
    setText(LEFT, REGEL_2, 1, WHITE, total);
    heatIndex = "";
    heatIndex += "Heat index : ";
    heatIndex += d1.heatIndexCelciusString;
    heatIndex += ", ";
    heatIndex += d1.heatIndexFahrenheitString;
    setText(LEFT, REGEL_3, 1, WHITE, heatIndex);
  }
  delay(DISPLAY_DELAY);

}

int humidityColor(int humidity) {
  if (humidity > HUMIDITY_MAX) {
    return RED;  
  }
  if (humidity < HUMIDITY_MIN) {
    return RED;
  }
  return GREEN;
}

int tempColor(float temp) {
  if (temp > SUMMER_HOT) {
    return RED; 
  }
  if (temp > SUMMER_COLD) {
    return ORANGE;
  }
  if (temp < WINTER_COLD) {
    return BLUE;
  }
  return GREEN;
}

void drawBorder () {

  // Draw a border

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(YELLOW);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), BACKGROUND);
  setText(border, height - border + 2, 1, BLUE, "Snels Software (c) 2017");

}

#define TR_L_TOP     85
#define TR_L_BORDER   5
#define RADIUS       20
#define TR_L_X      240
#define RED_Y       TR_L_TOP
#define ORANGE_Y    TR_L_TOP + 2 * (RADIUS + 2)
#define GREEN_Y     TR_L_TOP + 4 * (RADIUS + 2)

void drawTrafficLight() {
  tft.fillCircle(TR_L_X, RED_Y   , RADIUS + TR_L_BORDER, WHITE );
  tft.fillCircle(TR_L_X, GREEN_Y , RADIUS + TR_L_BORDER, WHITE );
  tft.fillRect(TR_L_X - RADIUS - TR_L_BORDER
             , RED_Y
             , (RADIUS + TR_L_BORDER) * 2 + 1
             , GREEN_Y - RED_Y
             , WHITE);
    
  tft.fillCircle(TR_L_X, RED_Y   , RADIUS, BACKGROUND );
  tft.fillCircle(TR_L_X, ORANGE_Y, RADIUS, BACKGROUND );
  tft.fillCircle(TR_L_X, GREEN_Y , RADIUS, BACKGROUND );
  
  tft.drawCircle(TR_L_X, RED_Y   , RADIUS, RED   );
  tft.drawCircle(TR_L_X, ORANGE_Y, RADIUS, ORANGE);
  tft.drawCircle(TR_L_X, GREEN_Y , RADIUS, GREEN );
}

void fillTrafficLight(float temp, float humidity) {
  if (temp > SUMMER_HOT || temp < WINTER_COLD) {
    tft.fillCircle(TR_L_X, RED_Y   , RADIUS, RED   );
    return;
  }
  if (humidity > HUMIDITY_MAX || humidity < HUMIDITY_MIN) {
    tft.fillCircle(TR_L_X, RED_Y   , RADIUS, RED   );
    return;
  }
  if (temp > SUMMER_COLD) {
    tft.fillCircle(TR_L_X, ORANGE_Y, RADIUS, ORANGE);
    return;
  }
  tft.fillCircle(TR_L_X, GREEN_Y , RADIUS, GREEN );
}

void setText(int x, int y, int size, long color,  String text) {
  tft.setCursor (x, y);
  tft.setTextSize (size);
  tft.setTextColor(color);
  tft.println(text);
}

/**
 * lees waarde uit uit de sensor en kopieer ze naar de globalen
 * return false als meter nog niet geschik is om te lezen
 */
boolean readDht(DHT* dht, DHT_STRUCT* d) {
  float h = dht->readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht->readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht->readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    setText(LEFT, REGEL_2, 2, RED, UNABLE_TO_READ_SENSOR);
    return false;
  }
  d->humidity = h;
  d->tempCelcius = t;
  d->tempFahrenheit = f;
  d->heatIndexCelcius = dht->computeHeatIndex(t, h, false);
  d->heatIndexFahrenheit = dht->computeHeatIndex(f, h);

  // ook de teksten maar meteen vullen
  d->humidityString = makeString(h,"%");
  d->tempCelciusString = makeString(t,"C");
  d->tempFahrenheitString = makeString(f,"F");
  d->heatIndexCelciusString =  makeString(d->heatIndexCelcius,"C");
  d->heatIndexFahrenheitString = makeString(d->heatIndexFahrenheit,"F");

  
  return true;
}

String makeString(float in, String unit) {
  String res = "";
  res += in;
  res += unit;
  return res;
}

void cleanAll() {
  if (hasError) {
    setText(LEFT,       REGEL_2, 2, BACKGROUND, UNABLE_TO_READ_SENSOR);
  }
  if (humidity != d1.humidityString) {
    setText(LEFT,       REGEL_3, 1, BACKGROUND, heatIndex);
    setText(LEFT_TEMP,  REGEL_4, 4, BACKGROUND, humidity);
  }
  if (temprature != d1.tempCelciusString) {
    setText(LEFT,       REGEL_2, 1, BACKGROUND, total);
    setText(LEFT_TEMP,  REGEL_5, 4, BACKGROUND, temprature);
    setText(LEFT,       REGEL_3, 1, BACKGROUND, heatIndex);
  }

  drawTrafficLight();
}

