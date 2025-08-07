#ifndef MUX_H
#define MUX_H

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MAX30105.h>
#include <ArduinoJson.h>

#define TCA_ADDR 0x70
#define MPU_ADDR 0x68
#define BME_ADDR 0x77
#define MAX_ADDR 0x57






Adafruit_MPU6050 mpu;
Adafruit_BME280 bme;        // I2C
MAX30105 max3010x;          // I2C

bool mpuAvailable[8] = {false};
bool bmeAvailable[8] = {false};
bool maxAvailable[8] = {false};

void tca_select(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void setupMUX() {
  Wire.begin();
  delay(500);

  for (uint8_t ch = 0; ch < 8; ch++) {
    tca_select(ch);
    delay(50);

    // === Check MPU6050 ===
    if (mpu.begin(MPU_ADDR)) {
      Serial.print("MPU6050 initialized on channel ");
      Serial.println(ch);
      mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
      mpu.setGyroRange(MPU6050_RANGE_500_DEG);
      mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
      mpuAvailable[ch] = true;
    }

    // === Check BME280 ===
    if (bme.begin(BME_ADDR)) {
      Serial.print("BME280 initialized on channel ");
      Serial.println(ch);
      bmeAvailable[ch] = true;
    }

    // === Check MAX30102 ===
    if (max3010x.begin(Wire, I2C_SPEED_STANDARD, MAX_ADDR)) {
      Serial.print("MAX30102 initialized on channel ");
      Serial.println(ch);
      max3010x.setup(); // basic default setup
      max3010x.setPulseAmplitudeRed(0x0A);    // Visible for debugging
      max3010x.setPulseAmplitudeIR(0x0A);
      maxAvailable[ch] = true;
    }

    delay(100);
  }
}

void getMUXSensors() {
  for (uint8_t ch = 0; ch < 8; ch++) {
    tca_select(ch);
    delay(10);

    // === MPU6050 ===
    if (mpuAvailable[ch]) {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      String accelPrefix = "accel_ch" + String(ch) + "_";
      String gyroPrefix = "gyro_ch" + String(ch) + "_";

      doc[accelPrefix + "x"] = a.acceleration.x;
      doc[accelPrefix + "y"] = a.acceleration.y;
      doc[accelPrefix + "z"] = a.acceleration.z;

      doc[gyroPrefix + "x"] = g.gyro.x;
      doc[gyroPrefix + "y"] = g.gyro.y;
      doc[gyroPrefix + "z"] = g.gyro.z;
    }

    // === BME280 ===
    if (bmeAvailable[ch]) {
      String prefix = "bme_ch" + String(ch) + "_";
      doc[prefix + "temp"] = bme.readTemperature();
      doc[prefix + "hum"] = bme.readHumidity();
      doc[prefix + "press"] = bme.readPressure() / 100.0F;
    }

    // === MAX30102 ===
    if (maxAvailable[ch]) {
      String prefix = "max_ch" + String(ch) + "_";
      doc[prefix + "ir"] = max3010x.getIR();
      doc[prefix + "red"] = max3010x.getRed();
    }
  }
}

#endif
