//This code also includes sensor mechanism to detect position of the rotating wheel or gear
#include <Arduino.h>
#include <SPI.h>

// Define pins
const int latchPin = 27;  // RCLK (ST_CP) of 74HC595
const int clockPin = 26; // SRCLK (SH_CP) of 74HC595
const int dataPin = 14;  // SER (DS) of 74HC595

const int hallSensorPins[10] = {4, 16, 17, 18, 19, 21, 34, 35, 36, 39}; // Hall sensor input pins
// Step sequences for the stepper motors
const uint8_t stepSequence[4] = {
  //  0b00110011,
  //  0b01100110,
  //  0b11001100,//Uncomment this 4 lines for high torque and comment out other 4
  //  0b10011001
  0b00010001,
  0b00100010,
  0b01000100,
  0b10001000
};

int stepIndex[10] = {0}; // Array to hold step indices for all motors
bool motorStopped[10] = {false}; // Array to hold stopped state of all motors

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Initialize hall sensor pins
  for (int i = 0; i < 10; i++) {
    pinMode(hallSensorPins[i], INPUT_PULLUP);
  }

  Serial.begin(115200);

  Serial.print("Count down: ");
  for (int i = 7; i >= 1; i--) {
    Serial.print(i);
    Serial.print("...");
    delay(1000);
  }
  Serial.println();
}
//short
void loop() {
  // Check hall sensors
  for (int i = 0; i < 10; i++) {
    if (digitalRead(hallSensorPins[i]) == LOW) {
      motorStopped[i] = true;
    }
  }

  // Create bytes to send
  uint64_t dataToSend = 0; // 5 shift registers * 8 bits = 40 bits; using 64-bit integer for simplicity

  // Update step index and data for each motor if not stopped
  for (int i = 0; i < 10; i++) {
    if (!motorStopped[i]) {
      if (i % 2 == 0) {
        dataToSend |= ((uint64_t)(stepSequence[stepIndex[i]] & 0b11110000)) << (i * 4);
      } else {
        dataToSend |= ((uint64_t)(stepSequence[stepIndex[i]] & 0b00001111)) << ((i - 1) * 4);
      }
      stepIndex[i] = (stepIndex[i] + 1) % 4;
    }
  }

  // Send data to 74HC595
  digitalWrite(latchPin, LOW);
  for (int i = 4; i >= 0; i--) {
    shiftOut(dataPin, clockPin, MSBFIRST, (dataToSend >> (i * 8)) & 0xFF); // Send each byte
  }
  digitalWrite(latchPin, HIGH);

  // Debugging output
    Serial.print("Data sent: ");
    for (int i = 4; i >= 0; i--) {
      Serial.print(((dataToSend >> (i * 8)) & 0xFF), BIN);
      Serial.print(" ");
    }
    Serial.println();

  for (int i = 0; i < 10; i++) {
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(":");
    Serial.print("\t");
    Serial.print(motorStopped[i] ? "Stopped" : "Running");
    Serial.println();
    //    Serial.print(" | ");
  }
  Serial.println();

  // Small delay to simulate the motor step speed
  delay(1000);
}
