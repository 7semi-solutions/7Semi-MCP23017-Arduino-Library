/*******************************************************
 * @file basic.ino
 *
 * @brief Basic example demonstrating GPIO control with
 *        the 7Semi MCP23017 I/O Expander over I2C.
 *
 * This sketch initializes the MCP23017 at address 0x20,
 * sets GPA0 as output (e.g., LED), and GPB0 as input (e.g., Button)
 * with internal pull-up. It toggles the output and prints
 * button state to the Serial Monitor every 500ms.
 *
 * Key features demonstrated:
 * - pinMode, digitalRead, digitalWrite, togglePin
 * - Internal pull-up resistor configuration
 * - Real-time I/O expansion via I2C
 *
 * Connections:
 * - VCC  -> 3.3V or 5V
 * - GND  -> GND
 * - SDA  -> A4 (Uno)
 * - SCL  -> A5 (Uno)
 * - GPA0 -> LED (Output)
 * - GPB0 -> Button (Input with pull-up)
 *
 * @section author Author
 * Written by 7Semi
 *
 * @section version Version
 * 1.0 - 16-Sep-2025
 *
 * @section license License
 * @license MIT
 * Copyright (c) 2025 7Semi
 *******************************************************/

#include <7Semi_MCP23017.h>

// MCP23017 at default address 0x20 on default Wire bus
MCP23017_7Semi mcp(0x20);

void setup() {
  Serial.begin(115200);

  // Initialize MCP23017
  if (!mcp.begin()) {
    Serial.println("MCP23017 init failed!");
    while (1);
  }
  Serial.println("MCP23017 initialized.");

  // Set GPA0 as output (LED)
  mcp.pinMode(GPA0, OUTPUT);

  // Set GPB0 as input (Button) with pull-up enabled
  mcp.pinMode(GPB0, INPUT);
  mcp.pullUp(GPB0, true);
}

void loop() {
  // Toggle LED every 500 ms
  mcp.togglePin(GPA0);

  // Read the button state
  uint8_t state = mcp.digitalRead(GPB0);

  // Print button state
  if (state == LOW) {
    Serial.println("Button Pressed");
  } else {
    Serial.println("Button Released");
  }

  delay(500);
}
