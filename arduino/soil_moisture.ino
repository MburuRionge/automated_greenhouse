// Libraries
#include <WiFi.h>         // 
#include <Wire.h>         // 
#include <Adafruit_Sensor.h>  	


// Pin Definitions
#define SOIL_MOISTURE_PIN 34   // Analog pin for soil moisture sensor
#define RELAY_PIN 26           // Digital pin for relay control
#define SOIL_MOISTURE_THRESHOLD 500 // Adjust threshold as needed

// Variables
int soilMoistureValue = 0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Configure GPIOs
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure relay is off at start

  Serial.println("Automated Greenhouse System Initialized");
}

void loop() {
  // Read soil moisture value
  soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  
  // Print soil moisture value to Serial Monitor
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);

  // Check if soil moisture is below the threshold
  if (soilMoistureValue < SOIL_MOISTURE_THRESHOLD) {
    Serial.println("Soil is dry. Turning on water pump...");
    digitalWrite(RELAY_PIN, HIGH); // Turn on relay (water pump)
  } else {
    Serial.println("Soil moisture is sufficient. Turning off water pump...");
    digitalWrite(RELAY_PIN, LOW); // Turn off relay
  }

  // Delay to avoid rapid toggling
  delay(2000);
}
