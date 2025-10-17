🌱 Solar-Powered CO₂ Monitor (Arduino Uno R4 WiFi + K30 Sensor)

This project is a solar-powered CO₂ monitoring system built using an Arduino Uno R4 WiFi and a K30 10,000 ppm CO₂ sensor.
It continuously measures CO₂ concentration in the air and publishes live readings to a local HTML webpage over Wi-Fi — enabling real-time wireless monitoring. Additionally, readings with timestamps are sent to Firebase Realtime Database for cloud-based logging, remote access, and long-term data analysis.

⚙️ Features

📡 Live CO₂ Readings via UART communication between the Arduino Uno R4 WiFi and K30 sensor

☀️ 5–9 W Solar Panel power source for off-grid operation

🌐 Built-in Web Interface served directly from the Arduino (accessible via its IP address)

📊 Dynamic Graph + PPM Display showing live CO₂ data

🔁 Fully Wireless Setup — view data from any browser on the same Wi-Fi network

☁️ Cloud Logging with Firebase — stores readings with timestamps for remote monitoring and historical analysis

⚡ Scalable Design — can be expanded to multiple sensors (~$200 USD per device)

🧰 Hardware Components
Component	Description	Approx. Cost
Arduino Uno R4 WiFi	Microcontroller with built-in Wi-Fi	$75
K30 CO₂ Sensor (10,000 ppm)	High-accuracy CO₂ measurement module	$120
Solar Panel (5–9 W)	Powers the system via sunlight	$10–15
Jumper Wires / Connectors	For UART communication and power	$5

Total estimated cost: ≈ $200 per device

🔌 Wiring Diagram (UART)
K30 Sensor Pin	Arduino Uno R4 WiFi Pin
TX (Sensor Out)	RX1 (Serial1)
RX (Sensor In)	TX1 (Serial1)
GND	GND
V+ (5 V)	5 V

Note: The K30 communicates over UART at 9600 baud. Ensure both devices share a common ground.

💻 Software Overview

Written in Arduino C++

Uses Serial1 for UART communication with the K30

Uses WiFi and WiFiClientSecure libraries for network connection and Firebase HTTPS requests

Uses Chart.js on the Arduino-hosted webpage for live graph visualization

Sends CO₂ readings with timestamps to Firebase Realtime Database for cloud storage and long-term analysis

Example Data Output (Serial Monitor)
CO2 Reading: 412 ppm
Sending data to Firebase...
Firebase response: {"name":"-NAbC12345abc"} 

🌐 Webpage Visualization

Access the Arduino’s IP address from a device on the same Wi-Fi network.

The webpage displays a live graph of the CO₂ readings, updating every 2 seconds.

Chart.js is used for smooth, dynamic plotting.

Local graph shows recent readings (~last 20), while Firebase stores all historical data.

☁️ Firebase Logging

Each reading is stored as a JSON object:

{
  "timestamp": 12345678,
  "co2": 450
}


Firebase automatically generates unique keys for each reading.

Cloud logging enables remote monitoring, historical analysis, and potential integration with dashboards or analytics tools.

🔧 How It Works

Arduino reads CO₂ ppm from the K30 sensor via UART.

Data is displayed on a live, local webpage for immediate visualization.

Simultaneously, readings with timestamps are sent to Firebase Realtime Database via HTTPS POST requests.

This combination of local visualization and cloud logging allows real-time monitoring, historical tracking, and remote access — improving upon the limitations of a local-only webpage.

📈 Improvements Over Local Webpage Only

Historical data is stored in the cloud for analysis.

Remote access is possible from anywhere with internet.

Provides both instant visualization and persistent logging for a complete CO₂ monitoring solution.

⚡ License

MIT License — free to use and modify.
