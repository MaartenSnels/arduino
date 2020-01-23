/**
 * accesspoint. Deze komt op als er nog geen contact met een wifi netwerk gemaakt kan worden
 */

void startAccessPoint() {
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssidAP, passwordAP);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  serverAP.on("/", handleAccessPoint);
  serverAP.begin();
  Serial.println("HTTP server started");
}

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleAccessPoint() {
//  serverAP.send(200, HTML,  MAIN_PAGE);

  // Check if a client has connected
  WiFiClient client = serverAP.available();
  if (!client) 
  {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  unsigned long ultimeout = millis()+250;
  while(!client.available() && (millis()<ultimeout) )
  {
    delay(1);
  }
  if(millis()>ultimeout) 
  { 
    Serial.println("client connection time-out!");
    return; 
  }
  
  // Read the first line of the request
  String sRequest = client.readStringUntil('\r');
  Serial.println("----------- REQ ------------------");
  Serial.println(sRequest);
  client.flush();

  
  // stop client, if request is empty
  if(sRequest=="")
  {
    Serial.println("empty request! - stopping client");
    client.stop();
    return;
  }

  Serial.println(sRequest);
  
  ///////////////////////////
  // format the html response
  ///////////////////////////
  String sResponse = MAIN_PAGE;
  
  
  // Send the response to the client
  Serial.println("----------- RESPONSE ------------------");
  Serial.println(sResponse);
  client.print(getHeader(sResponse.length()));
  client.print(sResponse);
  
  // and stop the client
  client.stop();
  Serial.println("Client disonnected");

}

