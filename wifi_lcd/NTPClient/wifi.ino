#define AP_SSID "deKlokAp"
#define AP_PWD  "itsTime"

void wifiSetup() {
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  delay(1000); // geef wifimanger tijd om op te komen
  //exit after config instead of connecting
//  wifiManager.setBreakAfterConfig(false);

  //reset settings - for testing
  //wifiManager.resetSettings();

  String apSSID = "deKLokAP";
  String apPwd  = "itsTime";

  println("Connect to wifi");
  println("Access point is (pwd):");
  println(AP_SSID);
  println(AP_PWD);
  println("IP: " + wifiManager.getIp());
  println("Select a wifi for deKlok");
  
  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect(AP_SSID, AP_PWD)) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  println("connected...yeey :)");

  ip = WiFi.localIP();
  ssid = wifiManager.getSSID();
  println("IP: " + ip.toString());
  println("SSID: " + ssid);

}

void updSetup() {
  println("Starting UDP");
  udp.begin(localPort);
  println("UPD on port: " + String(udp.localPort()));
  delay(1000);
}

