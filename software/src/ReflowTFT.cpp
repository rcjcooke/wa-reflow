#include "ReflowTFT.hpp"

char tempText[6] = "";
int previousTemp = 0;

void ReflowTFT::init() {
  initR(INITR_144GREENTAB);
  setRotation(ROTATE_0_DEGREES);
  fillScreen(BLACK);
  setTextSize(2);
  // Tried fonts to make it look nicer but had real problems with degree symbol
  setFont();
}

void ReflowTFT::updateScreenTempText(int temp) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (temp != previousTemp) {    
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
