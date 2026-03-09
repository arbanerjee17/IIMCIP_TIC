#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED I2C
#define OLED_SDA 4
#define OLED_SCL 5
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Analog Pins
#define TDS_PIN 0
#define PH_PIN  1

// LED Pins
#define GREEN_LED  8
#define YELLOW_LED 9
#define RED_LED    10

void setup() {
  Serial.begin(115200);

  // Initialize I2C
  Wire.begin(OLED_SDA, OLED_SCL);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED not found");
    while (true);
  }

  // LED pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {

  // Read Analog Values (0–4095)
  int tdsRaw = analogRead(TDS_PIN);
  int phRaw  = analogRead(PH_PIN);

  // Convert to meaningful values
  float tdsValue = map(tdsRaw, 0, 4095, 0, 600);  // 0–600 ppm
  float phValue  = map(phRaw, 0, 4095, 0, 140) / 10.0;  // 0–14 pH

  String status;

  // LED Logic
  if (tdsValue <= 100) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    status = "Good";
  }
  else if (tdsValue > 100 && tdsValue <= 400) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    status = "Moderate";
  }
  else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    status = "Poor";
  }

  // OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Water Quality Monitor");

  display.setCursor(0, 20);
  display.print("TDS: ");
  display.print(tdsValue);
  display.println(" ppm");

  display.setCursor(0, 35);
  display.print("pH : ");
  display.println(phValue);

  display.setCursor(0, 50);
  display.print("Status: ");
  display.println(status);

  display.display();

  delay(1000);
}
