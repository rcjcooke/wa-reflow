#ifndef __REFLOWTFT_H_INCLUDED__
#define __REFLOWTFT_H_INCLUDED__

#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include <Arduino.h>

#include "../ReflowModel.hpp"
#include "../Refreshable.hpp"
#include "ReflowSelectionScreen.hpp"
#include "ReflowProgressScreen.hpp"

// Colour definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// Rotation constants for the Adafruit TFT library
static const uint8_t ROTATE_0_DEGREES = 0;
static const uint8_t ROTATE_90_DEGREES = 1;
static const uint8_t ROTATE_180_DEGREES = 2;
static const uint8_t ROTATE_270_DEGREES = 3;

class ReflowTFT : public Refreshable {

public:
  // Constructor
  ReflowTFT(ReflowModel *reflowModel, int8_t cs, int8_t dc, int8_t rst)
      : mScreen(Adafruit_ST7735(cs, dc, rst)), mReflowModel(reflowModel),
        mReflowSelectionScreen(ReflowSelectionScreen(reflowModel, &mScreen)),
        mReflowProgressScreen(ReflowProgressScreen(reflowModel, &mScreen)){};

  // Initialise the display
  void init();

  // Called whenever the screen needs to check for model updates
  void refresh();

protected:
  // Draw the correct screen for the oven state
  void drawScreen();

private:
  // The TFT screen this displays on
  Adafruit_ST7735 mScreen;
  // Reference to the oven model
  ReflowModel *mReflowModel;

  // The selection screen
  ReflowSelectionScreen mReflowSelectionScreen;
  // The progress screen
  ReflowProgressScreen mReflowProgressScreen;

  // The current screen
  ReflowScreen* mCurrentScreen;
};

#endif // __REFLOWTFT_H_INCLUDED__