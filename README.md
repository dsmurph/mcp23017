# 🛸 MCP23017 I2C Expander

The MCP23017 expander is an excellent GPIO extension for the Raspberry Pi.
It features 16 additional GPIOs that can be configured as output and input pins.
This mcp23017 class simplifies working with the I2C interface, configuring the MCP, and switching the pins for your project.

The functions are kept simple and restore all the functionalities of the expander.

```cpp
pinMode()
pinWrite()
pinRead()
pinPullUp()
(pinInterrupt()) Not yet available
'''

Perfect for students, hobbyists and projects where GPIO control is simply needed – without having to delve deeper into the expander's registers.

---

## ✨ Features

✔️ Simple GPIO input/output  
✔️ One-line pin configuration  
✔️ Interrupt support (RISING, FALLING, BOTH)


---

## 🧩 Provided Functions

| Function | Description |
|----------|-------------|
| `pinMode(pin, Output/Input/Pullup)` | Configures pin direction |
| `pinWrite(pin, HIGH/LOW)` | Sets pin output state |
| `pinRead(pin)` | Reads digital input |
| `pinInterrupt(pin)` |  |
| `attachInterrupt(pin, edge, callback)` | Executes function on edge event |

---

📁 examples/
```
 ├── blink.cpp      // Make individual LEDs blink
 ├── taster.cpp     // Query buttons
 ├── highlow.cpp    // Set Pin high/low
 └── keypad.cpp     // A keypad matrix example
 ├── interrupt.cpp  // Interrupt on pins

```
 
---
