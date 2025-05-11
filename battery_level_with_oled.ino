#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Input pin
const int voltagePin = A0;

// LED pins
const int ledRed = 2;
const int ledYellow = 3;
const int ledGreen = 4;

float voltage = 0.0;
float batteryVoltage = 0.0;
int batteryPercent = 0;

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // halt if OLED fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Battery Monitor");
  display.display();
  delay(2000);
  display.clearDisplay();

  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(voltagePin);
  voltage = sensorValue * (5.0 / 1023.0);
  batteryVoltage = voltage * 5.0 * 0.9510;  // Calibrated
  batteryVoltage = constrain(batteryVoltage, 0.0, 14.0);

  // % Mapping
  batteryPercent = map(batteryVoltage * 100, 0, 1400, 0, 100);
  batteryPercent = constrain(batteryPercent, 0, 100);

  // Battery bar width
  int barWidth = map(batteryPercent, 0, 100, 0, 20);

  // OLED Display
  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("Voltage: ");
  display.print(batteryVoltage, 2);
  display.println(" V");

  display.setCursor(0, 10);
  display.print("Battery: ");
  display.print(batteryPercent);
  display.println("%");

  // Battery icon outer frame
  display.drawRect(90, 0, 25, 12, WHITE);     // main body
  display.fillRect(115, 3, 2, 6, WHITE);      // battery tip

  // Battery fill bar
  display.fillRect(91, 1, barWidth, 10, WHITE);

  display.setCursor(0, 25);
  
  // LEDs and Status
  if (batteryVoltage <= 0.7) {
    display.println("No Battery Connected");
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
  } else if (batteryPercent >= 80) {
    display.println("Status: Full");
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
  } else if (batteryPercent >= 30) {
    display.println("Status: Medium");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
  } else {
    display.println("Status: Low");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
  }

  display.display();
  delay(1000);
}
