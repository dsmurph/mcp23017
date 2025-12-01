 /**
 * @file  interrup.cpp
 * @class MCP23017.hpp
 * @brief Lightweight C++ API for MCP23017 GPIO expander access.
 *
 * Provides simplified GPIO handling on Raspberry Pi OS or other Linux-based systems
 * using the MCP23017 I²C IO expander. 
 *
 * Features:
 *  - Pin configuration: mode, read, write
 *  - Interrupt support: enable, trigger mode, output mode
 *  - Interrupt status: read flags, captured pin state
 *
 * Designed for clarity, beginner-friendly usage, and minimal overhead.
 *
 * @authors dsmurph
 * @version 1.0.0
 * @date 2025-11-27
 * @license MIT
 *
 * Hardware:
 *  - MCP23017 I²C 16-Bit IO Expander Module
 *
 * GitHub: https://github.com/dsmurph/mcp23017
 */

#include <iostream>
#include <chrono>
#include <thread>

// Include class mcp23017.
#include "MCP23017.hpp"

// Define a name.
const int interruptPin_1 = 0;

// Create an object
MCP23017 mcp;

int main() {
    try {

         // ITA/ITB switch to high on interrupt.
         mcp.intOutputMode(HIGH);

         // Configure interruptPin_1 with an internal pull-up resistor set to high.
         mcp.pinMode(interruptPin_1, INPUT_PULLUP);

         // With active pull-up, set falling or change edges.
         mcp.intTriggerMode(interruptPin_1, FALLING);

         // Interrupt pin enable
         mcp.enableInt(interruptPin_1, true);
         
         // Boolean helper
         bool val = false;
         while (!val) {
           
             // Wait for interrupt
             val = mcp.isInterruptOnPin(interruptPin_1);
           
         }
         std::cout << "A falling flank was detected!\n";


         // Interrupt pin disabled
         mcp.enableInt(interruptPin_1, false);
        
         // --- Output pinnum list ---
         auto pins = mcp.getInterruptPins();
         for (auto& p : pins) {
             std::cout << "Pin: " << int(p) << "\n";
         }

         // --- Bitmask decimal ---
         uint16_t flags = mcp.getInterruptFlags();
         std::cout << "Flags (decimal): " << flags << "\n";

         // --- Bitmask hex ---
         flags = mcp.getInterruptFlags();
         std::cout << "Flags (hex): 0x" << std::hex << std::uppercase << flags << "\n";

         // --- Bitmask binary & clear ---
         flags = mcp.getInterruptFlags(true);
         std::cout << "Flags (binary): ";
         for (int i = 15; i >= 0; i--) {
             std::cout << ((flags >> i) & 1);
         }
         std::cout << "\n";

         // --- INTCAP: captured pin states ---
         auto events = mcp.getIntCapture();
         for (auto& e : events) {
             std::cout << "Pin " << int(e.pin) << " State: " << (e.level ? "HIGH" : "LOW") << "\n";
         }

         // Clean-up
         mcp.clearIntCapture(interruptPin_1);
        
         // Optional: reset all interrupt settings
         mcp.clearInterrupts();
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
} 