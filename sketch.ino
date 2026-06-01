#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
long duration;
int distance;
bool detected = false;
bool alertShown = false;
int servoPos = 0;
int direction = 1;
void setup() {
  pinMode(9, OUTPUT);   // Trig
  pinMode(10, INPUT);   // Echo
  pinMode(6, OUTPUT);   // LED
  pinMode(7, OUTPUT);   // Buzzer
  myServo.attach(3);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Area Secure");
  lcd.setCursor(0, 1);
  lcd.print("Scanning...");
}
int getDistance() {
  digitalWrite(9, LOW);
  delayMicroseconds(2);
  digitalWrite(9, HIGH);
  delayMicroseconds(10);
  digitalWrite(9, LOW);
  duration = pulseIn(10, HIGH, 30000);
  if (duration == 0) {
    return -1;
  }
  return duration * 0.034 / 2;
}
void loop() {
  distance = getDistance();
  if (distance > 0 && distance <= 10) {
    detected = true;
  }
  else {
    detected = false;
  }
  // ALERT MODE
  if (detected) {
    if (!alertShown) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alert!!!");
      lcd.setCursor(0, 1);
      lcd.print("Intruder Found");
      alertShown = true;
    }
    digitalWrite(7, HIGH);
    delay(150);
    digitalWrite(7, LOW);
    delay(150);
    digitalWrite(6, HIGH);
    delay(150);
    digitalWrite(6, LOW);
    delay(150);
  }
  // NORMAL MODE
  else {
    if (alertShown) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Area Secure");
      lcd.setCursor(0, 1);
      lcd.print("Scanning...");
      alertShown = false;
    }
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    myServo.write(servoPos);
    servoPos += direction;
    if (servoPos >= 180) {
      servoPos = 180;
      direction = -1;
    }
    if (servoPos <= 0) {
      servoPos = 0;
      direction = 1;
    }
    delay(15);
  }
}
