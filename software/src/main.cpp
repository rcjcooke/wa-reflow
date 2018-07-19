#include <Arduino.h>
#include "max6675.h"
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

#include "SerialDebugger.hpp"

// Colour definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

// Rotation constants for the Adafruit TFT library
static const uint8_t ROTATE_0_DEGREES = 0;
static const uint8_t ROTATE_90_DEGREES = 1;
static const uint8_t ROTATE_180_DEGREES = 2;
static const uint8_t ROTATE_270_DEGREES = 3;

// Pin definitions
static const uint8_t THERMOCOUPLE_CS_PIN = 4;
static const uint8_t TFT_CS_PIN = 7;
static const uint8_t TFT_DC_PIN = 9;
static const uint8_t TFT_RST_PIN = 8;

// Locations
static const int16_t TEMP_TEXT_X_POS = 5;
static const int16_t TEMP_TEXT_Y_POS = 100;

// Pre-defined strings
static const char TEMP_END_C_STRING[] = { (char) 247, 'C', '\0'}; // degrees celcius

// Variables
Adafruit_ST7735 mTFTscreen = Adafruit_ST7735(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);
MAX6675 mThermocouple;
char tempText[6] = "";
SerialDebugger* mDebugger;
int previousTemp = 0;

void setup() {
  SPI.begin();
  mDebugger = new SerialDebugger();

  // Set up the display
  //mTFTscreen.begin();
  mTFTscreen.initR(INITR_144GREENTAB);
  mTFTscreen.setRotation(ROTATE_0_DEGREES);
  mTFTscreen.fillScreen(BLACK);
  mTFTscreen.setTextSize(1);
  mTFTscreen.setFont();
  // Set up the thermocouple
  mThermocouple.begin(THERMOCOUPLE_CS_PIN);
  // wait for MAX chip to thermally stabilize
  delay(500);
}

void loop() {
  // Note intentional truncation from double to int for easy comparison
  int temp = mThermocouple.readCelsius();
  
  // Update the temperature text readout
  updateScreenTempText(temp);

  delay(200);
}

void updateScreenTempText(int temp) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (temp != previousTemp) {    
    mTFTscreen.setTextColor(BLACK);
    mTFTscreen.setCursor(TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);
    mTFTscreen.print(tempText);

    // Update text
    dtostrf(temp, 3, 0, tempText);
    strcat(tempText, TEMP_END_C_STRING);
    mTFTscreen.setTextColor(YELLOW);    
    mTFTscreen.setCursor(TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);
    mTFTscreen.print(tempText);

    previousTemp = temp;
  }
}
