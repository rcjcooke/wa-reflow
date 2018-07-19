#include <Arduino.h>
#include "max6675.h"
#include <SPI.h>
#include "ReflowTFT.hpp"

// Pin definitions
static const uint8_t THERMOCOUPLE_CS_PIN = 4;
static const uint8_t TFT_CS_PIN = 7;
static const uint8_t TFT_DC_PIN = 9;
static const uint8_t TFT_RST_PIN = 8;

// Variables
ReflowTFT mTFTscreen = ReflowTFT(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);
MAX6675 mThermocouple = MAX6675();

void initThermocouple() {
  mThermocouple.begin(THERMOCOUPLE_CS_PIN);
  // wait for MAX chip to thermally stabilize
  delay(500);
}

void setup() {
  // Start serial comms with devices
  SPI.begin();
  // Set up the display
  mTFTscreen.init();
  // Set up the thermocouple
  initThermocouple();
}

void loop() {
  // Note intentional truncation from double to int for easy comparison
  int temp = mThermocouple.readCelsius();
  
  // Update the temperature text readout
  mTFTscreen.updateScreenTempText(temp);

  delay(200);
}
