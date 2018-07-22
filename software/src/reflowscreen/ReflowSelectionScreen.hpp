#ifndef __REFLOWSELECTIONSCREEN_H_INCLUDED__
#define __REFLOWSELECTIONSCREEN_H_INCLUDED__

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

#include "ReflowScreen.hpp"
#include "../Refreshable.hpp"
#include "../ReflowModel.hpp"

class ReflowSelectionScreen : public ReflowScreen {

public:
  // Constructor
  ReflowSelectionScreen(ReflowModel* reflowModel, Adafruit_ST7735* screen) : ReflowScreen(reflowModel, screen) {};

  // Update the temperature text display
  void updateScreenTempText(uint16_t temp);
  // Draw the reflow selection screen from scratch
  void drawScreen();
  // Called whenever the screen needs to check for model updates
  void refresh();

protected:

  // Locations and lengths
  static constexpr int16_t TEMP_TEXT_X_POS = 5;
  static constexpr int16_t TEMP_TEXT_Y_POS = 90;

  // Colours
  static constexpr uint16_t TEMP_TEXT_COLOUR = YELLOW;
  static constexpr uint16_t START_BUTTON_FILL_COLOUR = RED;
  static constexpr uint16_t START_BUTTON_TEXT_COLOUR = WHITE;

  // Sizes
  static constexpr uint8_t TEMP_TEXT_SIZE = 2;

  // Pre-defined strings
  /** 
   * Note: ridiculous line below for two reasons:
   * 1. has to be non-const to avoid adafruit_gfx gettextbounds issue - annoyingly
   * 2. compiler used by platformio for Arduino has incomplete support for C++ 11
   * https://stackoverflow.com/questions/35413821/how-to-fix-this-array-used-as-initializer-error
   */
  char START_BUTTON_TEXT[6] = {'S', 't', 'a', 'r', 't', '\0'};

  // Draw the abort button
  void drawStartButton();

private:

  ReflowModel mLocalModel = ReflowModel();
  char mCurTempText[6];

};

#endif // __REFLOWSELECTIONSCREEN_H_INCLUDED__