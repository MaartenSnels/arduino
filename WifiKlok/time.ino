void getTime() {
  // haal eerst de huidige tijd op
  configTime(TIME_SHIFT * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nTime received");
}


String twoDigits(int val) {
  if (val<10) {
    return "0" + String(val);
  }
  return String(val);
}

String currentTime2String() {
  time_t t = time(nullptr);
  Serial.println(ctime(&t));
  return ctime(&t);
}




