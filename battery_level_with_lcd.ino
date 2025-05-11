#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Analog input pin
const int voltagePin = A0;

// LED pins
const int ledRed = 2;     // Low Battery
const int ledYellow = 3;  // Medium Battery
const int ledGreen = 4;   // Full Battery

float voltage = 0.0;
float batteryVoltage = 0.0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Battery Monitor");
  delay(2000);
  lcd.clear();

  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(voltagePin);
  voltage = sensorValue * (5.0 / 1023.0);
  batteryVoltage = voltage * 5.0 * 0.9510;  // Calibrated

  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");
  lcd.print(batteryVoltage, 2);
  lcd.print("V   ");

  lcd.setCursor(0, 1);

  if (batteryVoltage <= 0.7) {
    lcd.print("No Battery Conn. ");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
  }
  else if (batteryVoltage >= 12.6) {
    lcd.print("Status: Full     ");
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
  } 
  else if (batteryVoltage >= 7.5) {
    lcd.print("Status: Medium   ");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
  } 
  else {
    lcd.print("Status: Low      ");
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
  }

  delay(1000);
}
