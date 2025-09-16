/*
  7Semi_MCP23017.cpp
  -----------------------------------------------------------------------------
  Implementation of the MCP23017 16-bit I²C GPIO expander driver.

  This source file defines all MCP23017_7Semi class functions:
  - I²C communication and register read/write helpers
  - Pin direction and digital I/O control
  - Internal pull-up and polarity inversion configuration
  - Interrupt enable/disable, flag reading, and clearing
  - Support for user-defined I²C pins and frequency

  Notes
  - Works with any TwoWire bus instance (default = Wire)
  - Use begin(sda, scl, freq) to select custom pins (if platform supports)
  - Interrupt flags are cleared by reading INTCAPx or GPIOx registers
  - Bit operations on uint8_t should use casts (uint8_t)(1U << n) to avoid warnings

  Supported Platforms
  - Arduino AVR (UNO, Nano, etc.)
  - ESP32

  Author  : 7semi
  License : MIT
  Version : 1.0.0
*/
#include "7Semi_MCP23017.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"
// Constructor: save address and I2C bus
MCP23017_7Semi::MCP23017_7Semi(uint8_t address, TwoWire* i2c) {
  _address = address;
  _i2c = i2c;
}
bool MCP23017_7Semi::begin(int sda, int scl, uint32_t freq) {
// Start I2C with user-defined or default pins
#if defined(ARDUINO)
  _i2c->begin();
#elif defined(ESP8266)
  if (sda >= 0 && scl >= 0) _i2c->begin(sda, scl);
  else _i2c->begin();
#elif defined(ESP32)
  if (sda >= 0 && scl >= 0) _i2c->begin(sda, scl, freq);
  else _i2c->begin(21, 22);
#elif defined(ARDUINO_ARCH_STM32)
  // Many STM32 boards use fixed pins from variant — if supported, you can:
  if (sda >= 0 && scl >= 0) _i2c->setSDA(sda), _i2c->setSCL(scl);
  _i2c->begin();
#else
  _i2c->begin();
#endif

  // Check if device responds
  _i2c->beginTransmission(_address);
  if (_i2c->endTransmission() != 0) return false;

  if (!writeReg(IODIRA, 0x00)) return false;
  if (!writeReg(IODIRB, 0x00)) return false;
  if (!writeReg(GPPUA, 0x00)) return false;
  if (!writeReg(GPPUB, 0x00)) return false;

  return true;
}


void MCP23017_7Semi::pinMode(enum pins pin, uint8_t mode) {
  if (pin < 8) {
    uint8_t iodir = readReg(IODIRA);
    if (mode == INPUT) iodir |= (1 << pin);
    else iodir &= ~(1 << pin);
    writeReg(IODIRA, iodir);
  } else {
    uint8_t iodir = readReg(IODIRB);
    if (mode == INPUT) iodir |= (1 << (pin - 8));
    else iodir &= ~(1 << (pin - 8));
    writeReg(IODIRB, iodir);
  }
}

void MCP23017_7Semi::digitalWrite(enum pins pin, bool value) {
  uint8_t currentState;
  if (pin < 8) {
    currentState = readReg(GPIOA);
    if (value == HIGH)
      currentState |= (1 << pin);
    else currentState &= ~(1 << pin);
    writeReg(OLATA, currentState);
  } else {
    currentState = readReg(GPIOB);
    if (value == HIGH) currentState |= (1 << (pin - 8));
    else currentState &= ~(1 << (pin - 8));
    writeReg(OLATB, currentState);
  }
}

uint8_t MCP23017_7Semi::digitalRead(enum pins pin) {
  return readPin(pin);
}

void MCP23017_7Semi::togglePin(enum pins pin) {
  uint8_t currentState = digitalRead(pin);
  digitalWrite(pin, (currentState == HIGH) ? LOW : HIGH);
}

bool MCP23017_7Semi::writeReg(uint8_t reg, uint8_t value) {
  _i2c->beginTransmission(_address);
  _i2c->write(reg);
  _i2c->write(value);
  if (_i2c->endTransmission() == 0)
    return true;
  return false;
}

uint8_t MCP23017_7Semi::readReg(uint8_t reg) {
  _i2c->beginTransmission(_address);
  _i2c->write(reg);
  _i2c->endTransmission();
  _i2c->requestFrom(_address, 1);
  return _i2c->read();
}

