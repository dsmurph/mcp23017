/**
 * @file taster.cpp
 * @class MCP23017.hpp
 * @brief Lightweight C++ API for Expander MCP23017 GPIO access.
 *
 * Simplifies GPIO input/output handling on RaspberryOS or other Linux systems with the MCP23017 expander.
 * Supports basic operations such as mode, write, and read.
 *
 * @author Kay (dsmurph)
 * @version 1.0.0
 * @date 2025-11-27
 * @license MIT
 *
 * Requires:
 *  - MCP23017 I2C 16 Bit I/O Expander Modul
 *
 * GitHub: https://github.com/dsmurph/mcp23017
 */

#include <iostream>
#include <chrono>
#include <thread>

// Include class mcp23017.
#include "MCP23017.hpp"

// Define a name
const int inputPin = 8;

// create an object
MCP23017 mcp;

int main() {

    try {
        // Sets inputPin (Pin 8) as INPUT with internal PULLUP
        mcp.pinMode(inputPin, INPUT_PULLUP);
        
         // pinValue helper
        pinValue val = HIGH;
        // The loop will continue to execute as long as the condition is true.
        while (val == HIGH) {
          
           // Read the Pin and passes state
           val = mcp.pinRead(inputPin);
           
           // Just a short break to relieve the system
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Button pressed, pin goes low and exits the loop.
        std::cout << "Button pressed!\n";
           
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}
