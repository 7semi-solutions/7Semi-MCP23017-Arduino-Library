# 7Semi-MCP23017-Arduino-Library

This Arduino library provides support for the **7Semi MCP23017 I/O Expander Module**, a 16-bit GPIO expander controlled via I2C. It allows easy control of up to 16 digital input/output pins using just two wires.

![Arduino](https://img.shields.io/badge/platform-arduino-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg)

---

## Hardware Required

- 7Semi MCP23017 I/O Expander Module  
- Arduino-compatible board  
- I2C connection (SDA, SCL)  

---

## Getting Started

### 1. Installation via Arduino Library Manager

1. Open the **Arduino IDE**
2. Go to:
   - `Sketch > Include Library > Manage Libraries…` (IDE 1.x), or  
   - Use the 📚 Library Manager in IDE 2.x sidebar
3. Search:
   - 7Semi MCP23017
4. Click **Install**

Then include in your sketch:

#include <7semi_mcp23017.h>

### 2.Wiring

| MCP23017 Pin | Arduino Pin           |
| ------------ | --------------------- |
| VCC          | 3.3V or 5V            |
| GND          | GND                   |
| SDA          | A4 (Uno)              |
| SCL          | A5 (Uno)              |

### 3.Applications

I/O expansion for microcontrollers

Home automation and relay control

Button matrix reading

Industrial control panels

