/*******************************************************
 * @file Interrupt_example.ino
 *
 * @brief Example demonstrating interrupt detection
 *        using the 7Semi MCP23017 I/O Expander.
 *
 * This sketch shows how to use the MCP23017's interrupt-on-change
 * capability to detect button presses using GPB0 and respond to
 * them in software.
 *
 * Key features demonstrated:
 * - Hardware interrupt configuration (STATE_CHANGE)
 * - Input pin with internal pull-up
 * - Output pin toggling (GPA0 as LED)
 * - Interrupt flag checking and clearing
 *
 * Connections:
 * - VCC   -> 3.3V or 5V
 * - GND   -> GND
 * - SDA   -> A4 (Uno)
 * - SCL   -> A5 (Uno)
 * - GPA0  -> LED (output)
 * - GPB0  -> Button (input with pull-up + interrupt)
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

MCP23017_7Semi mcp(0x20);

void setup() {
  Serial.begin(115200);

  if (!mcp.begin())
    Serial.println("MCP23017 init failed!");
  Serial.println("MCP23017 Initialized");

  // Set GPA0 as output (LED)
  mcp.pinMode(GPA0, OUTPUT);
  mcp.digitalWrite(GPA0, LOW);

  // Set GPB0 as input with pull-up (button)
  mcp.pinMode(GPB0, INPUT);
  mcp.pullUp(GPB0, true);  // Enable pull-up

  // Enable interrupt on change for GPB0
  mcp.enableInterrupt(GPB0, STATE_CHANGE, LOW);  // Interrupt if pin state  change
  // mcp.enableInterrupt(GPB0, STATE_CHANGE, LOW);  // Interrupt if pin state not matched
}

void loop() {
  // Blink LED every second
  mcp.togglePin(GPA0);
  // Check if button triggered interrupt
    if (mcp.getInterrupt(GPB0)) {
      Serial.println("Interrupt Detected on Button!");
      mcp.clearInterrupt();  // Clear interrupt flags
    }
  delay(1000);
}
