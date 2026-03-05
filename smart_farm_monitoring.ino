#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 4
#define OLED_SCL 5
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pins
#define GAS_PIN 0
#define LDR_PIN 2
#define PIR_PIN 3

#define GREEN_LED 8
#define RED_LED 9

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  pinMode(GAS_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {

  // Read Gas (0–4095)
  int gasValue = analogRead(GAS_PIN);

  // Read LDR
  int lightValue = analogRead(LDR_PIN);

  // Read PIR
  int motion = digitalRead(PIR_PIN);

  String status = "SAFE";

  // Condition logic
  if (gasValue > 2500 || motion == HIGH) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    status = "ALERT!";
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
  // -------- SERIAL MONITOR OUTPUT --------
  Serial.println("----- Smart Monitor Data -----");
  Serial.print("Gas Value: ");
  Serial.println(gasValue);


  Serial.print("Light Value: ");
  Serial.println(lightValue);


  Serial.print("Motion: ");
  Serial.println(motion == HIGH ? "YES" : "NO");


  Serial.print("Status: ");
  Serial.println(status);


  Serial.println("--------------------------------");
  Serial.println();
  // ---------------------------------------

  // OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.println("Smart Monitor");

  display.setCursor(0,15);
  display.print("Gas: ");
  display.println(gasValue);

  display.setCursor(0,30);
  display.print("Light: ");
  display.println(lightValue);

  display.setCursor(0,45);
  display.print("Motion: ");
  display.println(motion == HIGH ? "YES" : "NO");

  display.setCursor(0,55);
  display.print("Status: ");
  display.println(status);

  display.display();

  delay(1000);
}