#include "ReflowSelectionScreen.hpp"

#include "../reflowprofile/SAC305.hpp" 

void ReflowSelectionScreen::refresh() {
  // Check the temperature
  if (mReflowModel->getOvenTemp() != mLocalModel.getOvenTemp()) {
    // Only bother updating the screen if it's changed - avoids excessive "blinking"
    updateScreenTempText(mReflowModel->getOvenTemp());
    mLocalModel.setOvenTemp(mReflowModel->getOvenTemp());
  }
}

void ReflowSelectionScreen::drawScreen() {
  drawStartButton();
  refresh();
}

void ReflowSelectionScreen::updateScreenTempText(uint16_t temp) {
  char text[6] = "";
  dtostrf(temp, 3, 0, text);
  strcat(text, TEMP_END_C_STRING);

  updateScreenText(mCurTempText, text, TEMP_TEXT_SIZE, BLACK, TEMP_TEXT_COLOUR, TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);

  strcpy(mCurTempText, text);
}

void ReflowSelectionScreen::drawStartButton() {
  drawRoundRectangleButton(START_BUTTON_TEXT, START_BUTTON_TEXT_COLOUR, START_BUTTON_FILL_COLOUR, 75, 95);
}
