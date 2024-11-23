#include <ThingSpeak.h>
#include <WiFi.h>

// WiFi credentials
const char* ssid = "ALX The Piano 01";
const char* password = "Thepiano01";

// ThingSpeak credentials
unsigned long channelID = 2753781;
const char* apiKey = "NLCW0MZKYVZ7NP1Z";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  ThingSpeak.begin(client);
}

void loop() {
  float temperature = 25.0; // Replace with your sensor reading
  float humidity = 60.0;    // Replace with your sensor reading

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int responseCode = ThingSpeak.writeFields(channelID, apiKey);

  if (responseCode == 200) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Error sending data: " + String(responseCode));
  }
  delay(15000); // Wait 15 seconds
}
