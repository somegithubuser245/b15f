# include <iostream>
# include <b15f/b15f.h>

// Assuming that B15F is initialized and available as b15f

// Define the bit patterns for the chaser light
constexpr uint16_t CHASER_FORWARD = 0x0001; // Start with the first light
constexpr uint16_t CHASER_BACKWARD = 0x8000; // Start with the last light

void runStateA() {
    uint16_t inputs = 0;
    for (int i = 0; i < 16; ++i) {
        if (b15f.digitalRead0() & (1 << i)) {  // Read each digital input
            inputs |= (1 << i); // Set the corresponding bit in inputs
        }
    }
    inputs = ~inputs; // Reverse the bits
    for (int i = 0; i < 16; ++i) {
        if (inputs & (1 << i)) {
            b15f.digitalWrite1(1 << i); // Set the digital output
        } else {
            b15f.digitalWrite0(1 << i); // Clear the digital output
        }
    }
}

void runStateB() {
    // Assuming some delay function is available
    extern void delay(unsigned int milliseconds);

    uint16_t pattern = CHASER_FORWARD;
    bool direction = true; // true for forward, false for backward

    for (int i = 0; i < 32; ++i) { // Run the chaser pattern 32 times
        for (int j = 0; j < 16; ++j) {
            if (pattern & (1 << j)) {
                b15f.digitalWrite1(1 << j); // Turn on the LED
            } else {
                b15f.digitalWrite0(1 << j); // Turn off the LED
            }
        }
        // Update the pattern
        if (direction) {
            pattern <<= 1; // Move the light forward
            if (pattern == CHASER_BACKWARD) direction = false;
        } else {
            pattern >>= 1; // Move the light backward
            if (pattern == CHASER_FORWARD) direction = true;
        }
        delay(100); // Delay to control the speed of the chaser light
    }
}

void processInputsAndOutputs() {
    // Read DIP switch state
    uint8_t dipSwitchState = b15f.readDipSwitch();

    if (dipSwitchState == STATE_A) {
        runStateA();
    } else if (dipSwitchState == STATE_B) {
        runStateB();
    }
}

int main() {
    B15F b15f; // Initialize your B15F object
    while (true) {
        processInputsAndOutputs();
        // Add a delay or other functionality if needed
    }
}
