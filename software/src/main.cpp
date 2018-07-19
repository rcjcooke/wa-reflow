#include <Arduino.h>
#include "max6675.h"
#include <SPI.h>
#include "ReflowTFT.hpp"
#include "SR1230.hpp"


/**************************
 * Pin Definitions
 **************************/
/**
 * Note: On a Nano, the channel A pin and the encoder switch pin MUST be 
 * either pin 2 or 3 because these are the only two pins that the Arduino
 * can attach an interrupt to. We're only triggering off channel A (which
 * reduces our resolution but this doesn't matter for a simple human input
 * device).
 */
static const uint8_t ENCODER_CHANNEL_A_PIN = 2;
static const uint8_t ENCODER_CHANNEL_B_PIN = 7;
static const uint8_t ENCODER_SWITCH_PIN = 3;
static const uint8_t THERMOCOUPLE_CS_PIN = 4;
static const uint8_t TFT_CS_PIN = 7;
static const uint8_t TFT_DC_PIN = 9;
static const uint8_t TFT_RST_PIN = 8;

// Objects
ReflowTFT mTFTscreen = ReflowTFT(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);
MAX6675 mThermocouple = MAX6675();
SR1230 mEncoderSwitch = SR1230(ENCODER_CHANNEL_A_PIN, ENCODER_CHANNEL_B_PIN, ENCODER_SWITCH_PIN);

/**************************
 * Entry point methods
 **************************/
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
  // Set up the user input switch
  mEncoderSwitch.init();

  mTFTscreen.drawReflowSelectMenu();
}

void loop() {
  // Note intentional truncation from double to int for easy comparison
  int temp = mThermocouple.readCelsius();
  
  // Update the temperature text readout
  mTFTscreen.updateScreenTempText(temp);
  // Update the time remaining in this reflow text readout
  mTFTscreen.updateScreenTimeRemaining(120);
  
  delay(200);
}
