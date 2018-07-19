#ifndef __REFLOWTFT_H_INCLUDED__
#define __REFLOWTFT_H_INCLUDED__

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

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

// Locations
static const int16_t TEMP_TEXT_X_POS = 5;
static const int16_t TEMP_TEXT_Y_POS = 90;
static const int16_t TIME_TEXT_X_POS = 5;
static const int16_t TIME_TEXT_Y_POS = 110;

// Colours
static const uint16_t BUTTON_BORDER_COLOUR = WHITE;
static const uint16_t START_BUTTON_FILL_COLOUR = GREEN;
static const uint16_t ABORT_BUTTON_FILL_COLOUR = RED;
static const uint16_t START_BUTTON_TEXT_COLOUR = BLACK;
static const uint16_t ABORT_BUTTON_TEXT_COLOUR = WHITE;

// Pre-defined strings
static const char TEMP_END_C_STRING[3] = { (char) 247, 'C', '\0'}; // degrees celcius
static const char TIME_END_C_STRING[] = "s"; // seconds
static char START_BUTTON_TEXT[] = "Start"; // has to be non-const to avoid adafruit_gfx gettextbounds issue - annoyingly

class ReflowTFT : Adafruit_ST7735 {

public:
  // Constructor
  ReflowTFT(int8_t cs, int8_t dc, int8_t rst) : Adafruit_ST7735(cs, dc, rst) {};
  
  // Initialise the display
  void init();
  // Update the temperature text display
  void updateScreenTempText(int temp);  
  // Update the remaining time text display
  void updateScreenTimeRemaining(int secondsRemaining);
  // Draw the reflow paste selection menu
  void drawReflowSelectMenu();

protected:
  // Draw the start button
  void drawStartButton();
  // Draw a round rectangle cornered button around some text
  void drawRoundRectangleButton(char* buttonText, uint16_t textColour, uint16_t buttonColour, int16_t x, int16_t y );
};

#endif // __REFLOWTFT_H_INCLUDED__