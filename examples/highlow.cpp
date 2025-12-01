/**
 * @file highlow.cpp
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

// Include class mcp23017.
#include "MCP23017.hpp"

// Define a name
const int highlowPin = 1;

// Create an object
MCP23017 mcp;

int main() {
    try {
        // Set as Output 
        mcp.pinMode(highlowPin, OUTPUT);
        // Set high
        mcp.pinWrite(highlowPin, HIGH);
        // Set low 
        mcp.pinWrite(highlowPin, LOW);
           
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}
