## 🛸 MCP23017 I2C Expander


The MCP23017 expander is an excellent GPIO extension for Raspberry Pi, Microcontroller and many other platforms with I2C interface.

It features 16 additional GPIOs that can be configured as output and input pins.

This MCP23017 C++ API simplifies working with the I2C interface, configuring the MCP, and switching the pins for your project.

The functions are kept simple and restore all the functionalities of the expander.


Perfect for students, hobbyists and projects where GPIO control is simply needed –without having to delve deeper into the expander's registers.

---

---

## ✨ Features

✔️ Simple GPIO input/output 

✔️ One-line pin configuration 

✔️ Interrupt support (RISING, FALLING, BOTH)

✔️ Interrupt read and capture pin level

✔️ Interrupt Pin Output Mode (Rising/Falling POL, Open-Drain, Separate A/B Output Pins)

---

---

## ❗Basic

 📱 A fast way!
 
The basic functions are simple and clearly designed.

Just three lines of code will get the MCP working for you and complete your project.


1️⃣ Configure

2️⃣ Write

3️⃣ Read



🥵 You want to make an LED sweat? 


No problem! Use the `pinMode()` function and assign any MCP pin as the output.

With `pinWrite()`, you can animate the LED by switching it between high and low.



🔥 Or perhaps you'd like to make a button glow red-hot?


Use `pinMode()` and configure a pin as an input.

Want a stable high for your low flag but don't have a resistor ready to hand?

Extend the input pin with an internal pull-up resistor.

And now what? Yes, `pinRead()` keeps you informed, button pressed or released.

There are no limits to the output and further processing.

---

---

## 🤖 Interrupts 

💡 You don't have to configure everything.

The interrupt function is very easy to use – for more control,
you can also enable fine-tuning options like mirroring, open-drain, or slow edges.

The process is always the same:

1️⃣ Set the mode (output behavior and trigger type)

2️⃣ Enable interrupt

3️⃣ Query what happened

4️⃣ Optionally reset or confirm

Nothing is lost.

Whether you query the level live, view the bitmask, or use the capture function – the information remains stored until you explicitly delete it or confirm it by reading GPIO.

Capture is like a photo.

The `getIntCapture()` function records the state that was on the pin at the exact moment the interrupt was triggered.

Perfect if you want to know why the interrupt occurred – not just that it happened.

Deleting is optional.

All query functions have an optional `clear` parameter.

You decide: read values only, or also acknowledge and clear them.

Single pin or all at once.

There are dedicated functions like `clearInterrupt(pin)` for individual pins,

and convenient functions like `clearInterrupts()` to clear everything at once.

📝 A point for users to understand:

The interrupts on the MCP23017 are not "actively running," but rather "reporting and waiting."

Once an event is triggered, it remains stored—until you say:

"Thanks, I see."

That's exactly what the clear functions are for.


## 🧩 Provided Functions

```
| Function                                         | Description                       |
|--------------------------------------------------|-----------------------------------|
| `pinMode(pin, Output/Input/Pullup)`              | Configures pin direction          |

| `pinWrite(pin, HIGH/LOW)`                        | Sets pin output state             |

| `pinRead(pin)`                                   | Reads digital input               |

| `enableInt(pin, true/false)`                     | Enable Interrupts on Pin          |

| `intOutputMode(HIGH/LOW, ODR true, MIRROR true)` | Level, open-drain, seperate A/B   |

| `intTriggerMode(RISING, FALLING, BOTH)`          | Rising, Falling, Both -Flags      |    

| `getInterruptFlags(INTFA/B clear true)`          | Outputs Int events as bitmask (**)|

| `getInterruptPins(INTFA/B clear true)`           | Outputs Int events as pinnum (**) |

| `getIntCapture(INTCAPA/B clear true)`            | Edge of the interrupt event (**)  |

| `clearIntCapture(pin)`                           | Reset the capture bit             |

| `clearInterrupts()`                              | Reset Int configure and values    |

|--------------------------------------------------|-----------------------------------|
(**) optional reset -> default as false
```

---

---

## 🎁 Take a look at the examples.

Exemplares as inspiration and ideas for your project.


📁 examples/
```
 ├── blink.cpp      // Make individual LEDs blink
 ├── taster.cpp     // Query buttons
 ├── highlow.cpp    // Set Pin high/low
 └── keypad.cpp     // A keypad matrix example
 ├── interrupt.cpp  // Interrupt on pins

```

---

---

## 📐The functions explained
```cpp
/* Set interrupt output pins
*
*  intOutputMode(HIGH/LOW, TRUE/FALSE, TRUE/FALSE)
*
*  INTPOL  = The output pin can be set to either a falling or rising edge.
*            HIGH: Interrupt pin goes HIGH on event
*            LOW:  Interrupt pin goes LOW on event
*  ODR     = True: For external pull-up, INTPOL is automatically set to default.
*  MIRROR  = True: INTA and INTB Pins separately, default is false. If you don't need this setting, you can leave it out.
*
*/


/* Set interrupt input pins
*
*  intTriggerMode(PIN, CHANGE/RISING/FALLING)
*
*  CHANGE   = Every change of flank
*  RISING   = On rising flank
*  FALLING  = On a falling flank
*
*/


/* Enable/Disable the Interrupt Pin
*
*  enableInt(PIN, TRUE/FALSE)
*
*/


/* Interrupt output as Pinnums
*
*  getInterruptPins() **
*
*/


/* Interrupt output as bitmask
*
*  getInterruptFlags() **
*  
*/


/* Interrupt Capture reproduces the state of the pins during an interrupt.
*
*  getIntCapture() **
* 
*/

// ** Optionally, the values can be reset after output with true, default is false. If you don't need this setting, you can leave it out.


/* Interrupt flag as boolean
*
*  isInterruptOnPin(PIN)
*  
*  Optionally, the values can be reset after output, default is false. If you don't need this setting, you can leave it out.
*
*/


/* Interrupt Capture reset a specific pin.
*
*  clearIntCapture(PIN)
*
*  INTCAP can only be read, but according to the datasheet, reading the GPIOs is sufficient for resetting.
*  
*/


/* Interrupt clean up.
*
*  clearInterrupts()
*
*  All interrupt settings and values ​​will be reset.
*  
*/

/* MCP configure. 
*
*  DISSLW  = True: Only in cases of long cable runs or EMI problems I2C SDA, default is false. If you don't need this setting, you can leave it out.
```

---
