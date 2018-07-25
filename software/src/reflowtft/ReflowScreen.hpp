#ifndef __REFLOWSCREEN_H_INCLUDED__
#define __REFLOWSCREEN_H_INCLUDED__

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

#include "../Refreshable.hpp"
#include "../ReflowModel.hpp"

// Colour definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

static const uint16_t BUTTON_BORDER_COLOUR = WHITE;
static const uint8_t BUTTON_TEXT_SIZE = 1;

// Pre-defined strings
static constexpr char TEMP_END_C_STRING[3] = { (char) 247, 'C', '\0'}; // degrees celcius

class ReflowScreen : public Refreshable {

public:
  // Constructor
  ReflowScreen(ReflowModel* reflowModel, Adafruit_ST7735* screen) : mScreen(screen), mReflowModel(reflowModel) {};

protected:
  // Updates some text on the screen (clears the previous text off first)
  void updateScreenText(const char* prevText, const char* text, uint8_t size, uint16_t bgColour, uint16_t txtColour, int16_t x, int16_t y);
  // Draw a round rectangle cornered button around some text
  void drawRoundRectangleButton(char* buttonText, uint16_t textColour, uint16_t buttonColour, int16_t x, int16_t y );

  // The TFT screen this displays on
  Adafruit_ST7735* mScreen;
  // Reference to the oven model
  ReflowModel* mReflowModel;

};

#endif // __REFLOWSCREEN_H_INCLUDED__