# include <iostream>
# include <b15f/b15f.h>

// Define the states (You would need to define these constants)
constexpr uint8_t STATE_A = 0x01; // Example state A
constexpr uint8_t STATE_B = 0x02; // Example state B

B15F& drv = B15F::getInstance();

void runStateA() {
    // Read all inputs as a single byte
    uint8_t inputs = drv.digitalRead0();
    // Reverse the bits
    uint8_t reversedInputs = ~inputs; // bitwise NOT operation to reverse the bits
    // Write reversed inputs to outputs
    drv.digitalWrite0(reversedInputs);
}

void runChaserPattern(uint8_t &pattern, bool &direction) {
    drv.digitalWrite0(pattern); // Update LEDs

    // Move the light
    if (direction) {
        pattern <<= 1; // Shift left
    } else {
        pattern >>= 1; // Shift right
    }

    // Change direction if we hit either end
    if (pattern == 0x80) { // If the pattern is at the last LED
        direction = false; // Start shifting right
    } else if (pattern == 0x01) { // If the pattern is back at the first LED
        direction = true; // Start shifting left
    }
}

void runStateB() {

    uint8_t pattern = 0x01; // Start with the first LED
    bool direction = true; // Start by moving to the right

    for (int i = 0; i < 16; ++i) { // Run the chaser pattern
        runChaserPattern(pattern, direction);
        drv.delay_ms(30);
    }
}

void processInputsAndOutputs() {
    // Read DIP switch state
    uint8_t dipSwitchState = drv.readDipSwitch();

    if (dipSwitchState == STATE_A) {
        runStateA();
    } else if (dipSwitchState == STATE_B) {
        runStateB();
    }
}

int main() {
    // Main loop
    while (true) {
        processInputsAndOutputs();
        // Add other functionalities if needed
    }

    return 0;
}
