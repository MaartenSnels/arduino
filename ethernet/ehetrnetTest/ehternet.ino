
void handleClient(EthernetClient client) {
    String readString = "";
    Serial.println("client connected");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean actionFound = false;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read char by char HTTP request
        if (!actionFound) { // probleem bij opslaan van de string. Deze kan te lang worden en met het weinigge geheugen beperk opslag tot 1 regel
          if (c=='\n') { // hele regel geladen
            if (readString.startsWith(REFERER)) {
              Serial.print("Regel MET    REFERER: ");
              Serial.println(readString);
              actionFound = true;
            } else { // nieuwe regel opslaan
//              Serial.print("Regel zonder REFERER: ");
//              Serial.println(readString);
              readString = "";
            }
          } else {
            //store characters to string
            readString += c;
          }
        }
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
//          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html><body>");
          // output the value of each analog input pin
          client.println("Laatst gelezen kaart:");
          printId(client, &cardId);
          client.println("<br />");
          client.println("Laatste actie: ");
          client.println(lastAction);
          client.println("<br />");
          printList(client);
          printKeys(client);
          placeButtons(client);
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    Serial.println("Opgeslagen Strng:");
    Serial.println(readString);

    if (readString.indexOf(DEUR ) >0) {
      lastAction = DEUR;
      deur();   
    } else if (readString.indexOf(LAMPAAN ) >0) {
      lastAction = LAMPAAN;
      lampAanUit(true);   
    } else if (readString.indexOf(LAMPUIT ) >0) {
      lastAction = LAMPUIT;
      lampAanUit(false);   
    } else if (readString.indexOf(KEYS ) >0) {
      lastAction = KEYS;
    } else {
      geenActie();
    }

}

void printId(EthernetClient client, id_type* id) {
    client.println("<TD>");
    for (int i = 0; i < MAX_ID_SIZE && i < id->size; i++ ) {
      client.print(id->id[i],HEX);
      client.print(" ");
    }
    client.println("</TD><TD>");
    client.print(id->size);
    client.println("</TD>");            
}
void printList(EthernetClient client) {
  client.println("<H3>Laatste entries</H3>");
  client.println("<TABLE border=\"1\">");
  client.println("<TR><TH>ID</TH><TH>Key Length</TH></TR>");
  for(int i =  0; i < MAX_KEYS_HISTORY ; i++) {
    client.println("<TR>");
    // begin bij listteller - 1 (laatst opgeslagen ID) en loop terug naar 0 begin dan bij MAX_IDS en loop terug naar listteller
    printId(client,&ids[(listTeller - i - 1 + MAX_KEYS_HISTORY) % MAX_KEYS_HISTORY]);
    client.println("</TR>");
  }
  client.println("</TABLE>");
}

void placeButtons(EthernetClient client) {
  client.println("<p />");
  client.println("<TABLE border=\"1\">");
  client.println("<TR><TH>Actie</TH></TR>");
  client.print("<tr><td><a href=\"/?");
  client.print(KEYS);
  client.println("\"\">Geregistreerde sleutels</a></tr></td>");
  client.print("<tr><td><a href=\"/?");
  client.print(LAMPAAN);
  client.println("\"\">Lamp aan</a></tr></td>");
  client.print("<tr><td><a href=\"/?");
  client.print(LAMPUIT);
  client.println("\"\">Lamp uit</a></tr></td>");
  client.print("<tr><td><a href=\"/?");
  client.print(DEUR);
  client.println("\"\">Deur</a></tr></td>");
  client.println("</TABLE>");
}

void printKeys(EthernetClient client) {
  if (lastAction != KEYS) {
    return;
  }
  client.println("<CODE>");
  client.println(keyStore.getDump("<br />"));
  client.println("</CODE>");
}
