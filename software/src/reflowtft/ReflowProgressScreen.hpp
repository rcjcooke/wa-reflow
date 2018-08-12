#ifndef __REFLOWPROGRESSSCREEN_H_INCLUDED__
#define __REFLOWPROGRESSSCREEN_H_INCLUDED__

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

#include "ReflowScreen.hpp"

class ReflowProgressScreen : public ReflowScreen {

public:
  // Constructor
  ReflowProgressScreen(ReflowModel* reflowModel, Adafruit_ST7735* screen) : ReflowScreen(reflowModel, screen) {};

  // Update the reflow state text display
  void updateScreenStateText(ReflowState newState);
  // Update the temperature text display
  void updateScreenTempText(uint16_t temp);
  // Update the remaining time text display
  void updateScreenTimeRemaining(uint16_t secondsRemaining);
  // Draw the reflow in progress screen from scratch
  void drawScreen();
  // Called whenever the screen needs to check for model updates
  void refresh();

protected:

  // Locations and lengths
  static constexpr int16_t TEMP_TEXT_X_POS = 5;
  static constexpr int16_t TEMP_TEXT_Y_POS = 90;
  static constexpr int16_t TIME_TEXT_X_POS = 5;
  static constexpr int16_t TIME_TEXT_Y_POS = 110;
  static constexpr int16_t STATE_TEXT_X_POS = 20;
  static constexpr int16_t STATE_TEXT_Y_POS = 65;
  static constexpr int16_t GRAPH_ORIGIN_X = 20;
  static constexpr int16_t GRAPH_ORIGIN_Y = 45;
  static constexpr int16_t GRAPH_HEIGHT = 40;
  static constexpr int16_t GRAPH_WIDTH = 100;
  static constexpr int16_t TICK_LENGTH = 2;

  // Colours
  static constexpr uint16_t TEMP_TEXT_COLOUR = YELLOW;
  static constexpr uint16_t TIME_TEXT_COLOUR = YELLOW;
  static constexpr uint16_t STATE_TEXT_COLOUR = WHITE;
  static constexpr uint16_t ABORT_BUTTON_FILL_COLOUR = RED;
  static constexpr uint16_t ABORT_BUTTON_TEXT_COLOUR = WHITE;
  static constexpr uint16_t GRAPH_AXIS_COLOUR = WHITE;

  // Sizes
  static constexpr uint8_t TEMP_TEXT_SIZE = 2;
  static constexpr uint8_t TIME_TEXT_SIZE = 2;
  static constexpr uint8_t STATE_TEXT_SIZE = 2;
  static constexpr uint8_t GRAPH_LABEL_TEXT_SIZE = 1;

  // Pre-defined strings
  static constexpr char TIME_END_C_STRING[] = "s"; // seconds
  /** 
   * Note: ridiculous line below for two reasons:
   * 1. has to be non-const to avoid adafruit_gfx gettextbounds issue - annoyingly
   * 2. compiler used by platformio for Arduino has incomplete support for C++ 11
   * https://stackoverflow.com/questions/35413821/how-to-fix-this-array-used-as-initializer-error
   */
  char ABORT_BUTTON_TEXT[6] = {'A', 'b', 'o', 'r', 't', '\0'};

  // Draw the abort button
  void drawAbortButton();

  // Draw the planned reflow profile graph
  void drawProfileGraph(const ReflowProfile* profile, uint16_t startTemp);
  // Print a label on the graph Y Axis
  void printYAxisLabel(int16_t y, int16_t* ptxtby, uint16_t temp);
  // Print a label on the graph X Axis
  void printXAxisLabel(int16_t x, int16_t* ptxtbex, uint16_t seconds);
  // Plots the current temperature on the chart
  void plotCurrentTemp();
  
private:

  // Various current state variables to compare against the model
  int16_t mDisplayedOvenTemp = 0; 
  uint16_t mDisplayedSecondsRemaining = 0;
  ReflowState mDisplayedReflowState = ReflowState::Done;
  char mCurTempText[6];
  char mCurTimeText[5];
  char* mPreviousStateText;

  float mMillisPerPixel = 0.0;
  unsigned long mNextGraphPlotPointMillis = 0;
  float mDegreesPerPixel = 0.0;

};

#endif // __REFLOWPROGRESSSCREEN_H_INCLUDED__