# Arduino Stepper Motor Control with 74HC595 Shift Registers

This Arduino sketch demonstrates controlling multiple stepper motors using 74HC595 shift registers for step sequencing. Each motor follows a predefined step sequence, and the control signals are synchronized using shift registers.

## Hardware Components Used
- 74HC595 Shift Register
- Stepper Motors (Typically 4-wire)
- Arduino Board (e.g., Arduino Uno)

## Pin Configuration
- `dataPin` (DS pin of 74HC595): 25
- `clockPin` (SH_CP pin of 74HC595): 32
- `latchPin` (ST_CP pin of 74HC595): 33

## Step Sequence
The step sequence used for a typical 4-wire stepper motor:
- Step 1: `0b1001`
- Step 2: `0b0011`
- Step 3: `0b0110`
- Step 4: `0b1100`

## Functions
### `shiftOut40Bits(uint32_t data)`
- Sends 40 bits of data to the shift registers (10 motors * 4 bits each).

### `setMotorSteps(uint8_t motorSteps[10])`
- Sets the steps for multiple stepper motors based on the provided array of steps.

### `moveMotor(int motorIndex, int steps)`
- Moves a specified motor by the given number of steps using the defined step sequence.

### `coolDown()`
- Prints a countdown message for cool down between motor movements.

## Setup
- Initialize serial communication at 115200 baud rate for debugging.
- Setup pins for data, clock, and latch as outputs.
- Start with the latch pin low.

## Usage
- The `loop()` function iterates over each motor, moving it 150 steps and adding a delay of 1 second between each motor's movement.
- Adjust the delay in `moveMotor()` for controlling the speed of motor movement.

## Debugging
- Uncomment the debug print statements in `setMotorSteps()` and `moveMotor()` for detailed information on motor steps.

## Note
Ensure proper wiring and component compatibility. Adjust timing and delays as per motor specifications for optimal performance.

