// Pin Definitions
const int ldrPin = A0;       // LDR connected to analog pin A0
const int relayLightPin = 8; // Relay for artificial lights connected to digital pin 8
const int relayShadePin = 9; // Relay for shading device connected to digital pin 9
const int lightThresholdLow = 500;  // Low light threshold (adjust based on testing)
const int lightThresholdHigh = 800; // High light threshold (adjust based on testing)

void setup() {
  Serial.begin(9600);            // Start serial communication for debugging
  pinMode(relayLightPin, OUTPUT); // Set light relay pin as output
  pinMode(relayShadePin, OUTPUT); // Set shade relay pin as output
  digitalWrite(relayLightPin, LOW); // Turn off artificial lights initially
  digitalWrite(relayShadePin, LOW); // Retract shades initially
}

void loop() {
  int lightIntensity = analogRead(ldrPin); // Read LDR value (0-1023)
  Serial.print("Light Intensity: ");
  Serial.println(lightIntensity);

  // Automation logic
  if (lightIntensity < lightThresholdLow) {
    // Low light detected, turn on artificial lights
    digitalWrite(relayLightPin, HIGH);
    digitalWrite(relayShadePin, LOW); // Ensure shades are retracted
    Serial.println("Low light - Turning ON artificial lights");
  } else if (lightIntensity > lightThresholdHigh) {
    // High light detected, activate shading
    digitalWrite(relayShadePin, HIGH);
    digitalWrite(relayLightPin, LOW); // Ensure artificial lights are off
    Serial.println("High light - Activating shading");
  } else {
    // Moderate light detected, turn off artificial lights and retract shades
    digitalWrite(relayLightPin, LOW);
    digitalWrite(relayShadePin, LOW);
    Serial.println("Moderate light - No action needed");
  }

  delay(1000); // Wait for a second before the next reading
}
