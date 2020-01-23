
bool startServer()
{
  ulReconncount++;
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssidServer);
  
  WiFi.begin(ssidServer, passwordServer);
  
  while (WiFi.status() != WL_CONNECTED && attempt <= MAX_ATTEMPTS) {
    delay(500);
    attempt++;
    Serial.print(".");
  }
  Serial.println("");

  if (attempt >= MAX_ATTEMPTS) {
    Serial.println("WiFi NOT connected");
    return false;
  }
  Serial.println("WiFi connected");
  
  // Start the server
  serverServer.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  return true;
}


void handleServer() {
  // Check if a client has connected
  WiFiClient client = serverServer.available();
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

  Request req = getCommand(sRequest);
  
  ///////////////////////////
  // format the html response
  ///////////////////////////
  String sResponse;
  
  ////////////////////////////
  // 404 for non-matching path
  ////////////////////////////
  if(req.path != "/")
  {
    sResponse=PAGE_NOT_FOUND;
  }
  ///////////////////////
  // format the html page
  ///////////////////////
  else
  {

    ulReqcount++;
    sResponse  = HEAD;
    sResponse += "Current time: " + currentTime2String() + ".<BR>";
    sResponse += BUTTONS1;
    sResponse += BUTTONS2;
    
    //////////////////////
    // react on parameters
    //////////////////////
    if (req.command.length()>0)
    {
      // write received command to html page
      sResponse += "Command: " + req.command + "<BR>";
      handleCommand(req.command);
    }
    
    sResponse += "<FONT SIZE=-2>";
    sResponse += "<BR>requestcount = "; 
    sResponse += ulReqcount;
    sResponse += " - wifi reconnections = "; 
    sResponse += ulReconncount;
    sResponse += "<BR>";
    sResponse += "Stefan Thesen 04/2015<BR>";
    sResponse += CLOSE_BODY;
  }
  
  // Send the response to the client
  Serial.println("----------- RESPONSE ------------------");
  Serial.println(sResponse);
  client.print(getHeader(sResponse.length()));
  client.print(sResponse);
  
  // and stop the client
  client.stop();
  Serial.println("Client disonnected");

}

String getHeader(long length) {
  String sHeader  = HEADER1;
         sHeader += length;
         sHeader += HEADER2;

  return sHeader;
}

Request getCommand(String commandString) {
  // get path; end of path is either space or ?
  // Syntax is e.g. GET /?pin=MOTOR1STOP HTTP/1.1
  String sPath="",
         sParam="", 
         sCmd="";
  
  int iStart,
      iEndSpace,
      iEndQuest;
      
  iStart = commandString.indexOf(GET);
  if (iStart >= 0)
  {
    iStart += String(GET).length();
    iEndSpace = commandString.indexOf(" ",iStart);
    iEndQuest = commandString.indexOf("?",iStart);
    
    // are there parameters?
    if(iEndSpace > 0)
    {
      if(iEndQuest > 0)
      {
        // there are parameters
        sPath  = commandString.substring(iStart,iEndQuest);
        sParam = commandString.substring(iEndQuest,iEndSpace);
      }
      else
      {
        // NO parameters
        sPath  = commandString.substring(iStart,iEndSpace);
      }
    }
  }
  
  ///////////////////////////////////////////////////////////////////////////////
  // output parameters to serial, you may connect e.g. an Arduino and react on it
  ///////////////////////////////////////////////////////////////////////////////
  if(sParam.length() > 0)
  {
    int iEqu = sParam.indexOf("=");
    if(iEqu >= 0)
    {
      sCmd = sParam.substring(iEqu + 1, sParam.length());
      Serial.print("Command = ");
      Serial.println(sCmd);
    }
  }

  Request req;
  req.command = sCmd;
  req.path = sPath;
  return req;

}


void handleCommand(String command) {
  // switch GPIO
  if(command.indexOf(FUNCTION1ON) >= 0)
  {
    digitalWrite(LED1, LOW);
  }
  else if(command.indexOf(FUNCTION1OFF) >= 0)
  {
    digitalWrite(LED1, HIGH);
  }
  else if(command.indexOf(FUNCTION2ON) >= 0)
  {
    digitalWrite(LED2, HIGH);
  }
  else if(command.indexOf(FUNCTION2OFF) >= 0)
  {
    digitalWrite(LED2, LOW);
  }
}
