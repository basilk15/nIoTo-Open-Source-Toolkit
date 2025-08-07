uint64_t  startEpochOffset = 0;


void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void setupTime(){
  // Use native NTP (better than NTPClient)
  lcdPrintAt(0, 0, "Setting Time");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  //Wait for time to sync
  while (time(nullptr) < 24 * 3600) {
    delay(500);
    Serial.print(".");
  }

  // Once synced, calculate offset
  startEpochOffset = static_cast<int64_t>(time(nullptr)) * 1000LL - (int64_t)millis();
  Serial.printf("\nEpoch offset (ms): %lld\n", startEpochOffset);
  lcdPrintAt(0, 0, "Time Synced");
}

long getWifiStrength(){
  return WiFi.RSSI();
}

