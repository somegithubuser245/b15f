#include <iostream>
#include "B15F.h"  // Include the B15F library

int main() {
    B15F &drv = B15F::getInstance();

    // Read the state of the DIP switch
    uint8_t dipSwitchState = drv.readDipSwitch();

    if (dipSwitchState == 1) {
        // DIP switch is set to 1, toggle LEDs in a row
        for (int i = 1; i <= 16; i++) {
            drv.digitalWrite1(1 << (i - 1)); // Set one bit at a time
            // You might need to add a delay here to control the LED toggling speed
        }
        for (int i = 16; i >= 1; i--) {
            drv.digitalWrite1(1 << (i - 1)); // Turn off the same LEDs in reverse order
            // Add a delay here as well
        }
    } else {
        // DIP switch is set to 0, read, invert, and send digital input states to output
        uint8_t digitalInputState = drv.digitalRead1();

        // Invert the input state
        uint8_t invertedState = ~digitalInputState;

        // Send the inverted state to the digital output
        drv.digitalWrite1(invertedState);
    }

    return 0;
}
