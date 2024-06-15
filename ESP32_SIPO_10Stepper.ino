// Define the pins
const int dataPin = 25;    // DS pin of 74HC595
const int clockPin = 32;   // SH_CP pin of 74HC595
const int latchPin = 33;   // ST_CP pin of 74HC595

// Step sequence for a typical 4-wire stepper motor
const uint8_t stepSequence[4] = {
  0b1001,  // Step 1
  0b0011,  // Step 2
  0b0110,  // Step 3
  0b1100   // Step 4
};

// Initialize the pins
void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  digitalWrite(latchPin, LOW);  // Start with latch low

  Serial.begin(115200);  // Initialize serial communication for debugging
}

// Function to send 40 bits of data to the shift registers (10 motors * 4 bits each)
void shiftOut40Bits(uint32_t data) {
  digitalWrite(latchPin, LOW);  // Prepare for data load

  for (int i = 39; i >= 0; i--) {
    digitalWrite(clockPin, LOW);  // Prepare for data shift
    digitalWrite(dataPin, (data & (1 << i)) ? HIGH : LOW);  // Set data bit
    digitalWrite(clockPin, HIGH);  // Shift data bit
  }

  digitalWrite(latchPin, HIGH);  // Latch data to output
}

// Function to set steps for multiple stepper motors
void setMotorSteps(uint8_t motorSteps[10]) {
  uint32_t data = 0;

  for (int i = 0; i < 10; i++) {
    data |= (uint32_t)motorSteps[i] << (i * 4);
  }

  shiftOut40Bits(data);

  // Debug print
//    Serial.print("Motor steps: ");
//    for (int i = 0; i < 10; i++) {
//      Serial.print(motorSteps[i], BIN);
//      Serial.print(" ");
//    }
//    Serial.println();
}

void moveMotor(int motorIndex, int steps) {
  for (int step = 0; step < steps; step++) {
    for (int seq = 0; seq < 4; seq++) {
      uint8_t motorSteps[10] = {0};
      motorSteps[motorIndex] = stepSequence[seq];
      setMotorSteps(motorSteps);
//      delay(2);  // Adjust delay for motor speed
    }
  }
}

void loop() {
  for (int motor = 0; motor < 10; motor++) {
    moveMotor(motor, 150);  // Move each motor 150 steps
    delay(1000);            // Delay between motors
  }
  coolDown();
}

void coolDown() {
  Serial.print("Cool down: ");
  for (int i = 5; i >= 1; i--) {
    Serial.print(i);
    Serial.print("...");
    delay(1000);
  }
  Serial.println();
}
