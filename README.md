<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>nIoTo — Open‑Source IoT Toolkit</title>
  <meta name="description" content="nIoTo is an open‑source IoT toolkit: ESP32 examples, HTTP/MQTT pipelines, AWS integrations, data capture tools, and lab‑ready utilities." />
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800;900&family=JetBrains+Mono:wght@400;600&display=swap" rel="stylesheet">
  <style>
    :root{
      --bg: #0b0f1a; /* page bg */
      --panel: #0f1424; /* card bg */
      --muted: #9aa4bf; /* secondary text */
      --text: #e6e9f4; /* primary text */
      --pri: 200 100% 58%; /* accent (HSL: hue 200) */
      --glass: 0.6; /* glass blur strength */
      --radius: 18px;
      --shadow: 0 20px 60px rgba(0,0,0,.35), inset 0 1px 0 rgba(255,255,255,.04);
    }
    *{box-sizing:border-box}
    html,body{height:100%}
    body{
      margin:0; font-family:Inter, system-ui, -apple-system, Segoe UI, Roboto, "Helvetica Neue", Arial, "Noto Sans", "Apple Color Emoji", "Segoe UI Emoji";
      background: radial-gradient(1200px 600px at 20% -10%, hsl(260 80% 20%/.55), transparent 60%),
                  radial-gradient(1000px 500px at 120% 0%, hsl(200 90% 25%/.40), transparent 55%),
                  linear-gradient(180deg, #0a0e19 0%, #070b14 100%);
      color:var(--text);
    }
    .container{width:min(1100px, 92%); margin-inline:auto}

    /* ===== Header ===== */
    header{position:sticky; top:0; z-index:50; backdrop-filter:saturate(140%) blur(10px); background:rgba(8,12,22,.55); border-bottom:1px solid rgba(255,255,255,.06)}
    .nav{display:flex; align-items:center; justify-content:space-between; padding:14px 0}
    .brand{display:flex; align-items:center; gap:12px; text-decoration:none; color:inherit}
    .logo{width:40px; height:40px; border-radius:12px; background:conic-gradient(from 190deg, hsl(200 100% 60%), hsl(260 90% 60%), hsl(310 90% 60%), hsl(200 100% 60%)); box-shadow:0 8px 24px hsl(200 100% 50%/.25)}
    .title{font-weight:800; letter-spacing:.3px}
    .gh{display:inline-flex; align-items:center; gap:10px; padding:10px 14px; border-radius:12px; border:1px solid rgba(255,255,255,.08); background:rgba(255,255,255,.03); text-decoration:none; color:var(--text)}
    .gh:hover{background:rgba(255,255,255,.06)}

    /* ===== Hero ===== */
    .hero{padding:64px 0 32px}
    .hero-inner{display:grid; grid-template-columns:1.2fr .8fr; gap:28px}
    @media (max-width: 900px){ .hero-inner{grid-template-columns:1fr} }
    .hero h1{font-size:clamp(32px, 5vw, 56px); line-height:1.1; margin:0 0 14px}
    .hero p{font-size:clamp(16px, 1.4vw, 18px); color:var(--muted); margin:0 0 22px}
    .cta{display:flex; gap:12px; flex-wrap:wrap}
    .btn{--ring: hsla(200, 100%, 60%, .45); display:inline-flex; align-items:center; gap:10px; padding:12px 16px; font-weight:600; text-decoration:none; color:#06121c; background:hsl(var(--pri)); border-radius:14px; box-shadow:0 14px 40px var(--ring), inset 0 1px 0 rgba(255,255,255,.4)}
    .btn.secondary{color:var(--text); background:transparent; border:1px solid rgba(255,255,255,.12); box-shadow:var(--shadow)}
    .tagrow{display:flex; gap:10px; flex-wrap:wrap; margin-top:16px}
    .tag{font-family:"JetBrains Mono", ui-monospace, SFMono-Regular, Menlo, Consolas, "Liberation Mono", monospace; font-size:12px; letter-spacing:.2px; padding:8px 10px; border-radius:12px; background:rgba(255,255,255,.04); color:var(--muted); border:1px solid rgba(255,255,255,.06)}

    .terminal{position:relative; background:linear-gradient(180deg, #0c1426, #0a1020); border:1px solid rgba(255,255,255,.08); border-radius:18px; padding:18px; box-shadow:var(--shadow)}
    .term-head{display:flex; gap:8px; margin-bottom:10px}
    .dot{width:10px;height:10px;border-radius:50%; background:#f56565}
    .dot:nth-child(2){background:#f6ad55}
    .dot:nth-child(3){background:#48bb78}
    pre{margin:0; overflow:auto}
    code{font-family:"JetBrains Mono", ui-monospace, SFMono-Regular, Menlo, Consolas, "Liberation Mono", monospace; font-size:13.5px; line-height:1.5}

    /* ===== Grid cards ===== */
    .section{padding:28px 0}
    .section h2{font-size:clamp(22px, 3vw, 28px); margin:0 0 12px}
    .muted{color:var(--muted)}
    .grid{display:grid; grid-template-columns:repeat(3,1fr); gap:16px}
    @media (max-width: 950px){ .grid{grid-template-columns:repeat(2,1fr)} }
    @media (max-width: 650px){ .grid{grid-template-columns:1fr} }
    .card{background:linear-gradient(180deg, #0f162c, #0d1326); border:1px solid rgba(255,255,255,.08); border-radius:var(--radius); padding:16px; box-shadow:var(--shadow)}
    .card h3{margin:0 0 6px; font-size:18px}
    .card p{margin:0; color:var(--muted)}

    /* ===== Code block chips ===== */
    .chip{display:inline-flex; align-items:center; gap:8px; padding:8px 10px; border-radius:12px; background:rgba(255,255,255,.05); border:1px solid rgba(255,255,255,.08); font-family:"JetBrains Mono"; font-size:12px; margin:6px 6px 0 0}

    /* ===== Collapsible details ===== */
    details{background:rgba(255,255,255,.03); border:1px solid rgba(255,255,255,.08); border-radius:14px; padding:12px 14px}
    details+details{margin-top:10px}
    summary{cursor:pointer; font-weight:600}
    summary::-webkit-details-marker{display:none}

    /* ===== Footer ===== */
    footer{border-top:1px solid rgba(255,255,255,.08); margin-top:36px; padding:22px 0 60px; color:var(--muted)}
    .footgrid{display:grid; grid-template-columns:1fr auto; align-items:center}
    @media (max-width: 700px){.footgrid{grid-template-columns:1fr; gap:10px}}
    a{color:hsl(var(--pri)); text-decoration:none}
    a:hover{text-decoration:underline}
  </style>
</head>
<body>
  <header>
    <div class="container nav">
      <a class="brand" href="#">
        <div class="logo" aria-hidden="true"></div>
        <div class="title">nIoTo</div>
      </a>
      <a class="gh" href="#getting-started" title="Jump to Quick Start">
        <svg width="18" height="18" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" aria-hidden="true"><path d="M9 18l6-6-6-6" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>
        Get Started
      </a>
    </div>
  </header>

  <main class="container">
    <!-- ===== Hero ===== -->
    <section class="hero">
      <div class="hero-inner">
        <div>
          <h1>nIoTo — Open‑Source IoT Toolkit</h1>
          <p>A practical, lab‑tested toolkit for building IoT systems fast: ESP32 examples, dual‑protocol (HTTP/MQTT) pipelines, AWS integrations, data capture/FFT utilities, and ready‑to‑run sketches for sensors like INMP441, MPU6050, SW420, and more.</p>
          <div class="cta">
            <a class="btn" href="#getting-started">🚀 Quick Start</a>
            <a class="btn secondary" href="#features">Browse Features</a>
          </div>
          <div class="tagrow" aria-label="highlights">
            <span class="tag">ESP32</span>
            <span class="tag">HTTP ↔ MQTT</span>
            <span class="tag">AWS IoT + API Gateway + Lambda + DynamoDB</span>
            <span class="tag">I2S + FFT</span>
            <span class="tag">TCA9548A + I²C</span>
          </div>
        </div>
        <div class="terminal" role="region" aria-label="terminal">
          <div class="term-head"><span class="dot"></span><span class="dot"></span><span class="dot"></span></div>
          <pre><code>$ git clone https://github.com/&lt;you&gt;/nIoTo
$ cd nIoTo
$ code .

# Arduino IDE
Sketch → Include Library → Manage Libraries…
Install: "Adafruit MPU6050", "ArduinoJson", "ESP32 filesystem uploader"

# PlatformIO (optional)
$ pio run -e esp32dev -t upload

# Python helpers (optional)
$ python3 -m venv .venv && source .venv/bin/activate
$ pip install -r tools/requirements.txt
</code></pre>
        </div>
      </div>
    </section>

    <!-- ===== Highlights ===== -->
    <section id="features" class="section">
      <h2>Why nIoTo?</h2>
      <p class="muted">Everything you need to go from sensor to cloud to insights — with batteries‑included examples.</p>
      <div class="grid" role="list">
        <article class="card" role="listitem">
          <h3>ESP32 Sensor Recipes</h3>
          <p>Plug‑and‑play sketches for INMP441 (I2S audio), MPU6050 (IMU), SW420 (vibration), and IR sensors. Includes dual‑mic capture and real‑time FFT examples.</p>
        </article>
        <article class="card" role="listitem">
          <h3>HTTP & MQTT Pipelines</h3>
          <p>Two complete data paths with request/response and pub/sub tests, payload sizing utilities, and Wireshark‑friendly tracing.</p>
        </article>
        <article class="card" role="listitem">
          <h3>AWS‑Ready</h3>
          <p>Templates for IoT Core, API Gateway, Lambda, and DynamoDB with least‑privilege policies and sample deployment scripts.</p>
        </article>
        <article class="card" role="listitem">
          <h3>Data + Debug Tools</h3>
          <p>Serial log formatters, SD card helpers, CSV writers, and Python notebooks for quick plots and latency analysis.</p>
        </article>
        <article class="card" role="listitem">
          <h3>Modular & Extensible</h3>
          <p>Clean headers for I²C mux (TCA9548A), sensor auto‑detect stubs, and a simple config layer to toggle features.</p>
        </article>
        <article class="card" role="listitem">
          <h3>Well‑Documented</h3>
          <p>Inline comments, top‑level README guides, and troubleshooting notes for common ESP32 hiccups.</p>
        </article>
      </div>
    </section>

    <!-- ===== Getting Started ===== -->
    <section id="getting-started" class="section">
      <h2>Getting Started</h2>
      <p class="muted">Choose your path and run your first sample in minutes.</p>
      <div class="card" style="margin-top:10px">
        <h3>1) Clone & Open</h3>
        <div class="chip">git clone https://github.com/&lt;you&gt;/nIoTo</div>
        <div class="chip">cd nIoTo</div>
        <div class="chip">Open with Arduino IDE or VS Code</div>
      </div>
      <div class="grid" style="margin-top:12px">
        <div class="card">
          <h3>Arduino IDE</h3>
          <ol>
            <li>Install ESP32 board support (Boards Manager → ESP32 by Espressif).</li>
            <li>Install required libraries (Library Manager): Adafruit_MPU6050, Adafruit_Sensor, ArduinoJson, SD(esp32).</li>
            <li>Tools → Board: ESP32 Dev Module (or your board), select correct Port.</li>
            <li>Open <code>examples/INMP441_DualFFT/INMP441_DualFFT.ino</code> → Upload.</li>
          </ol>
        </div>
        <div class="card">
          <h3>PlatformIO (optional)</h3>
          <ol>
            <li>Install the VS Code PlatformIO extension.</li>
            <li><code>pio run -e esp32dev -t upload</code> to build & flash.</li>
            <li>Adjust <code>platformio.ini</code> environments as needed.</li>
          </ol>
        </div>
        <div class="card">
          <h3>AWS Setup (optional)</h3>
          <ol>
            <li>Provision a thing in IoT Core; attach certs & policy.</li>
            <li>Deploy sample API Gateway → Lambda → DynamoDB stack.</li>
            <li>Update endpoint/keys in <code>config/aws_secrets_example.h</code>.</li>
          </ol>
        </div>
      </div>
    </section>

    <!-- ===== Folder Structure ===== -->
    <section class="section">
      <h2>Folder Structure</h2>
      <pre><code>nIoTo/
├─ examples/
│  ├─ INMP441_DualFFT/
│  ├─ MPU6050_TCA9548A/
│  ├─ HTTP_Client_ESP32/
│  └─ MQTT_Client_ESP32/
├─ libs/
│  ├─ sensors/
│  ├─ mux/
│  └─ net/
├─ tools/
│  ├─ scripts/
│  └─ notebooks/
├─ data/            # CSV, captures
├─ docs/
└─ LICENSE
</code></pre>
    </section>

    <!-- ===== Usage Snippets ===== -->
    <section class="section">
      <h2>Usage Snippets</h2>
      <details>
        <summary>Publish with MQTT (ESP32)</summary>
        <pre><code>#include &lt;WiFi.h&gt;
#include &lt;PubSubClient.h&gt;

WiFiClientSecure net;  
PubSubClient mqtt(net);

void loop(){
  StaticJsonDocument&lt;128&gt; doc;  
  doc["t"] = millis();
  doc["ax"] = ax; // from MPU6050
  char buf[128]; serializeJson(doc, buf);
  mqtt.publish("nioto/telemetry", buf);
}
</code></pre>
      </details>
      <details>
        <summary>POST with HTTP</summary>
        <pre><code>HTTPClient http; 
http.begin(apiUrl);
http.addHeader("Content-Type", "application/json");
int code = http.POST(payload);
</code></pre>
      </details>
      <details>
        <summary>Enable I²C channel (TCA9548A)</summary>
        <pre><code>void tca_select(uint8_t ch){
  Wire.beginTransmission(0x70);
  Wire.write(1 &lt;&lt; ch);
  Wire.endTransmission();
}
</code></pre>
      </details>
    </section>

    <!-- ===== Troubleshooting ===== -->
    <section class="section">
      <h2>Troubleshooting</h2>
      <ul>
        <li><strong>SPIFFS not declared:</strong> Include <code>#include &lt;SPIFFS.h&gt;</code> and call <code>SPIFFS.begin(true)</code>.</li>
        <li><strong>Multiple SD libs found:</strong> Prefer the ESP32 core SD library shipped with your board package.</li>
        <li><strong>Two I2S mics at once?</strong> Use L/R channels on a single bus or separate I2S0/I2S1 with care (clock domain).</li>
        <li><strong>MPU on only one channel?</strong> Ensure TCA channel latch and pull‑ups; scan each channel before init.</li>
      </ul>
    </section>

    <!-- ===== Contributing & License ===== -->
    <section class="section">
      <h2>Contributing</h2>
      <p class="muted">PRs welcome! If you add a sensor recipe, include a short README and a wiring diagram image in the example folder.</p>
      <h2 style="margin-top:18px">License</h2>
      <p>MIT © nIoTo contributors</p>
    </section>

    <footer>
      <div class="footgrid">
        <div>Made with ❤️ for embedded tinkering • <span class="muted">nIoTo</span></div>
        <div><a href="#">Back to top ↑</a></div>
      </div>
    </footer>
  </main>
</body>
</html>
