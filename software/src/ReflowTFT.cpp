#include "ReflowTFT.hpp"

char tempText[6] = "";
char timeText[5] = "";
int previousTemp = 0;
int previousSecondsRemaining = 0;

void ReflowTFT::init() {
  initR(INITR_144GREENTAB);
  setRotation(ROTATE_0_DEGREES);
  fillScreen(BLACK);
  setTextSize(2);
  // Tried fonts to make it look nicer but had real problems with degree symbol
  setFont();
}

void ReflowTFT::drawReflowSelectMenu() {
  drawStartButton();
}

void ReflowTFT::updateScreenTimeRemaining(int secondsRemaining) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (secondsRemaining != previousSecondsRemaining) {
    setTextSize(2);
    // Clear off existing
    setTextColor(BLACK);
    setCursor(TIME_TEXT_X_POS, TIME_TEXT_Y_POS);
    print(timeText);

    // Update text
    itoa(secondsRemaining, timeText, 10);
    strcat(timeText, TIME_END_C_STRING);
    setTextColor(YELLOW);    
    setCursor(TIME_TEXT_X_POS, TIME_TEXT_Y_POS);
    print(timeText);

    previousSecondsRemaining = secondsRemaining;
  }
}

void ReflowTFT::updateScreenTempText(int temp) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (temp != previousTemp) {
    setTextSize(2);        
    // Clear off existing
    setTextColor(BLACK);
    setCursor(TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);
    print(tempText);

    // Update text
    dtostrf(temp, 3, 0, tempText);
    strcat(tempText, TEMP_END_C_STRING);
    setTextColor(YELLOW);    
    setCursor(TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);
    print(tempText);

    previousTemp = temp;
  }
}

void ReflowTFT::drawStartButton() {
  drawRoundRectangleButton(START_BUTTON_TEXT, START_BUTTON_TEXT_COLOUR, START_BUTTON_FILL_COLOUR, 75, 95);
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