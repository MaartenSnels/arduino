/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

 /*
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC (that is: a RFID
 * Tag or Card) using a MFRC522 based RFID Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout
 * below), load this sketch into Arduino IDE then verify/compile and upload it.
 * To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M).
 * When you present a PICC (that is: a RFID Tag or Card) at reading distance
 * of the MFRC522 Reader/PCD, the serial output will show the ID/UID, type and
 * any data blocks it can read. Note: you may see "Timeout in communication"
 * messages when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs
 * presented (that is: multiple tag reading). So if you stack two or more
 * PICCs on top of each other and present them to the reader, it will first
 * output all details of the first and then the next PICC. Note that this
 * may take some time as all data blocks are dumped, so keep the PICCs at
 * reading distance until complete.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */


/*
    TODO
    -   DONE lijst met kaarten die toegestaan zijn
    -   DONE mogelijkheid om eenvoudig kaarten toe te voegen
    -   DONE mogelijkheid om eenvoudig kaarten te verwijderen
    - bij website wodt deur soms twee keer uitgevoerd
    -   DONE logging op memory card (SD.h vreet veel geheugen!) -> MEGA ipv UNO
    - Datum en tijd van internet halen
    -   DONE lamp tijdelijk aan bij openen en sluiten deur
    -   DONElamp via knop aan en uit kunnen zetten
    -   DONE let op bij tijdslimieten als millis > LONG_MAX wodt i weer 0, contorle millis() < oldMillis() kan dan HEEL lang duren
    - Laat zien als toevoegen van card gelukt is
    - Veel duidelijkere namen in de code
    - wissen van geheugen is niet echt wissen van geheugen, alleen de teller wordt op 0 gezet.
    - BUG lamp voor mastercard werkt niet meer goed (splitsen knipper??)
    - BUG lamp gaan aan bij het schakelen naar remove mode
    - BUG lamp gaat niet uit als geactiveerd door deur
*/

/*
 * Gebruik:
 * 
 * Voer in de code een mastercard in, deze kan kaarten toevoegen en verwijderen, maar zelf de deur niet openen
 * 
 * -- als je alles op internet wil kunnen volgen
 *    voor een mac adres in
 *    zorg voor een vast ip adres 
 *    
 *    vraag alle sleutel op ip_adres/?keys
 *    
 * -----------------------------------------------
 * 
 * Voeg kaart toe
 *    1 houw mastercard voor de lezer
 *    2 led gaat constant branden
 *    3 hou toe te voegen kaart voor lezer
 *    4 herhaal vorige stappen tot je klaar ben
 *    5 hou master voor de lezer.
 *    
 *    LET OP na 10 sec. geen activitiet stopt het automatisch
 *    
 * Verwijder kaart
 *    1 druk op de kaart verwijder knop en houdt die ingedrukt
 *    3 show mastercard
 *    3 lamp knippert langzaam
 *    4 hou kaart voor scanner die verwijderd moet worden
 *    
 *  Alle kaarten verwijdferen
 *    1 houd lamp aan (in de garage) 10 seconden ingedrukt
 *    2 deur ledje gaat knipperen
 *    3 gegeugen wordt gewist
 * 
 * 
 * 
 * knipperen van ledje 
 *  bij de lezer
 *    10 keer (2 seconden) kaart niet correct
 *    10 keer heel snel (ziet er uit als een korte flits0 lezen geslaagd
 *    
 *  bij kaart toevoegen
 *    brand constant -> je zit in toevoeg modus
 *    10 keer snel (1 seconde) kaart is al toegevoegd
 *    
 */

#define __DEBUG__                                           // print o.a. ID's van kaarten


#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <Time.h>
#include <SD.h>
#include <EEPROM.h>
#include "keyMemory.h"

// acties
#define LAMPAAN     "lampAan"                               // website knop lamp aan gedrukt
#define LAMPUIT     "lampuit"                               // website knop lamp uit gedrukt
#define DEUR        "deur"                                  // website knop deur gedrukt
#define KEYS        "keys"                                  // website knop deur gedrukt
#define SCAN        "card scan"                             // cardrader heeft kaart gescant

// ehternet settings
#define MAC         {0x00, 0x02, 0x03, 0x04, 0xAA, 0xBB}    // mac address van de ethernet module
#define IP          192, 168, 0, 101                        // IP addres van de ethernet module

// pin comfiguratie
#define RST_PIN_READER      46                                    // reste pin ethernet module 
#define SS_PIN_READER       48  //voor nano gebruik 5             // SS pin voor de cardreader, 10 is al bezet door ehternet shield
#define CHIP_SELECT_SD      4                                     // chipselect pin voor memory card reader
#define LAMP_RELAIS         3                                     // aansturen lamp                              --> HIGH = relais uit
#define DEUR_RELAIS         2                                     // deur relais sturen                          --> HIGH = relais uit
#define SCANNER_LED_RED     6                                     // rood ledje als foute kaart wordt gebruikt   --> LOW = led aan
#define SCANNER_LED_BLUE    5                                     // rood ledje als foute kaart wordt gebruikt   --> LOW = led aan
#define DEUR_MANUAL_BTN     0                                     // handbediening deur                          -->
#define LAMP_MANUAL_BTN     7                                     // handmatig lamp aan en uit zetten
#define REMOVE_KEY_BTN      22                                    // als je deze knop indrukt bij MASTER dan maag je kaarten verwijderen

//overig
#define MAX_KEYS_HISTORY          10                              // maximale aantal opgeslagen id's ID
#define REFERER                   "Referer: "                     // zoek deze tekst in request

