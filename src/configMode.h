#define BUTTON_PIN 13 

int analogSense[4] = {0,0,0,0};
int i2cSense[4] = {0,0,0,0};

WebServer server(80);
#include "sd_module.h"

String ssid = "pop-os";
String password = "111222333";
bool credentialsReceived = false;
bool inStationMode = false;
float dummySensorValue = 0.0;


unsigned long lastTapTime = 0;
int tapCount = 0;
const unsigned long doubleTapThreshold = 400;
bool lastButtonState = HIGH;


void handleSensorData() {
  Serial.println("Sensor endpoint hit");
  
  char buffer[DOCSIZE];
  size_t payloadSize = serializeJson(doc, buffer);
  
  Serial.println("Sending JSON: " + String(buffer));
  
  server.send(200, "application/json", buffer);
}

void handleSetCreds() {
  if (server.hasArg("ssid")) ssid = server.arg("ssid");
  if (server.hasArg("password")) password = server.arg("password");
  if (server.hasArg("delay")) DELAY_MS = server.arg("delay").toInt();
  for (int i = 0; i < 8; i++) {
    String argName = "opt" + String(i+1);
    if (server.hasArg(argName)) {
      int raw = server.arg(argName).toInt();
      if (i < 4) analogSense[i] = raw; 
      else i2cSense[i-4] = raw;              
    }
  }

  credentialsReceived = true;
  server.send(200, "text/html",
    "<h3>Saved! Double tap the button to connect or keep using AP by going back to IP. "
    "Selected sensors will show in Live Data.</h3>");
}



void registerRoutes() {
  server.on("/", handleRoot);  
  server.on("/setCreds", HTTP_POST, handleSetCreds);
  server.on("/sensor", handleSensorData);
}



void setupAPMode() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.softAP("ESP32_ConfigHI", "12345678");
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP Mode IP address: " + IP.toString());
  String ipStr = IP.toString();

  lcdPrintAt(0, 1, ipStr.c_str());
  server.on("/chart.js", []() {
    handleChartJS();
  });

  registerRoutes();
  server.begin();
  Serial.println("Web server started in AP mode");
}

void startWebServerSTA() {
  registerRoutes();
  server.begin();
  Serial.println("Web server started in STA mode");
}



void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  lcdPrintAt(0, 0, "Connecting Wifi");
  WiFi.begin(ssid.c_str(), password.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected! IP address: " + WiFi.localIP().toString());
    startWebServerSTA();
    String ipStr = WiFi.localIP().toString();
    lcdPrintAt(0, 1, ipStr.c_str());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}


void handleServerAndButton() {
  server.handleClient();

  bool currentButtonState = digitalRead(BUTTON_PIN);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    unsigned long currentTime = millis();

    if (currentTime - lastTapTime <= doubleTapThreshold) {
      tapCount++;

      if (tapCount == 2) {
        Serial.println("Double-tap detected!");

        if (!inStationMode && credentialsReceived) {
          Serial.println("Switching to STA mode...");
          inStationMode = true;

          server.close();
          WiFi.softAPdisconnect(true);
          delay(1000);
          connectToWiFi();

        } else if (inStationMode) {
          Serial.println("Switching back to AP mode...");
          inStationMode = false;
          setup_flag = 1;

          server.close();
          WiFi.disconnect(true);
          delay(1000);
          setupAPMode();
        }

        tapCount = 0;
        lastTapTime = 0;
      }
    } else {
      tapCount = 1; 
    }

    lastTapTime = currentTime;
  }

  lastButtonState = currentButtonState;
}
