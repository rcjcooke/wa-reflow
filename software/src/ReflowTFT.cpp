#include "ReflowTFT.hpp"

char curTempText[6] = "";
char curTimeText[5] = "";
int previousTemp = 0;
int previousSecondsRemaining = 0;
const char* previousState;

void ReflowTFT::init() {
  initR(INITR_144GREENTAB);
  setRotation(ROTATE_0_DEGREES);
  fillScreen(BLACK);
  setTextSize(2);
  // Tried fonts to make it look nicer but had real problems with degree symbol
  setFont();
}

void ReflowTFT::drawReflowSelectMenu() {
  drawAbortButton();
}

void ReflowTFT::updateScreenTimeRemaining(int secondsRemaining) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (secondsRemaining != previousSecondsRemaining) {
    char text[5] = "";
    itoa(secondsRemaining, text, 10);
    strcat(text, TIME_END_C_STRING);

    updateScreenText(curTimeText, text, TIME_TEXT_SIZE, BLACK, TIME_TEXT_COLOUR, TIME_TEXT_X_POS, TIME_TEXT_Y_POS);

    previousSecondsRemaining = secondsRemaining;
    strcpy(curTimeText, text);
  }
}

void ReflowTFT::updateScreenTempText(int temp) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (temp != previousTemp) {
    char text[6] = "";
    dtostrf(temp, 3, 0, text);
    strcat(text, TEMP_END_C_STRING);

    updateScreenText(curTempText, text, TEMP_TEXT_SIZE, BLACK, TEMP_TEXT_COLOUR, TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);

    previousTemp = temp;
    strcpy(curTempText, text);
  }
}

void ReflowTFT::updateScreenStateText(const char* state) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (state != previousState) {
    updateScreenText(previousState, state, STATE_TEXT_SIZE, BLACK, STATE_TEXT_COLOUR, STATE_TEXT_X_POS, STATE_TEXT_Y_POS);
    previousState = state;
  }
}

void ReflowTFT::updateScreenText(const char* prevText, const char* text, uint8_t size, uint16_t bgColour, uint16_t txtColour, int16_t x, int16_t y) {
  setTextSize(size);        
  
  // Clear off existing
  setTextColor(bgColour);
  setCursor(x, y);
  print(prevText);

  // Update text
  setTextColor(txtColour);    
  setCursor(x, y);
  print(text);
}

void ReflowTFT::drawStartButton() {
  drawRoundRectangleButton(START_BUTTON_TEXT, START_BUTTON_TEXT_COLOUR, START_BUTTON_FILL_COLOUR, 75, 95);
}

void ReflowTFT::drawAbortButton() {
  drawRoundRectangleButton(ABORT_BUTTON_TEXT, ABORT_BUTTON_TEXT_COLOUR, ABORT_BUTTON_FILL_COLOUR, 75, 95);
}

void ReflowTFT::drawRoundRectangleButton(char* buttonText, uint16_t textColour, uint16_t buttonColour, int16_t x, int16_t y ) {
  
  // Work out how big the text is
  int16_t txtx = x+10, txty = y+10;
  int16_t txtbx, txtby;
  uint16_t txtw, txth;
  setTextSize(1);
  getTextBounds(buttonText, txtx, txty, &txtbx, &txtby, &txtw, &txth);

  // fill
  fillRoundRect(x, y, txtw+20, txth+20, 4, buttonColour);
  // border
  drawRoundRect(x, y, txtw+20, txth+20, 4, BUTTON_BORDER_COLOUR);
  // text
  setCursor(txtx, txty);
  setTextColor(textColour);
  print(buttonText);

}

