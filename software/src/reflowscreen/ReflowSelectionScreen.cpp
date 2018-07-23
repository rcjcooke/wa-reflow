#include "ReflowSelectionScreen.hpp"

#include <Arduino.h>

void ReflowSelectionScreen::refresh() {
  // Check the temperature
  if (mReflowModel->getOvenTemp() != mLocalModel.getOvenTemp()) {
    // Only bother updating the screen if it's changed - avoids excessive
    // "blinking"
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

  updateScreenText(mCurTempText, text, TEMP_TEXT_SIZE, BLACK, TEMP_TEXT_COLOUR,
                   TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);

  strcpy(mCurTempText, text);
}

void ReflowSelectionScreen::drawStartButton() {
  drawRoundRectangleButton(START_BUTTON_TEXT, START_BUTTON_TEXT_COLOUR,
                           START_BUTTON_FILL_COLOUR, 75, 95);
}

void ReflowSelectionScreen::drawProfileList() {

  // Draw the list
  uint8_t listScreenStartIndex = min(
      mSelectedProfileIndex, mReflowModel->getNumProfiles() - MAX_LIST_LENGTH);
  uint8_t visibleListIndex = 0;
  for (uint8_t i = listScreenStartIndex; i < listScreenStartIndex + MAX_LIST_LENGTH; i++) {
    drawListCell(visibleListIndex, i, i == mSelectedProfileIndex);
    visibleListIndex++;
  }

  // Draw the "more list items" indicators - triangles on the side
  if (listScreenStartIndex > 0) {
    drawUpArrow();
  }
  if (listScreenStartIndex + MAX_LIST_LENGTH < mReflowModel->getNumProfiles()) {
    drawDownArrow();
  }

}

void ReflowSelectionScreen::drawListCell(uint8_t visibleListIndex,
                                         uint8_t profileIndex, bool selected) {

  char *listItemText =
      mReflowModel->getReflowProfiles()[profileIndex]->getName();

  // Work out how big the text is
  int16_t txtx = PROFILE_LIST_X, txty = PROFILE_LIST_Y;
  int16_t txtbx, txtby;
  uint16_t txtw, itemTxtHeight;
  mScreen->setTextSize(LIST_TEXT_SIZE);
  mScreen->getTextBounds(listItemText, txtx, txty, &txtbx, &txtby, &txtw,
                         &itemTxtHeight);
  // Therefore work out how far down this list item goes
  txty = PROFILE_LIST_Y + visibleListIndex * itemTxtHeight;
  mScreen->getTextBounds(listItemText, txtx, txty, &txtbx, &txtby, &txtw,
                         &itemTxtHeight);
  // If it's selected, highlight the background
  mScreen->fillRect(txtbx, txtby,
                    mScreen->width() - PROFILE_LIST_X - 5 -
                        LIST_TRIANGLE_ARROW_WIDTH - 5,
                    itemTxtHeight, LIST_HIGHLIGHT_COLOUR);
  // Print the name
  mScreen->setCursor(txtx, txty);
  mScreen->print(listItemText);
}

void ReflowSelectionScreen::drawUpArrow() {
  mScreen->fillTriangle(mScreen->width() - 5 - LIST_TRIANGLE_ARROW_WIDTH / 2, 5,
                        mScreen->width() - 5, 5 + LIST_TRIANGLE_ARROW_WIDTH,
                        mScreen->width() - 5 - LIST_TRIANGLE_ARROW_WIDTH,
                        5 + LIST_TRIANGLE_ARROW_WIDTH, LIST_ARROW_COLOUR);
}

void ReflowSelectionScreen::drawDownArrow() {
  mScreen->fillTriangle(mScreen->width() - 5 - LIST_TRIANGLE_ARROW_WIDTH / 2, PROFILE_LIST_Y + PROFILE_LIST_HEIGHT - 5,
                        mScreen->width() - 5, PROFILE_LIST_Y + PROFILE_LIST_HEIGHT,
                        mScreen->width() - 5 - LIST_TRIANGLE_ARROW_WIDTH,
                        PROFILE_LIST_Y + PROFILE_LIST_HEIGHT, LIST_ARROW_COLOUR);
}