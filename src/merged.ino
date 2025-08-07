#define DOCSIZE 2048
#define BAUD_RATE 115200
#define DELAY_MS 2000

#include <ArduinoJson.h>

StaticJsonDocument<DOCSIZE> doc;
int data_count = 1;
bool setup_flag = 1;


#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <time.h>  // for configTime() and time()
#include <PubSubClient.h>

#include "mux.h"
#include "lcdModule.h"
#include "analogSensors.h"
#include "secret.h"
#include "wifiTime.h"
#include "aws.h"
#include "sd_module.h"
#include "configMode.h"
//#include "imnpData.h"





void sendAndSaveData(){

  char Data[200] = "";  // Output buffer
  char temp[16];           // Temporary buffer for each value
  bool first = true;

  for (JsonPair kv : doc.as<JsonObject>()) {
    if (!first) {
      strcat(Data, ",");  // Add comma between values
    }

    // Convert value to float and then format to 2 decimal places
    float val = kv.value().as<float>();
    snprintf(temp, sizeof(temp), "%.2f", val);
    strcat(Data, temp);

    first = false;
  }
  char space[2] = {'\n','\0'};
  strcat(Data, space);
  
  saveData(SD, "/data_4.csv",Data);
  getLastData(SD, "/data_4.csv");

  

  
  bool resultMQTT = sendMQTT(doc);
  Serial.println("=== MQTT Result ===");
  if (resultMQTT){
    Serial.println("MQTT Message Sent Successfully");
    lcdPrintAt(0, 0, "Data Sent Saved");
  } else {
    Serial.println("Error Sending MQTT Message");
  }
}



void setup() {
  Serial.begin(BAUD_RATE); // Use high baud rate for faster streaming
  delay(50);

  initLCD();

  lcdPrintAt(0, 0, "Hello, World!");
  lcdPrintAt(0, 1, "ESP32 Ready");


  setupSD();
  //setupI2S();
  setupAnalogSensors();
  setupMUX();
  
  lcdPrintAt(0, 0, "In Config Mode");
  
  setupAPMode();
    
}



void loop() {

  unsigned long ctime = millis();
  while (millis() - ctime < DELAY_MS) handleServerAndButton();

  if (inStationMode) lcdPrintAt(0, 0, "In Active Mode");
  
  if (setup_flag && inStationMode){
    setupTime();
    setupAWS();

    lcdPrintAt(0, 0, "In Active Mode");
    setup_flag = 0;
  }

  handleServerAndButton();

  

  doc["deviceID"] = 24;
  doc["deviceName"] = "ESP32";
  doc["dataID"] = data_count;

  getAnalogSensorData();
  getMUXSensors();
  //getImnpData();


  if (inStationMode){
    sendAndSaveData();
  }

  data_count++;

  Serial.print("Sample Number: ");
  Serial.println(data_count-1);
  Serial.println("");
}