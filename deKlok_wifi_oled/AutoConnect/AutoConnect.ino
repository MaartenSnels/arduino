#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <Time.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#define OLED_RESET 16
Adafruit_SSD1306 display(OLED_RESET);


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // gebruik oled display voor ip addres
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    display.clearDisplay();
    display.setTextSize(1);
    testDisplay(true);
    testDisplay(false);


    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    IPAddress ip = IPAddress(10,0,1,1);
    wifiManager.setAPStaticIPConfig(ip, ip, IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    char ap[] = "deKlokWifiSetup";
    displayAp(ap, ip);
    wifiManager.autoConnect(ap);
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    

    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    Serial.println(wifiManager.getConfigPortalSSID());
    displayConnected(wifiManager.getConfigPortalSSID());
}

void loop() {
    // put your main code here, to run repeatedly:
    
}

void displayAp(String ap, IPAddress ip) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("con. to accesspoint");
  display.println(ap);
  display.println(ip.toString());
  display.display();
}

void displayConnected(String ssid) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("deKlok");
  display.println("CONNECTED!");
  display.display();
}

void testDisplay(boolean invert) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  if (invert) {
    display.setTextColor(BLACK, WHITE);
  } else {
    display.setTextColor(WHITE);
  }
  display.println("12345678901234567890");
  display.println("abcdefghijklmnopqrst");
  display.println("ABCDEFGHIJKLMNOPQRST");
  display.println("-- !Display OK! --");
  display.display();
  delay(1000);
   
}

