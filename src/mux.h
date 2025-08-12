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
Adafruit_BME280 bme;       
MAX30105 max3010x;          

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

  for (uint8_t ch = 2; ch < 6; ch++) {
    if (1) {
      tca_select(ch);
      delay(50);

      if (mpu.begin(MPU_ADDR)) {
        Serial.print("MPU6050 initialized on channel ");
        Serial.println(ch);
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
        mpuAvailable[ch] = true;
      }

      if (bme.begin(BME_ADDR)) {
        Serial.print("BME280 initialized on channel ");
        Serial.println(ch);
        bmeAvailable[ch] = true;
      }

      
      if (max3010x.begin(Wire, I2C_SPEED_STANDARD, MAX_ADDR)) {
        Serial.print("MAX30102 initialized on channel ");
        Serial.println(ch);
        max3010x.setup(); 
        max3010x.setPulseAmplitudeRed(0x0A);    
        max3010x.setPulseAmplitudeIR(0x0A);
        maxAvailable[ch] = true;
      }

      delay(100);
    }
  }
}

void getMUXSensors() {
  for (uint8_t ch = 2; ch < 6; ch++) {
    if (i2cSense[ch-2] != 0) {
      tca_select(ch);
      delay(10);

      
      if (i2cSense[ch-2] == 102 && mpuAvailable[ch]) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        String accelPrefix = "accel_ch" + String(ch) + "_";
        String gyroPrefix = "gyro_ch" + String(ch) + "_";

        doc[accelPrefix + "x B" + String(ch-1)] = a.acceleration.x;
        doc[accelPrefix + "y B" + String(ch-1)] = a.acceleration.y;
        doc[accelPrefix + "z B" + String(ch-1)] = a.acceleration.z;

        doc[gyroPrefix + "x B" + String(ch-1)] = g.gyro.x;
        doc[gyroPrefix + "y B" + String(ch-1)] = g.gyro.y;
        doc[gyroPrefix + "z B" + String(ch-1)] = g.gyro.z;
      }

      if (i2cSense[ch-2] == 103 && bmeAvailable[ch]) {
        String prefix = "bme_ch" + String(ch) + "_";
        doc[prefix + "temp B" + String(ch-1)] = bme.readTemperature();
        doc[prefix + "hum B" + String(ch-1)] = bme.readHumidity();
        doc[prefix + "press B" + String(ch-1)] = bme.readPressure() / 100.0F;
      }

      if (i2cSense[ch-2] == 101 && maxAvailable[ch]) {
        String prefix = "max_ch" + String(ch) + "_";
        doc[prefix + "ir B" + String(ch-1)] = max3010x.getIR();
        doc[prefix + "red B" + String(ch-1)] = max3010x.getRed();
      }
    }
  }
}

#endif
