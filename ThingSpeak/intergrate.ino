#include <LiquidCrystal_I2C.h>

#include <ThingSpeak.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
const char* ssid = "alxap";
const char* password = "#DoHardThings01";

// ThingSpeak credentials
unsigned long channelID = 2753781;
const char* apiKey = "NLCW0MZKYVZ7NP1Z";

WiFiClient client;

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define I2C_SCL 2
#define I2C_SDA 42

// pH sensor calibration parameters
float calibration_value = 14.69; 
float slope = -4.90;
int pHsensor = A0; 

// DHT Sensor
#define DHTPIN 5      // GPIO pin connected to the DHT sensor
#define DHTTYPE DHT11 // Change to DHT22 if using DHT22
DHT dht(DHTPIN, DHTTYPE);

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Soil moisture sensor
int soilSensorPin = 39; // Soil moisture sensor analog pin
int pumpPin = 2;        // Water pump control pin

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Monitoring");
  display.display();

  // Initialize DHT sensor
  dht.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Irrigation System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);

  // Pump setup
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH); // Turn pump off initially
}

void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Handle pH sensor reading
  int buffer_arr[10];
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(pHsensor);
    delay(30);
  }
  std::sort(buffer_arr, buffer_arr + 10);
  unsigned long avgval = 0;
  for (int i = 2; i < 8; i++) {
    avgval += buffer_arr[i];
  }
  float average = (float)avgval / 6.0;
  float volt = average * 5.0 / 1024.0;
  float ph_act = slope * volt + calibration_value;

  // Read soil moisture
  int soilValue = analogRead(soilSensorPin);

  // Control pump based on soil moisture
  if (soilValue > 950) {
    digitalWrite(pumpPin, LOW); // Turn pump on
    lcd.setCursor(0, 0);
    lcd.print("Water Pump: ON ");
  } else {
    digitalWrite(pumpPin, HIGH); // Turn pump off
    lcd.setCursor(0, 0);
    lcd.print("Water Pump: OFF");
  }

  lcd.setCursor(0, 1);
  if (soilValue < 300) {
    lcd.print("Moisture: HIGH ");
  } else if (soilValue < 950) {
    lcd.print("Moisture: MID  ");
  } else {
    lcd.print("Moisture: LOW  ");
  }

  // Display values on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" C");

  display.setCursor(0, 10);
  display.print("Humidity: ");
  display.print(humidity);
  display.print(" %");

  display.setCursor(0, 20);
  display.print("pH: ");
  display.print(ph_act, 2);

  display.setCursor(0, 30);
  display.print("Soil: ");
  display.print(soilValue);

  display.display();

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, ph_act);
  ThingSpeak.setField(4, soilValue);
  int responseCode = ThingSpeak.writeFields(channelID, apiKey);

  if (responseCode == 200) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Error sending data: " + String(responseCode));
  }

  delay(15000); // Wait 15 seconds before next update
}