#include <Arduino.h>
#include <max6675.h>
#include <SPI.h>
#include "ReflowModel.hpp"
#include "ReflowController.hpp"
#include "reflowtft/ReflowTFT.hpp"
#include "selectionswitch/SelectionSwitch.hpp"
#include "ReflowHeater.hpp"
#include "Refreshable.hpp"

/**************************
 * Pin Definitions
 **************************/
#define REFLOW_MICROPROCESSOR MEGA

/**
 * SPI pins can be found here: https://www.arduino.cc/en/reference/SPI
 */
#if REFLOW_MICROPROCESSOR == NANO
  /**
   * Note: On a Nano, the channel A pin and the encoder switch pin MUST be 
   * either pin 2 or 3 because these are the only two pins that the Arduino
   * can attach an interrupt to. We're only triggering off channel A (which
   * reduces our resolution but this doesn't matter for a simple human input
   * device).
   */
  static constexpr uint8_t ENCODER_CHANNEL_A_PIN = 2;
  static constexpr uint8_t ENCODER_CHANNEL_B_PIN = 5;
  static constexpr uint8_t ENCODER_SWITCH_PIN = 3;
  static constexpr uint8_t THERMOCOUPLE_CS_PIN = 4;
  static constexpr uint8_t TFT_CS_PIN = 7;
  static constexpr uint8_t TFT_DC_PIN = 9;
  static constexpr uint8_t TFT_RST_PIN = 8;
  static constexpr uint8_t LOW_POWER_HEATING_ELEMENT_PIN = 10;
  static constexpr uint8_t HIGH_POWER_HEATING_ELEMENT_PIN = 6;
#elif REFLOW_MICROPROCESSOR == MEGA
  /**
   * Note: On a Nano, the channel A pin and the encoder switch pin MUST be 
   * either pin 2 or 3 because these are the only two pins that the Arduino
   * can attach an interrupt to. We're only triggering off channel A (which
   * reduces our resolution but this doesn't matter for a simple human input
   * device).
   */
  static constexpr uint8_t ENCODER_CHANNEL_A_PIN = 2;
  static constexpr uint8_t ENCODER_CHANNEL_B_PIN = 5;
  static constexpr uint8_t ENCODER_SWITCH_PIN = 3;
  static constexpr uint8_t THERMOCOUPLE_CS_PIN = 4;
  static constexpr uint8_t TFT_CS_PIN = 7;
  static constexpr uint8_t TFT_DC_PIN = 9;
  static constexpr uint8_t TFT_RST_PIN = 8;
  static constexpr uint8_t LOW_POWER_HEATING_ELEMENT_PIN = 10;
  static constexpr uint8_t HIGH_POWER_HEATING_ELEMENT_PIN = 6;
#endif

// Refresh rates in milliseconds
static constexpr unsigned long SCREEN_REFRESH_PERIOD = 100;
static constexpr unsigned long USER_SWITCH_REFRESH_PERIOD = 10;
static constexpr unsigned long HEATER_REFRESH_PERIOD = 1000;
static constexpr unsigned long CONTROLLER_REFRESH_PERIOD = 10;

// Inerrupt function declarations for reference
void userTurnInterruptDispatch();
void userSelectButtonPressInterruptDispatch();

// Global Objects
ReflowModel gReflowModel = ReflowModel();
ReflowController gReflowController = ReflowController(&gReflowModel);
ReflowTFT gTFTscreen = ReflowTFT(&gReflowModel, TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);
MAX6675 gThermocouple = MAX6675();
SelectionSwitch gSelectionSwitch = SelectionSwitch(&gReflowModel, &gReflowController, &userTurnInterruptDispatch, &userSelectButtonPressInterruptDispatch, ENCODER_CHANNEL_A_PIN, ENCODER_CHANNEL_B_PIN, ENCODER_SWITCH_PIN);
ReflowHeater gReflowHeater = ReflowHeater(&gReflowModel, LOW_POWER_HEATING_ELEMENT_PIN, HIGH_POWER_HEATING_ELEMENT_PIN);

// Update timers
unsigned long gPreviousScreenRefresh = 0;
unsigned long gPreviousSelectionSwitchRefresh = 0;
unsigned long gPreviousHeaterRefresh = 0;
unsigned long gPreviousControllerRefresh = 0;

/**************************
 * Interrupt function calls
 **************************/
void userTurnInterruptDispatch() {
  gSelectionSwitch.userTurnInterrupt();
}

void userSelectButtonPressInterruptDispatch() {
  gSelectionSwitch.userSelectButtonPressInterrupt();
}

/**************************
 * Entry point methods
 **************************/
void initThermocouple() {
  gThermocouple.begin(THERMOCOUPLE_CS_PIN);
  // Wait for MAX6675 chip to thermally stabilise
  delay(500);
}

void setup() {
  // Start serial comms for debug
  Serial.begin(115200);
  // Start SPI comms with devices
  SPI.begin();

  // Initialise the controller and model
  gReflowController.init();
  // Set up the display
  gTFTscreen.init();
  // Set up the thermocouple
  initThermocouple();
  // Set up the user input switch
  gSelectionSwitch.init();
  // Set up the heater
  gReflowHeater.init();
}

void checkAndRefresh(unsigned long* previousRefreshTimeMillisPtr, unsigned long refreshPeriod, Refreshable* refreshable) {
  unsigned long currentMillis = millis();
  // Intentially subtracting and comparing this way to ensure millis rollover doesn't cause problems
  if ((unsigned long) (currentMillis - *previousRefreshTimeMillisPtr) >= refreshPeriod) {
    refreshable->refresh();
    *previousRefreshTimeMillisPtr = currentMillis;
  }
}

void loop() {
  // Note intentional truncation from double to int for easy comparison
  int16_t temp = gThermocouple.readCelsius();
  gReflowModel.setOvenTemp(temp);

  // Check to see if the user has done anything
  checkAndRefresh(&gPreviousSelectionSwitchRefresh, USER_SWITCH_REFRESH_PERIOD, &gSelectionSwitch);
  // Refresh the screen for the user
  checkAndRefresh(&gPreviousScreenRefresh, SCREEN_REFRESH_PERIOD, &gTFTscreen);
  // Get the controller to update the model as required
  checkAndRefresh(&gPreviousControllerRefresh, CONTROLLER_REFRESH_PERIOD, &gReflowController);
  // Make sure the heater is doing sensible things
  checkAndRefresh(&gPreviousHeaterRefresh, HEATER_REFRESH_PERIOD, &gReflowHeater);

  delay(200);

  // TODO: temp for testing - delete the line below once the switch is wired up
  gReflowController.startReflow();
}
