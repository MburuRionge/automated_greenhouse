#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

// Create an SSD1306 instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0X3C, 0x3C)) { // Pass the I2C address directly
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("IRRIGATION");
  display.setCursor(0, 10);
  display.print("SYSTEM IS ON");
  display.display();
  delay(3000);
  display.clearDisplay();

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
}

void loop() {
  int value = analogRead(A0);
  Serial.println(value);

  display.clearDisplay();

  if (value > 950) {
    digitalWrite(2, LOW);
    display.setCursor(0, 0);
    display.print("Water Pump is ON");
  } else {
    digitalWrite(2, HIGH);
    display.setCursor(0, 0);
    display.print("Water Pump is OFF");
  }

  if (value < 300) {
    display.setCursor(0, 10);
    display.print("Moisture : HIGH");
  } else if (value > 300 && value < 950) {
    display.setCursor(0, 10);
    display.print("Moisture : MID");
  } else if (value > 950) {
    display.setCursor(0, 10);
    display.print("Moisture : LOW");
  }

  display.display();
}