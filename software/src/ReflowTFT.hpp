#ifndef __REFLOWTFT_H_INCLUDED__
#define __REFLOWTFT_H_INCLUDED__

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

#include "reflowprofile/ReflowProfile.hpp"

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

// Locations and lengths
static const int16_t TEMP_TEXT_X_POS = 5;
static const int16_t TEMP_TEXT_Y_POS = 90;
static const int16_t TIME_TEXT_X_POS = 5;
static const int16_t TIME_TEXT_Y_POS = 110;
static const int16_t STATE_TEXT_X_POS = 20;
static const int16_t STATE_TEXT_Y_POS = 65;
static const int16_t GRAPH_ORIGIN_X = 20;
static const int16_t GRAPH_ORIGIN_Y = 45;
static const int16_t GRAPH_HEIGHT = 40;
static const int16_t GRAPH_WIDTH = 100;
static const int16_t TICK_LENGTH = 2;

// Colours
static const uint16_t BUTTON_BORDER_COLOUR = WHITE;
static const uint16_t TEMP_TEXT_COLOUR = YELLOW;
static const uint16_t TIME_TEXT_COLOUR = YELLOW;
static const uint16_t STATE_TEXT_COLOUR = WHITE;
static const uint16_t START_BUTTON_FILL_COLOUR = GREEN;
static const uint16_t ABORT_BUTTON_FILL_COLOUR = RED;
static const uint16_t START_BUTTON_TEXT_COLOUR = BLACK;
static const uint16_t ABORT_BUTTON_TEXT_COLOUR = WHITE;
static const uint16_t GRAPH_AXIS_COLOUR = WHITE;

// Sizes
static const uint8_t TEMP_TEXT_SIZE = 2;
static const uint8_t TIME_TEXT_SIZE = 2;
static const uint8_t STATE_TEXT_SIZE = 2;
static const uint8_t BUTTON_TEXT_SIZE = 1;
static const uint8_t GRAPH_LABEL_TEXT_SIZE = 1;

// Pre-defined strings
static const char TEMP_END_C_STRING[3] = { (char) 247, 'C', '\0'}; // degrees celcius
static const char TIME_END_C_STRING[] = "s"; // seconds
static char START_BUTTON_TEXT[] = "Start"; // has to be non-const to avoid adafruit_gfx gettextbounds issue - annoyingly
static char ABORT_BUTTON_TEXT[] = "Abort"; // has to be non-const to avoid adafruit_gfx gettextbounds issue - annoyingly

class ReflowTFT {

public:
  // Constructor
  ReflowTFT(ReflowModel* reflowModel, int8_t cs, int8_t dc, int8_t rst) : mScreen(Adafruit_ST7735(cs, dc, rst)), mReflowModel(reflowModel) {};
  
  // Initialise the display
  void init();

  // Update the reflow state text display
  void updateScreenStateText(const char* state);
  // Update the temperature text display
  void updateScreenTempText(uint16_t temp);  
  // Update the remaining time text display
  void updateScreenTimeRemaining(uint16_t secondsRemaining);
  // Draw the reflow paste selection menu
  void drawReflowSelectScreen();
  // Draw the reflow in progress screen
  void drawProgressScreen();
  // Called whenever the screen needs to check for model updates
  void refresh();

protected:
  // Updates some text on the screen (clears the previous text off first)
  void updateScreenText(const char* prevText, const char* text, uint8_t size, uint16_t bgColour, uint16_t txtColour, int16_t x, int16_t y);
  
  // Draw the start button
  void drawStartButton();
  // Draw the abort button
  void drawAbortButton();
  // Draw a round rectangle cornered button around some text
  void drawRoundRectangleButton(char* buttonText, uint16_t textColour, uint16_t buttonColour, int16_t x, int16_t y );

  // Draw the planned reflow profile graph
  void drawProfileGraph(ReflowProfile* profile, uint16_t startTemp);
  // Print a label on the graph Y Axis
  void printYAxisLabel(int16_t y, int16_t* ptxtby, uint16_t temp);
  // Print a label on the graph X Axis
  void printXAxisLabel(int16_t x, int16_t* ptxtbex, uint16_t seconds);

private:

  // The TFT screen this displays on
  Adafruit_ST7735 mScreen;
  // Reference to the oven model
  ReflowModel* mReflowModel;

};

#endif // __REFLOWTFT_H_INCLUDED__