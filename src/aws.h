#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define SERVER_PORT             4080


WiFiClientSecure net;
PubSubClient mqttClient(net);


void setupAWS() {

  lcdPrintAt(0, 0, "Connecting AWS");
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);
  mqttClient.setBufferSize(DOCSIZE);

  Serial.print("Connecting to AWS IoT");
  int retries = 0;
  while (!mqttClient.connect(THINGNAME) && retries < 10) {
    Serial.print(".");
    delay(1000);
    retries++;
  }

  if (mqttClient.connected()) {
    Serial.println("\nConnected to AWS IoT");
    lcdPrintAt(0, 0, "Connected to AWS");
    mqttClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  } else {
    Serial.println("\nAWS IoT Connection Failed!");
    Serial.print("MQTT state: ");
    Serial.println(mqttClient.state());
  }
}


bool sendMQTT(StaticJsonDocument<DOCSIZE> doc){

  char buffer[DOCSIZE];
  size_t payloadSize = serializeJson(doc, buffer);

  if (!mqttClient.connected()) {
    Serial.println("MQTT not connected at publish time!");
    Serial.print("MQTT state: ");
    Serial.println(mqttClient.state());

    while (!mqttClient.connected()) {
      delay(500);
      mqttClient.connect(THINGNAME);
    }
  }
  Serial.println(buffer);
  return mqttClient.publish(AWS_IOT_PUBLISH_TOPIC, buffer);
}
