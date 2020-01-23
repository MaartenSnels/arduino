/**
 * In deze module de routines voor het aansturen van het scherm
 */

#define LAST_FETCHED_LINE 0     // regel waar last fetch datum en tijd moet komen te staan
#define SSID_LINE         1     // regel waar SSID info moet komen te staan
#define IP_LINE           2     // regel waar IP adres moet komen te staan
#define NTP_SERVER_LINE   3     // regel waar NTP SERVER naam moet komen te staan
#define CURRENT_TIME_LINE 4     // regel waar huidige UTC tijd moet komen te staan
#define CURRENT_DATE_LINE 5     // regel waar huidige UTC datum moet komen te staan
#define ACTUAL_TIME_LINE  6     // regel waar actuele datum en tijd moet komen te staan


// voeg regel toe aan de dispaly buffer
void addLine(String in) 
{
  if (lineNr < ROWS) {
    emptyLine(lineNr);
    lines[lineNr++] = in;
    return;
  }
  
  for (int i = 0; i < ROWS - 1; i++) {
    lines[i] = lines[i + 1];
  }
  emptyLine(ROWS - 1);
  lines[ROWS - 1] = in;
}


// Laat zien wat de laatste keer is dat de tijd van de NTP server is opgehaald
void displayLastFetch() {
  Serial.print("Haal tijd op. Laatse keer gelukt: ");
  Serial.println(lasTimeFetch);

  lines[LAST_FETCHED_LINE] = getTime(lasTimeFetch, "LF: ", ":") + getDate(lasTimeFetch, " ", "-");
}

void doDisplayDateTime(time_t tijd) { 
  printAtLine(getTime(tijd, "UTC:  ", ":"), CURRENT_TIME_LINE);
  printAtLine(getDate(tijd, "Date: ", "/"), CURRENT_DATE_LINE);
  doPrint();
}

void doDisplayLocalTime(time_t utc, char delta) {
  time_t tijd = utc + delta * 3600;
  printAtLine(getTime(tijd, "NOW: ", ":") + getDate(tijd, " ", "/"), ACTUAL_TIME_LINE);
  doPrint();
}

void displaySettings() {
  emptyLines();
  doPrint();
  lines[SSID_LINE] = "SSID: " + String(ssid);
  lines[IP_LINE] = "IP: " + ip.toString();
  lines[NTP_SERVER_LINE] = ntpServerName;
  doPrint();
}


/**
 * Voeg regel toe aan de oled print buffer
 */
void println(String in)
{
  Serial.println(in);
  addLine(in);
  doPrint();
}

/**
 * Voeg regel toe aan de oled buffer zonder deze nog
 * weg te schrijven.
 */
void printAtLine(String in, char line) {
  Serial.println(in);
  emptyLine(line);
  lines[line] = in;
}

/**
 * kopieer de stringbuffer voor oled display naar het oled display
 */
void doPrint() {
  for(int j = 0; j < ROWS; j++) {
    oled.setTextXY(j,0);
    oled.putString(String(lines[j].substring(0,COLS - 1)));
    Serial.println(lines[j]);
  }
}

/**
 * Leeg de string oled display buffer:
 * Vul met spaties
 * @pariam line: line die geleegd meot worden
 */
void emptyLine(char line) {
  lines[line] = "";
  for(int i = 0; i < COLS; i++) {
    lines[line] += " ";
  }
}

/**
 * Leeg de gehele oled string buffer
 */
void emptyLines() {
  for(int i = 0; i < ROWS; i++) {
    emptyLine(i);
  }
}

/**
 * maak een string van twee cijfers van een integer. zet er evt. een string voor
 * @param digits int: het getal dat getoond moet worden
 * @param start: voorloop thekst
 * @return string met daarin het integer, evt. een voorloop 0 en de starttekst
 */
// 
String getDigits(int digits, String start){
 String res = start;
 if(digits < 10) {
   res += "0";
 }
 return res + String(digits,DEC);
}


/**
 * Test het display
 */
void testDisplay(boolean invert) {
  if (invert) {
    oled.setInverseDisplay();
  }
  
  lines[0] = "12345678901234567890";
  lines[1] = "abcdefghijklmnopqrst";
  lines[2] = "ABCDEFGHIJKLMNOPQRST";
  lines[3] = "                    ";
  lines[4] = "--------------------";
  lines[5] = "--- !Display OK! ---";
  lines[6] = "--------------------";
  lines[7] = "--------------------";
  doPrint();
  delay(1500);
  oled.setNormalDisplay();
  emptyLines();
  doPrint();
}

