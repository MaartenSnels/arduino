

void timeserverSetup() {
  setSyncProvider(getNtpTime);
  setSyncInterval(UPDATE_INTERVAL); // probeer elke 600 seconde, ofwel 10 minuten de tijd opnieuw op te halen
}

String getTime(time_t tijd, String first, String sep) {
 tmElements_t tm;
 breakTime(tijd, tm);
 return getDigits(tm.Hour  , first ) +
        getDigits(tm.Minute, sep) + 
        getDigits(tm.Second, sep);
}

String getDate(time_t tijd, String first, String sep) {
 tmElements_t tm;
 breakTime(tijd, tm);
 return getDigits(tm.Day   , first) +
        getDigits(tm.Month , sep) +
        getDigits(tm.Year + 1970  , sep); 
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}


long getNtpTime()
{
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 
  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  delay(1000);
  int cb = udp.parsePacket();
  if (cb) {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    long theTime = secsSince1900 - seventyYears;

    // dispaly last fetched time
    lasTimeFetch = theTime;
    displayLastFetch();

    //ready
    return theTime;
   }
   return 0; // return 0 if unable to get the time
}




