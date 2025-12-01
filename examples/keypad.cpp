/**
 * @file keypad.cpp
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

// Create an object.
MCP23017 mcp;

// Define columns and rows pins.
const uint8_t colPins[3] = {8, 9, 10};
const uint8_t rowPins[4] = {11, 12, 13, 14};

// Define keypad keys
char keyMap[4][3] = {
    {'1','2', '3'},
    {'4','5', '6'},
    {'7','8', '9'},
    {'*','0', '#'}
};

int main() {
  
  // Set colums as Output and level HIGH 
  for (uint8_t i = 0; i < 3; ++i) {
       mcp.pinMode(colPins[i], OUTPUT);
       mcp.pinWrite(colPins[i], HIGH);
    }

    // Set rows as Input and pullup internal resistors for a stable high/low
    for (uint8_t i = 0; i < 6; ++i) {
       mcp.pinMode(rowPins[i], INPUT_PULLUP);
    } 
    
    while (true) {
       
       // The loop sets all columns to low one after the other.
       // c = 0-2
       for (uint8_t c = 0; c < 3; c++) {
          mcp.pinWrite(colPins[c], LOW);
          // This loop reads the rows one after the other to see if they are actively low.
          // r = 0-3
          for (uint8_t r = 0; r < 4; r++) {
             if (mcp.pinRead(rowPins[r]) == LOW) {
                
                // The active low rows are passed as characters.
                // For example, c = 0, r = 3 returns the character
                // from our keypad array, key 7.
                char key = keyMap[r][c];
                
                // Output key value
                std::cout << "Pressed Key= " << key << "\n";
                
                // Most pushbuttons vibrate when pressed,
                // causing them to trigger multiple times.
                // Simple debouncing.
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
             }
          }
        // Column back on high
        mcp.pinWrite(colPins[c], HIGH);
        }
       
        // Just a short break to relieve the system
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
 }

