#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
#define LED_GREEN 2
#define LED_RED 4
#define BUZZER 7
#define SERVO 3

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
byte password[] = {0x83, 0x67, 0x05, 0xE5};  // Predefined UID
Servo myServo;  // Create Servo instance

//*****************************************************************************************//
void setup() {
  myServo.attach(SERVO);
  myServo.write(0);              // Move servo to initial position (0 degrees)
  pinMode(LED_GREEN, OUTPUT);   // Set LED_MATCH pin as output
  pinMode(LED_RED, OUTPUT); // Set LED_NO_MATCH pin as output
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);  // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  Serial.println("Scan an RFID card");
}

//*****************************************************************************************//
void loop() {
// Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show UID on serial monitor
 if (isEqualUID(mfrc522.uid.uidByte, mfrc522.uid.size, password, sizeof(password))) {
    digitalWrite(LED_GREEN, 1);
    successTone();   
    myServo.write(90); 
    delay(2000);
    digitalWrite(LED_GREEN, 0); 
    myServo.write(0); 

  } else {
    digitalWrite(LED_RED, 1);
    errorTone();                    // Play success tone
    delay(2000);
    digitalWrite(LED_RED, 0);
  }

  delay(2000);
}

bool isEqualUID(byte *uid1, byte uid1Size, byte *uid2, byte uid2Size) {
  if (uid1Size != uid2Size) {
    return false;
  }
  for (byte i = 0; i < uid1Size; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}

void successTone() {
  tone(BUZZER, 1000, 200);  // Play 1kHz tone for 200 ms
  delay(200);                   // Wait for the tone to finish
}

void errorTone() {
  tone(BUZZER, 500, 500);   // Play 500Hz tone for 500 ms
  delay(500);                   // Wait for the tone to finish
}