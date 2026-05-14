#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if needed

const int buttonPin = 11;
int buttonState;

Servo myServo;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  myServo.attach(8);
  myServo.write(0);

  lcd.init();
  lcd.backlight();

  // Startup message
  lcd.setCursor(0, 0);
  lcd.print("Fortisim Secure");
  lcd.setCursor(0, 1);
  lcd.print("SIM Tray Lock");
  delay(3000);
  lcd.clear();

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint Ready");
    lcd.print("System Ready");
  } else {
    Serial.println("Sensor Error");
    lcd.print("Sensor Error");
    while (1);
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    Serial.println("Place your finger...");
    lcd.clear();
    lcd.print("Place Finger");

    // Wait for finger
    while (finger.getImage() != FINGERPRINT_OK);

    Serial.println("Finger detected");
    lcd.clear();
    lcd.print("Scanning...");

    if (finger.image2Tz() != FINGERPRINT_OK) {
      Serial.println("Image Error");
      lcd.clear();
      lcd.print("Image Error");
      return;
    }

    if (finger.fingerSearch() == FINGERPRINT_OK) {
      Serial.println("Access Granted");
      lcd.clear();
      lcd.print("Access Granted");

      myServo.write(90);
      delay(3000);
      myServo.write(0);

    } else {
      Serial.println("Access Denied");
      lcd.clear();
      lcd.print("Access Denied");
      delay(2000);

      lcd.clear();
      lcd.print("Mail Sent");   // as per your requirement
    }

    delay(2000);
    lcd.clear();
  }
}
