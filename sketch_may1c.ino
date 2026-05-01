#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 5
#define RST_PIN 22
#define SERVO_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);
Servo gateServo;

byte authorizedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Gate closed
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  if (checkAccess(rfid.uid.uidByte)) {
    Serial.println("Access Granted");
    gateServo.write(90); // Open gate
    delay(3000);
    gateServo.write(0); // Close gate
  } else {
    Serial.println("Access Denied");
  }

  rfid.PICC_HaltA();
}

bool checkAccess(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] != authorizedUID[i]) return false;
  }
  return true;
}