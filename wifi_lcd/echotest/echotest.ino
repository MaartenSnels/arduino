#include <Arduino.h>
#include <U8g2lib.h>
#include "ESP8266WiFi.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// init the display
U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0
                              , /* clock=*/ 5
                              , /* data=*/  4
                              , /* reset=*/ 16);   // Adafruit Feather M0 Basic Proto + FeatherWing OLED


void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void setup(void) {
  Serial.begin(115200);
  // setup display
  u8g2.begin();
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  println("Setup done");
}

void loop(void) {
  println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  char n = WiFi.scanNetworks();
  println("scan done");
  if (n == 0)
    println("no networks found");
  else
  {
    println(String(n) + " networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      String out  = String(i + 1) + ": " + WiFi.SSID(i)+ " (" + WiFi.RSSI(i)+ ")" + ((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      println(out);
      delay(100);
    }
  }
}

void println(String in) {
  Serial.println(in);
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.drawUTF8(10, 8, "regel tekst");    // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(100);
}