void MCP23017_7Semi::writePin(enum pins pin, uint8_t value) {
  uint8_t currentState;
  if (pin < 8) {
    currentState = readReg(GPIOA);
    if (value == HIGH)
      currentState |= (1 << pin);
    else currentState &= ~(1 << pin);
    writeReg(OLATA, currentState);
  } else {
    currentState = readReg(GPIOB);
    if (value == HIGH) currentState |= (1 << (pin - 8));
    else currentState &= ~(1 << (pin - 8));
    writeReg(OLATB, currentState);
  }
}

uint8_t MCP23017_7Semi::readPin(enum pins pin) {
  if (pin < 8) {
    return (readReg(GPIOA) & (1 << pin)) ? HIGH : LOW;
  } else {
    return (readReg(GPIOB) & (1 << (pin - 8))) ? HIGH : LOW;
  }
}

void MCP23017_7Semi::pullUp(enum pins pin, bool enable) {
  if (pin < 8) {
    uint8_t currentPullup = readReg(GPPUA);
    if (enable) currentPullup |= (1 << pin);
    else currentPullup &= ~(1 << pin);
    writeReg(GPPUA, currentPullup);
  } else {
    uint8_t currentPullup = readReg(GPPUB);
    if (enable) currentPullup |= (1 << (pin - 8));
    else currentPullup &= ~(1 << (pin - 8));
    writeReg(GPPUB, currentPullup);
  }
}

void MCP23017_7Semi::setPolarity(enum pins pin, bool invert) {
  if (pin < 8) {
    uint8_t currentPolarity = readReg(IPOLA);
    if (invert) currentPolarity |= (1 << pin);
    else currentPolarity &= ~(1 << pin);
    writeReg(IPOLA, currentPolarity);
  } else {
    uint8_t currentPolarity = readReg(IPOLB);
    if (invert) currentPolarity |= (1 << (pin - 8));
    else currentPolarity &= ~(1 << (pin - 8));
    writeReg(IPOLB, currentPolarity);
  }
}

void MCP23017_7Semi::enableInterrupt(enum pins pin, bool compareToDefval, bool defval) {
  uint8_t port = (pin < 8) ? 0 : 1;
  uint8_t bit = (pin < 8) ? pin : (pin - 8);
  if (port == 0) {
    uint8_t intcon = readReg(INTCONA);
    if (compareToDefval) intcon |= (1 << bit);
    else intcon &= ~(1 << bit);
    writeReg(INTCONA, intcon);

    uint8_t def = readReg(DEFVALA);
    if (defval) def |= (1 << bit);
    else def &= ~(1 << bit);
    writeReg(DEFVALA, def);

    uint8_t inte = readReg(INTEA);
    inte |= (1 << bit);
    writeReg(INTEA, inte);
  } else {
    uint8_t intcon = readReg(INTCONB);
    if (compareToDefval) intcon |= (1 << bit);
    else intcon &= ~(1 << bit);
    writeReg(INTCONB, intcon);

    uint8_t def = readReg(DEFVALB);
    if (defval) def |= (1 << bit);
    else def &= ~(1 << bit);
    writeReg(DEFVALB, def);

    uint8_t inte = readReg(INTEB);
    inte |= (1 << bit);
    writeReg(INTEB, inte);
  }
}

void MCP23017_7Semi::disableInterrupt(enum pins pin) {
  if (pin < 8) {
    uint8_t inte = readReg(INTEA);
    inte &= ~(1 << pin);
    writeReg(INTEA, inte);
  } else {
    uint8_t inte = readReg(INTEB);
    inte &= ~(1 << (pin - 8));
    writeReg(INTEB, inte);
  }
}

void MCP23017_7Semi::clearInterrupt() {
  readReg(INTCAPA);
  readReg(INTCAPB);
  writeReg(INTFA, 0x00);
  writeReg(INTFA, 0x00);
}
uint8_t MCP23017_7Semi::getInterrupt(enum pins pin) {
  if (pin < 8)
    return (readReg(INTFA) & (1 << pin)) ? 1 : 0;
  else
    return (readReg(INTFB) & (1 << (pin - 8))) ? 1 : 0;
}
