# nIoTo — Open-Source IoT Toolkit

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
![ESP32](https://img.shields.io/badge/ESP32-Supported-blue)
![AWS IoT](https://img.shields.io/badge/AWS%20IoT-Integrated-orange)
![MQTT](https://img.shields.io/badge/Protocol-MQTT-green)
![HTTP](https://img.shields.io/badge/Protocol-HTTP-lightgrey)

> **nIoTo** is a practical, lab-tested toolkit for building IoT systems fast — featuring ESP32 examples, dual-protocol (HTTP/MQTT) pipelines, AWS integrations, sensor data capture, FFT utilities, and ready-to-run sketches.

---

## 🚀 Quick Start

```bash
# Clone the repository
git clone https://github.com/<your-username>/nIoTo
cd nIoTo

# Arduino IDE
Sketch → Include Library → Manage Libraries…
Install: "Adafruit MPU6050", "ArduinoJson", "ESP32 filesystem uploader"

# PlatformIO (optional)
pio run -e esp32dev -t upload
