#define PinA1 33
#define PinA2 34
#define PinA3 35
#define PinA4 36


void setupAnalogSensors(){
  pinMode(PinA1, INPUT);
  pinMode(PinA2, INPUT);
  pinMode(PinA3, INPUT);
  pinMode(PinA4, INPUT);
}

void getAnalogSensorData(){
  doc["A1"] = analogRead(PinA1);
  doc["A2"] = analogRead(PinA2);
  doc["A3"] = analogRead(PinA3);
  doc["A4"] = analogRead(PinA4);
}