/*
Eerste setup van timer. Gebruik gemamaakt van wifi bordje met 32*128 display.
bord: NodeMCU1.0 (ESP-12E module)
Adres 0x3d
12c 5, 4
reset 16

Tweede

dispaly 64x128
i2c D1, D2
Address 0x3c

LET OP de adressen van de 12C kunnen omgedraaid zijn

voor juise library etc. https://github.com/ThingPulse/esp8266-oled-ssd1306


*/

#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Time.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <Wire.h>



// init the display
#define ROWS               8                                  // aantal rijen in scherm
#define COLS              25                                  // aantal kolommen in scherm
#define UPDATE_INTERVAL   300                                 // aantal seconden waarop nieuwe tijd moet worden opgehaald
String  ssid            = "";                                 // your network SSID (name)
//char pass[]             = "** PWD HERE **";                 // your network password
unsigned int localPort  = 2390;                               // local port to listen for UDP packets
time_t prevDisplay      = 0;                                  // when the digital clock was displayed
time_t lasTimeFetch     = 0;                                  // when the digital clock was displayed
String lines[ROWS];                                           // lines to display on lcd
char lineNr             = 0;                                  // linenumbers, needed for scrolling over list
bool setupCompleted     = false;                              // setup complet
char delta              = 2;                                  // tijdsverschil tussen huidige tijd en utc tijd in uren

/*  Don't hardwire the IP address or we won't get the benefits of the pool.
 *  Lookup the IP address for the host name instead 
 */
IPAddress   timeServerIP;                                     // time.nist.gov NTP server address
const char* ntpServerName = "nl.pool.ntp.org";                // "time.nist.gov";
const int   NTP_PACKET_SIZE = 48;                             // NTP time stamp is in the first 48 bytes of the message
byte        packetBuffer[ NTP_PACKET_SIZE ];                  // buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP   udp;
IPAddress ip;

void setup()
{
  emptyLines();                     // zorg dat tekstbuffer beschikbaar en nu ff leeg is
  Serial.begin(115200);
  Wire.begin(D1, D2);  
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              
  oled.setFont(font5x7);            
  testDisplay(true);
  testDisplay(false);

  wifiSetup();
  timeserverSetup();
  updSetup();
  
  displaySettings();
  setupCompleted = true;
}

void loop()
{
 if(timeStatus() != timeNotSet) 
    // here if the time has been set
    if( now() != prevDisplay) //update the display only if the time has changed
    {
      prevDisplay = now();
      doDisplayDateTime(prevDisplay);
      doDisplayLocalTime(prevDisplay, delta);  
    }
}





