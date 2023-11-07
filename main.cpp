#include <iostream>
#include <fstream>
#include <b15f/b15f.h>

// Define the states
constexpr uint8_t STATE_A = 0x01; // pin1
constexpr uint8_t STATE_B = 0x02; // pin2
constexpr uint8_t STATE_C = 0x04; // pin3
constexpr uint8_t STATE_D = 0x08; // pin4
constexpr uint8_t STATE_E = 0x80; // pin8

constexpr uint8_t OUTPUT_BITS = 4;

B15F& drv = B15F::getInstance();

void runStateA() {
    uint8_t inputs = drv.digitalRead0();
    uint8_t reversedInputs = ~inputs;
    drv.digitalWrite0(reversedInputs);
}

void runChaserPattern(uint8_t &pattern, bool &direction, bool &write0) {
    if(write0){
        drv.digitalWrite0(pattern); // Update LEDs
        drv.digitalWrite1(0x0);
    } else {
        drv.digitalWrite1(pattern);
        drv.digitalWrite0(0x0);
    }
    drv.digitalWrite0(pattern); // Update LEDs

    // Move the light
    if (direction) {
        pattern <<= 1; // Shift left
    } else {
        pattern >>= 1; // Shift right
    }

    // Change direction if we hit either end
    if (pattern == 0x80 && !write0) { // If the pattern is at the last LED
        direction = false; // Start shifting right
    } else if (pattern == 0x01 && write0) { // If the pattern is back at the first LED
        direction = true; // Start shifting left
    }

    if(pattern == 0x80 && write0) {
        write0 = false;
        pattern = 0x01;
    } else if(pattern == 0x01 && !write0){
        write0 = true;
        pattern = 0x80;
    }
}

void runStateB() {

    uint8_t pattern = 0x01; // Start with the first LED
    bool direction = true; // Start by moving to the right
    bool write0 = true;

    for (int i = 0; i < 31; ++i) { // Run the chaser pattern
        runChaserPattern(pattern, direction, write0);
        drv.delay_ms(30);
    }
}

void printTruthTableToConsole() {
    uint8_t inputs = drv.digitalRead0();
    for (int i = 0; i < 8; ++i) {
        std::cout << "Pin " << i << ": " << ((inputs & (1 << i)) ? "TRUE" : "FALSE") << std::endl;
    }
}

void printTruthTableToFile() {
    std::ofstream outFile("wahrheitswerttabelle.txt");
    uint8_t inputs = drv.digitalRead0();
    for (int i = 0; i < 8; ++i) {
        outFile << "Pin " << i << ": " << ((inputs & (1 << i)) ? "TRUE" : "FALSE") << std::endl;
    }
    outFile.close();
}

void displayOutputTable() {
    // Header for the table
    std::cout << "Output1 Output2 Output3 Output4 InputResult" << std::endl;

    for (uint8_t outputValue = 0; outputValue < (1 << OUTPUT_BITS); ++outputValue) {
        // Send output to port 0
        drv.digitalWrite0(outputValue);

        // Read input from port 1
        uint8_t inputResult = drv.digitalRead0();

        // Display outputs
        std::bitset<OUTPUT_BITS> outputs(outputValue);
        for (int i = OUTPUT_BITS - 1; i >= 0; --i) {
            std::cout << outputs[i] << "       ";
        }

        // Display input result in the 5th column
        std::cout << ((inputResult & 0x01) ? 1 : 0) << std::endl;

        // Here we just delay a bit between iterations for readability
        drv.delay_ms(100);
    }
}

void processInputsAndOutputs() {
    uint8_t dipSwitchState = drv.readDipSwitch();

    if(drv.digitalRead1() == STATE_A){
        digitalWrite0(0x0);
        digitalWrite1(0x0);
    }

    if (dipSwitchState == STATE_A) {
        runStateA();
    } else if (dipSwitchState == STATE_B) {
        runStateB();
    } else if (dipSwitchState == STATE_C) {
        printTruthTableToConsole();
    } else if (dipSwitchState == STATE_D) {
        printTruthTableToFile();
    } else if(dipSwitchState == STATE_E) {
        displayOutputTable();
    }
}

int main() {
    while (true) {
        processInputsAndOutputs();
    }

    return 0;
}
