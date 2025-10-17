#include <WiFi.h>
#include "SoftwareSerial.h"

// Replace these with your WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Set your server port
WiFiServer server(80);

// Buffer for CO2 readings
float co2Value = 0;

// Function to read CO2 from K30 sensor
float readCO2() {
  // K30 UART command for "Read CO2"
  byte cmd[9] = {0xFE, 0x44, 0x00, 0x08, 0x02, 0x9F, 0x25, 0x00, 0x00};
  Serial1.write(cmd, 9); // Send command to K30
  delay(10);

  if (Serial1.available() >= 9) {
    byte response[9];
    Serial1.readBytes(response, 9);
    int high = response[2];
    int low = response[3];
    int ppm = high << 8 | low;
    return ppm;
  }
  return -1; // error
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // K30 default baud
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          // Check if it's end of HTTP request
          if (currentLine.length() == 0) {
            // Send HTML page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<title>K30 CO2 Monitor</title>");
            client.println("<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>CO2 Monitor</h1>");
            client.println("<canvas id='co2Chart' width='400' height='200'></canvas>");
            client.println("<script>");
            client.println("var ctx = document.getElementById('co2Chart').getContext('2d');");
            client.println("var co2Data = { labels: [], datasets: [{ label: 'CO2 ppm', data: [], borderColor: 'rgba(75, 192, 192, 1)', fill: false }] };");
            client.println("var co2Chart = new Chart(ctx, { type: 'line', data: co2Data, options: { responsive: true, scales: { x: { title: { display: true, text: 'Time' } }, y: { title: { display: true, text: 'CO2 ppm' }, min: 0, max: 12000 } } } });");
            client.println("function fetchData() {");
            client.println("fetch('/data').then(response => response.text()).then(value => {");
            client.println("var now = new Date().toLocaleTimeString();");
            client.println("co2Data.labels.push(now);");
            client.println("co2Data.datasets[0].data.push(parseInt(value));");
            client.println("if(co2Data.labels.length > 20){ co2Data.labels.shift(); co2Data.datasets[0].data.shift(); }");
            client.println("co2Chart.update();");
            client.println("}); }");
            client.println("setInterval(fetchData, 2000);"); // update every 2 seconds
            client.println("</script>");
            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Handle /data request
        if (currentLine.endsWith("GET /data")) {
          co2Value = readCO2();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println("Connection: close");
          client.println();
          client.println(String(co2Value));
        }
      }
    }
    delay(1);
    client.stop();
  }
}