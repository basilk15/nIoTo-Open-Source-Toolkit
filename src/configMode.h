
#define BUTTON_PIN 13  // Connect one leg of button to GPIO 0, other to GND

// Web server on port 80
WebServer server(80);

// To store credentials
String ssid = "pop-os";
String password = "111222333";
bool credentialsReceived = false;
bool inStationMode = false;
float dummySensorValue = 0.0;


unsigned long lastTapTime = 0;
int tapCount = 0;
const unsigned long doubleTapThreshold = 400; // max delay between taps (ms)
bool lastButtonState = HIGH;



void handleSensorData() {
  dummySensorValue += 0.1;
  Serial.println("Sensor endpoint hit in AP mode: " + String(dummySensorValue));
  String json = "{\"value\": " + String(dummySensorValue, 2) + "}";
  
  char buffer[DOCSIZE];
  size_t payloadSize = serializeJson(doc, buffer);

  server.send(200, "application/json", buffer);
}


void handleRoot() {
  String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Config</title>
  <style>
    body { font-family: Arial; background: #f2f2f2; padding: 20px; }
    h2 { color: #333; }
    input, select, textarea { width: 100%; padding: 8px; margin: 6px 0; }
    button { padding: 10px 20px; background: #4CAF50; color: white; border: none; }
    details { background: #fff; padding: 10px; border: 1px solid #ccc; margin-top: 10px; }
    canvas { width: 100%; height: 250px; background: white; border: 1px solid #ccc; }
  </style>
  <script src="/chart.js"></script>
</head>
<body>
  <h2>ESP32 Configuration</h2>
  <form action="/setCreds" method="POST">
    <!-- Your form inputs remain unchanged -->
    <label>WiFi SSID:</label><input type="text" name="ssid" required><br>
    <label>WiFi Password:</label><input type="password" name="password"><br>
    <label>Delay (ms):</label><input type="number" name="delay" min="0"><br>

    <label>Option 1:</label><select name="opt1"><option value="A">A</option><option value="B">B</option></select><br>
    <label>Option 2:</label><select name="opt2"><option value="C">C</option><option value="D">D</option></select><br>
    <label>Option 3:</label><select name="opt3"><option value="E">E</option><option value="F">F</option></select><br>
    <label>Option 4:</label><select name="opt4"><option value="G">G</option><option value="H">H</option></select><br>
    <label>Option 5:</label><select name="opt5"><option value="I">I</option><option value="J">J</option></select><br>
    <label>Option 6:</label><select name="opt6"><option value="K">K</option><option value="L">L</option></select><br>
    <label>Option 7:</label><select name="opt7"><option value="M">M</option><option value="N">N</option></select><br>
    <label>Option 8:</label><select name="opt8"><option value="O">O</option><option value="P">P</option></select><br>

    <details>
      <summary><strong>Advanced Settings (AWS)</strong></summary>
      <label>Thing Name:</label><input type="text" name="thing"><br>
      <label>AWS Key:</label><textarea name="cert2" rows="4"></textarea><br>
      <label>AWS CA:</label><textarea name="cert3" rows="4"></textarea><br>
      <label>AWS Cert (Device):</label><textarea name="cert1" rows="4"></textarea><br>
    </details>

    <br><button type="submit">Submit</button>
  </form>

    <h3>Live Sensor Data:</h3>
  <div id="dataBoxes" style="display:flex;flex-wrap:wrap;gap:10px;"></div>

  <script src="/chart.js"></script>  <!-- if you still need Chart.js below -->

  <script>
    // The three keys we always skip
    // Keep histories and Chart instances in globals
    const reserved = ["deviceID","deviceName","dataID"];
    const histories = {};    // { key: [v1, v2, …] }
    const charts = {};       // { key: ChartInstance }

    // Called on every fetch
    function renderGraphs(json) {
      const container = document.getElementById("dataBoxes");
      
      Object.keys(json).forEach(key => {
        if (reserved.includes(key)) return;

        // Initialize history array
        if (!histories[key]) histories[key] = [];
        histories[key].push(json[key]);
        if (histories[key].length > 50) histories[key].shift();

        // If we haven’t created a box + chart for this key yet, do it now
        if (!charts[key]) {
          // Create box
          const box = document.createElement("div");
          box.className = "box";

          // Heading
          const h = document.createElement("h4");
          h.textContent = key;
          box.appendChild(h);

          // Canvas for chart
          const canvas = document.createElement("canvas");
          canvas.id = "chart_" + key;
          box.appendChild(canvas);

          container.appendChild(box);

          // Instantiate Chart.js on this canvas
          const ctx = canvas.getContext("2d");
          charts[key] = new Chart(ctx, {
            type: "line",
            data: {
              labels: Array(histories[key].length).fill(""), // we'll update labels later
              datasets: [{
                label: key,
                data: histories[key],
                borderColor: "blue",
                fill: false,
                tension: 0.1
              }]
            },
            options: {
              animation: false,
              responsive: true,
              scales: {
                x: { display: false },
                y: { beginAtZero: true }
              }
            }
          });
        } else {
          // Update existing chart’s data
          const chart = charts[key];
          chart.data.datasets[0].data = histories[key];
          // generate simple incremental labels or timestamps:
          chart.data.labels = histories[key].map((_, i) => "");
          chart.update();
        }
      });
    }


    function fetchSensorData() {
      fetch('/sensor')
        .then(r => r.json())
        .then(json => {
          renderGraphs(json);
        })
        .catch(console.error);
    }

    // Poll once per second
    setInterval(fetchSensorData, 1000);
    // And do an immediate first fetch:
    fetchSensorData();
  </script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", htmlPage);
}



void handleSetCreds() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    ssid = server.arg("ssid");
    password = server.arg("password");
    credentialsReceived = true;
    server.send(200, "text/html", "<h3>Credentials Saved! Now press the button to connect.</h3>");
  } else {
    server.send(400, "text/plain", "Missing SSID or Password");
  }
}


void registerRoutes() {
  server.on("/", handleRoot);
  server.on("/setCreds", HTTP_POST, handleSetCreds);  // Optional after STA
  server.on("/sensor", handleSensorData);
  
}

void setupAPMode() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.softAP("ESP32_ConfigHI", "12345678");
  //always remember this and that esp32 can function as dhcp server too.
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP Mode IP address: " + IP.toString());
  String ipStr = IP.toString();
  lcdPrintAt(0, 1, ipStr.c_str());

  server.on("/chart.js", []() {
    handleChartJS(server);
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

  // Detect falling edge (button press)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    unsigned long currentTime = millis();

    if (currentTime - lastTapTime <= doubleTapThreshold) {
      tapCount++;

      if (tapCount == 2) {
        Serial.println("Double-tap detected!");

        if (!inStationMode && credentialsReceived) {
          // Switch from AP → STA
          Serial.println("Switching to STA mode...");
          inStationMode = true;

          server.close();
          WiFi.softAPdisconnect(true);
          delay(1000);
          connectToWiFi();

        } else if (inStationMode) {
          // Switch from STA → AP
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
      tapCount = 1;  // First tap (reset count if too late)
    }

    lastTapTime = currentTime;
  }

  lastButtonState = currentButtonState;
}