// timers
#define ADD_DURATION_MILLS        5000                            // hoe mag je bezig zijn om een kaart toe te voegen (wordt na elke actie verlengd) 
#define LAMP_DURATION_MILLS       15000                           // hoe lang mag de lamp branden bij openen of sluiten van de deur 
#define CLEAR_MEMORY_HOLD_MILLS   10000                           // hoe lang moet je de lamp knop indrukken om geheugen te wissen  

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = MAC;
IPAddress ip(IP);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
MFRC522 mfrc522(SS_PIN_READER, RST_PIN_READER);       // Create MFRC522 instance

unsigned long lampActivated    = 0;     // kijk hoe lang de lamp al brandt i.v.m. automatische deur acties
unsigned long cardAddActivated = 0;     // kijk hoe lang de lamp al brandt i.v.m. automatische deur acties
unsigned long cardRemoveOnOff  = 0;     // kijk hoe lang de button remove is ingedrukt

// bijhouden wat de status van de lamp is die d.m.v. relais wordt bedient.
boolean isLampActivated        = false; // kijk of deur lamp heeft aangezet
boolean lampToggleState        = false; // manueel aan en uitzetten van de lamp

String lastAction = "";
id_type cardId;                      // laatst gelezen ID
id_type ids[MAX_KEYS_HISTORY];       // maximaal aantal id's dat wordt opgeslagen

boolean useCard             = true;  // memory card aanwezig
boolean addCardMode         = false; // er kan een card toegevoegd worden
boolean removeMode          = false; // addCardMode heeft twee toestanden: add en remove
boolean removeCardLightOn   = false; // deur lamp knippert als je in de remove status bent
// master card, controlling all other cards
id_type masterId;
KeyMemory keyStore;

void setup() {

  // load master key 
  masterId.id[0] = 0X63;
  masterId.id[1] = 0x9F;
  masterId.id[2] = 0x8C;
  masterId.id[3] = 0x00;
  masterId.id[4] = 0x00;
  masterId.id[5] = 0x00;
  masterId.id[6] = 0x00;
  masterId.size = 4;

  // Open serial communications and wait for port to open:
  Serial.begin(250000);

#if defined (__DEBUG__)
  keyStore.dumpStore();
#endif

  // relais
  pinMode(     LAMP_RELAIS      , OUTPUT );
  digitalWrite(LAMP_RELAIS      , HIGH   );
  pinMode(     DEUR_RELAIS      , OUTPUT );
  digitalWrite(DEUR_RELAIS      , HIGH   );

  // leds
  pinMode(     SCANNER_LED_RED   , OUTPUT );
  digitalWrite(SCANNER_LED_RED   , HIGH   );
  pinMode(     SCANNER_LED_BLUE  , OUTPUT );
  digitalWrite(SCANNER_LED_BLUE  , HIGH   );

  // buttons
  pinMode(DEUR_MANUAL_BTN       , INPUT_PULLUP  );
  pinMode(LAMP_MANUAL_BTN       , INPUT_PULLUP  );
  pinMode(REMOVE_KEY_BTN        , INPUT_PULLUP  );

  
  // start the Ethernet connection and the server:
  Serial.print("Initializing webserver...");
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  // initializing card reader
  Serial.print("Initializing NFC reader...");
  mfrc522.PCD_Init();   // Init MFRC522
  ShowReaderDetails();  // Show details of PCD - MFRC522 Card Reader details

  // inizializing sd card
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT_SD)) {
    Serial.println("Card failed, or not present");
    useCard = false;
  } else {
    Serial.println("card initialized.");
  }


  Serial.print("Getting current time...");
  EthernetUDP udp;
  unsigned long unixTime = ntpUnixTime(udp);
  Serial.print("Tijd: ");
  Serial.println(unixTime);
  
  Serial.println("\nsetup completed");
}


void loop() {
  checkEthernet();
  checkCard();
  checkButtons();
  checkTimers();
  setLeds();
  setRelais();
}

/**
 * Logica voor het laten branden van leds hier
 */
void setLeds() {
  
}


/**
 * logica voor het schakelen van relais hier
 */
void setRelais() {
  
}


// listen for incoming clients
void   checkEthernet() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Ehternet client availible");
    handleClient(client);
  }
}

// Look for new cards
void checkCard() {
  if ( mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("New rfid card persent");
    // Select one of the cards
    if ( mfrc522.PICC_ReadCardSerial()) {
      Serial.println("New rfid card read");
      handleCard();
    }
  }
}

// check buttons
void checkButtons() {
  // testje
  if (digitalRead(REMOVE_KEY_BTN) == LOW) {
    Serial.println("remove button pressed");
  }
  if (digitalRead(DEUR_MANUAL_BTN) == LOW) {
    Serial.println("DEUR pressed");
    deur();
  }
  if (digitalRead(LAMP_MANUAL_BTN) == LOW) {
    Serial.println("LAMP pressed");
    lampToggle();
  }
}

// check timer
void checkTimers() {
  // toevoegen van kaar moet een keer stoppen
  if (addCardMode) {
    // lamp knipperen als je in remove zit
    if (removeMode) {
      if (millis() > cardRemoveOnOff + 500) {
        cardRemoveOnOff = millis();
        removeCardLightOn = !removeCardLightOn;
        lampDeur(removeCardLightOn);
      }
    }
    if (millis() > cardAddActivated + ADD_DURATION_MILLS) {
      Serial.println("May add card: max tijd verstreken");
      mayAddCard();  
    }
  }
  
  // lamp moet weer uit als: door openen of sluiten van de deur is aangezet
  if (isLampActivated && !lampToggleState) { 
    // maximale tijd is verstreken
    if (millis() > LAMP_DURATION_MILLS + lampActivated) {
      Serial.println("lamp automatisch uit: max tijd verstreken");
      lampAanUit(false);
    }
  }
}

