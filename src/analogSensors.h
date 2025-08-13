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
  int pins[4] = {PinA1, PinA2, PinA3, PinA4};
  for (int i = 0; i < 4; i++) {
    if (analogSense[i] == 1) doc["Distance A" + String(i+1)] = analogRead(pins[i]);
    if (analogSense[i] == 2) doc["Vibration A" + String(i+1)] = analogRead(pins[i]);
    if (analogSense[i] == 3) doc["Tilt A" + String(i+1)] = analogRead(pins[i]);
  }
}

