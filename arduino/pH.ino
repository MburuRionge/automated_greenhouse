// Pin Definitions
const int ldrPin = A0;       // LDR connected to analog pin A0
const int relayPin = 8;      // Relay or output device connected to digital pin 8
const int threshold = 600;   // Light intensity threshold (adjust based on testing)

void setup() {
  Serial.begin(9600);         // Start serial communication for debugging
  pinMode(relayPin, OUTPUT);  // Set relay pin as output
  digitalWrite(relayPin, LOW); // Turn off relay initially
}

void loop() {
  int lightIntensity = analogRead(ldrPin); // Read LDR value (0-1023)
  Serial.print("Light Intensity: ");
  Serial.println(lightIntensity);

  if (lightIntensity < threshold) {
    // Low light detected, turn on artificial lights or open curtain
    digitalWrite(relayPin, HIGH);
    Serial.println("Low light - Activating device");
  } else {
    // Sufficient light detected, turn off artificial lights or close curtain
    digitalWrite(relayPin, LOW);
    Serial.println("Sufficient light - Deactivating device");
  }

  delay(1000); // Wait for a second before the next reading
}
