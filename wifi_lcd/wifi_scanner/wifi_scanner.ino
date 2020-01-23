#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#define ROWS 4   // aantal rijen in scherm
#define COLS 16  // aantal kolommen in scherm

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

void setup(void)
{
  Serial.begin(115200);
  u8x8.begin();
  u8x8.initDisplay();
  u8x8.setPowerSave(0);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  println("Setup done");
}

void loop() {
  println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  println("scan done");
  if (n == 0)
    println("none found");
  else
  {
    println(String(n) + " found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      println(String(i + 1) + ": " + WiFi.SSID(i));// + " (" + WiFi.RSSI(i) + ")" + ((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*"));
    }
  }
  println("----------------");
}

String lines[] = {"", "", "", ""};
char lineNr = 0;
void addLine(String in) 
{
  if (lineNr < ROWS) {
    lines[lineNr++] = in;
    return;
  }
  
  for (int i = 0; i < ROWS - 1; i++) {
    lines[i] = lines[i + 1];
  }
  lines[ROWS - 1] = in;
}

void println(String in)
{
  Serial.println(in);
  addLine(in);
  u8x8.clear();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.setCursor(0,0);
  for(int j = 0; j < ROWS; j++) {
    
    u8x8.print(String(lines[j].substring(0,COLS - 1)));
    u8x8.print("\n");
  }
  delay(1000);
  
}
