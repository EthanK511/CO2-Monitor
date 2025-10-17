#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Firebase settings
const char* firebaseHost = "YOUR_FIREBASE_PROJECT_ID.firebaseio.com"; // e.g., myproject.firebaseio.com
const char* firebaseAuth = "YOUR_DATABASE_SECRET_OR_FIREBASE_AUTH_KEY";

// K30 sensor UART
float readCO2() {
  byte cmd[9] = {0xFE, 0x44, 0x00, 0x08, 0x02, 0x9F, 0x25, 0x00, 0x00};
  Serial1.write(cmd, 9);
  delay(10);

  if (Serial1.available() >= 9) {
    byte response[9];
    Serial1.readBytes(response, 9);
    int high = response[2];
    int low = response[3];
    int ppm = high << 8 | low;
    return ppm;
  }
  return -1;
}

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // K30 baud
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  
  // Allow self-signed certificates
  client.setInsecure();
}

void loop() {
  float co2 = readCO2();
  if (co2 != -1) {
    // Build JSON payload
    String payload;
    payload += "{";
    payload += "\"timestamp\": " + String(millis());
    payload += ", \"co2\": " + String(co2);
    payload += "}";

    // Firebase REST API URL (push new child)
    String url = "/co2_data.json?auth=" + String(firebaseAuth);

    Serial.println("Sending data to Firebase...");
    if (client.connect(firebaseHost, 443)) {
      client.println("POST " + url + " HTTP/1.1");
      client.println("Host: " + String(firebaseHost));
      client.println("User-Agent: Arduino/1.0");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(payload.length());
      client.println();
      client.println(payload);

      // Print response
      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") break; // End of headers
      }
      String response = client.readString();
      Serial.println(response);
      client.stop();
    } else {
      Serial.println("Connection failed!");
    }
  }
  delay(5000); // send every 5 seconds
}