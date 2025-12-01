/**
 * @file MCP23017.hpp
 * @brief Lightweight C++ API for Expander MCP23017 GPIO access.
 *
 * Simplifies GPIO input/output handling on RaspberryOS or other Linux systems with the MCP23017 expander.
 * Supports basic operations such as Pin: mode, write, and read.
 * Supports expanded operations such as Interrupts: enable, output Mode, trigger Mode, get Flags and get Capture.
 *
 * @authors dsmurph & Lex
 * @version 1.0.0
 * @date 2025-11-27
 * @license MIT
 *
 * Requires:
 *  - MCP23017 I2C 16 Bit I/O Expander Modul
 *
 * GitHub: https://github.com/dsmurph/mcp23017
 */

#pragma once

#include <iostream>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <string>
#include <vector>

enum pin_Mode { INPUT = 1, OUTPUT = 0, INPUT_PULLUP = 3 };
enum pin_Value { HIGH = 1, LOW = 0, ERROR = 255 };
enum int_Mode { CHANGE = 2, RISING = 1, FALLING = 0 };

struct IntEvent {
    uint16_t pin;
    bool level;
};

class MCP23017 {
public:
   
    MCP23017(uint8_t address = 0x20, const std::string &i2cDev = "/dev/i2c-1") {
        fd = open(i2cDev.c_str(), O_RDWR);

        try {
           if (fd < 0) throw std::runtime_error("I2C write failed");
           if (ioctl(fd, I2C_SLAVE, address) < 0) throw std::runtime_error("I2C ioctl failed");

           writeReg(IODIRA, 0xFF);
           writeReg(IODIRB, 0xFF);          
        }
        catch (const std::runtime_error& e) {
           std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    ~MCP23017() { if (fd >= 0) close(fd); }
    


    void pinMode(uint8_t pin, pin_Mode mode) {
        if (!pinCheck(pin)) return;
        
        uint8_t regDir = (pin < 8) ? IODIRA : IODIRB;
        uint8_t regPup = (pin < 8) ? GPPUA  : GPPUB;
        uint8_t bit    = pin % 8;

        uint8_t dirVal = readReg(regDir);
        uint8_t pupVal = readReg(regPup);

        switch(mode) {
            case OUTPUT:
               dirVal &= ~(1 << bit);  // Bit 0 -> Output
               break;
            case INPUT:
            case INPUT_PULLUP:
               dirVal |=  (1 << bit);  // Bit 1 -> Input
               break;
            default:
               std::cerr << "Invalid input: pinMode(pin, INPUT/OUTPUT/INPUT_PULLUP)" << std::endl;
               return;
         }
         
         switch(mode) {
            case INPUT_PULLUP:
               pupVal |=  (1 << bit);   // Pull-Up einschalten
               break;
            case INPUT:
            case OUTPUT:
               pupVal &= ~(1 << bit);   // Pull-Up ausschalten
               break;
         }
         
         writeReg(regDir, dirVal);
         writeReg(regPup, pupVal);
    }


    void pinWrite(uint8_t pin, pin_Value value) {
        if (!pinCheck(pin)) return;
        
        uint8_t reg = (pin < 8) ? OLATA : OLATB;
        uint8_t bit = pin % 8;
        uint8_t val = readReg(reg);

        switch (value) {
           case HIGH:
               val |= (1 << bit);
               break;
           case LOW:
               val &= ~(1 << bit);
               break;
           default:
               std::cerr << "Invalid input: pinWrite(pin, HIGH/LOW)" << std::endl;
               return;
        }
        writeReg(reg, val);
    }
    

    pin_Value pinRead(uint8_t pin) {
        if (!pinCheck(pin)) return ERROR;

        uint8_t reg = (pin < 8) ? GPIOA : GPIOB;
        uint8_t bit = pin % 8;
        uint8_t val = readReg(reg);

        return ((val >> bit) & 1) ? HIGH : LOW;
    }


    void enableInt(uint8_t pin, bool enable = true) {
        if (!pinCheck(pin)) return;

        uint8_t regGPINTEN = (pin < 8) ? GPINTENA : GPINTENB;
        uint8_t bit = pin % 8;
        uint8_t regVal = readReg(regGPINTEN);
        
        if (enable) regVal |= (1 << bit);
        else regVal &= ~(1 << bit);
        
        writeReg(regGPINTEN, regVal);      
    }
    
    
    void intOutputMode(pin_Value w_INTPOL, bool w_ODR = false, bool w_MIRROR = false) {
        uint8_t val = readReg(IOCON);

        if (w_ODR)  val |=  (1 << 2);
        else val &= ~(1 << 2);

        if (!w_ODR) {
            if (w_INTPOL == HIGH) val |=  (1 << 1);
            else val &= ~(1 << 1);
        } else val &= ~(1 << 1);

        if (w_MIRROR) val |=  (1 << 6);
        else val &= ~(1 << 6);

        writeReg(IOCON, val);
    }

    
    void intTriggerMode(uint8_t pin, int_Mode mode) {
        if (!pinCheck(pin)) return;

        uint8_t regDEFVAL  = (pin < 8) ? DEFVALA  : DEFVALB;
        uint8_t regINTCON  = (pin < 8) ? INTCONA  : INTCONB;
        uint8_t bit = pin % 8;

        uint8_t intConVal = readReg(regINTCON);
        uint8_t defVal    = readReg(regDEFVAL);

        switch (mode) {
            case CHANGE:
                intConVal &= ~(1 << bit);
                break;
            case RISING:
               
                intConVal |=  (1 << bit);
                defVal    |=  (1 << bit);
                break;
            case FALLING:
                intConVal |=  (1 << bit);
                defVal    &= ~(1 << bit);
                break;
            default:
                std::cerr << "Invalid input: pinInterrupt(pin, CHANGE/RISING/FALLING)" << std::endl;
                return;
        }
        writeReg(regINTCON, intConVal);
        writeReg(regDEFVAL, defVal);
    }
    
    
    uint16_t getInterruptFlags(bool clear = false) {
        return readIntFlags(clear);
    }


    std::vector<uint8_t> getInterruptPins(bool clear = false) {
        uint16_t flags = readIntFlags(clear);
        
        std::vector<uint8_t> pins;
        
        for (uint8_t pin = 0; pin < 16; pin++) {
            if (flags & (1 << pin)) pins.push_back(pin);
        }
        
        return pins;
    }
    
    
    bool isInterruptOnPin(uint8_t pin, bool clear = false) {
        if (!pinCheck(pin)) return false;
        
        uint16_t flags = readIntFlags(clear);
        
        return flags & (1 << pin);
    }


    std::vector<IntEvent> getIntCapture(bool clear = false) {
        uint8_t a = readReg(INTFA);
        uint8_t b = readReg(INTFB);
        uint16_t flags = (uint16_t(b) << 8) | a;

        uint8_t capA = readReg(INTCAPA);
        uint8_t capB = readReg(INTCAPB);
        uint16_t captured = (uint16_t(capB) << 8) | capA;

        std::vector<IntEvent> events;
        for (uint16_t pin = 0; pin < 16; pin++) {
            if (flags & (1 << pin)) {
                bool level = captured & (1 << pin);
                events.push_back({ pin, level });
                if (clear) clearIntCapture(pin);
            }
        }

        return events;
    }


    void clearIntCapture(uint16_t pin) {
        if (pin < 8) (void)readReg(GPIOA);
        else (void)readReg(GPIOB);
    }


    void clearInterrupts() {
        (void)readReg(GPIOA);
        (void)readReg(GPIOB);
        writeReg(INTFA, 0x00);
        writeReg(INTFB, 0x00);
    }
   

    void enableSlewRateControl(bool enabled) {
        uint8_t iocon = readReg(IOCON);
    
        if (enabled) {
            iocon |= (1 << 4);
        } else {
            iocon &= ~(1 << 4);
        }
    
        writeReg(IOCON, iocon);
     }


     void setSequentialOperation(bool enabled) {
         uint8_t iocon = readReg(IOCON);

         if (enabled) {
             iocon &= ~(1 << 5);
         } else {
             iocon |= (1 << 5);
         }
    
         writeReg(IOCON, iocon);
      }



private:
    int fd;
    static constexpr uint8_t IODIRA  = 0x00;
    static constexpr uint8_t IODIRB  = 0x01;
    static constexpr uint8_t GPPUA   = 0x0C;
    static constexpr uint8_t GPPUB   = 0x0D;
    static constexpr uint8_t GPIOA   = 0x12;
    static constexpr uint8_t GPIOB   = 0x13;
    static constexpr uint8_t OLATA   = 0x14;
    static constexpr uint8_t OLATB   = 0x15;

    static constexpr uint8_t GPINTENA = 0x04;
    static constexpr uint8_t GPINTENB = 0x05;

    static constexpr uint8_t IOCON   = 0x0A;
    static constexpr uint8_t DEFVALA = 0x06;
    static constexpr uint8_t DEFVALB = 0x07;
    static constexpr uint8_t INTFA   = 0x0E;
    static constexpr uint8_t INTFB   = 0x0F;
    static constexpr uint8_t INTCAPA = 0x10;
    static constexpr uint8_t INTCAPB = 0x11;
    static constexpr uint8_t INTCONA = 0x08;
    static constexpr uint8_t INTCONB = 0x09;

    uint16_t readIntFlags(bool clear) {
        uint8_t a = readReg(INTFA);
        uint8_t b = readReg(INTFB);
        uint16_t flags = (uint16_t(b) << 8) | a;

        if (clear) {
            writeReg(INTFA, 0x00);
            writeReg(INTFB, 0x00);
        }
        return flags;
    }

    bool pinCheck(int pin) {
       if (pin < 0 || pin > 15) {
          std::cerr << "Valid Pinnums 0-15" << std::endl;
          return false;
        }
        return true;
    }

    void writeReg(uint8_t reg, uint8_t value) {
        uint8_t data[2] = {reg, value};
        try {
           if (write(fd, data, 2) != 2) throw std::runtime_error("I2C write failed");
        }
        catch (const std::runtime_error& e) {
           std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    uint8_t readReg(uint8_t reg) {
        uint8_t value = 0;
        try {
           if (write(fd, &reg, 1) != 1) throw std::runtime_error("I2C write failed");
           if (read(fd, &value, 1) != 1) throw std::runtime_error("I2C write failed"); 
        }
        catch (const std::runtime_error& e) {
           std::cerr << "Error: " << e.what() << std::endl;
        }
      return value;   
    }
};