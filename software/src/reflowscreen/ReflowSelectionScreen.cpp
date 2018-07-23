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
  drawProfileList();
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
  uint8_t listScreenStartIndex =
      mReflowModel->getNumProfiles() > MAX_LIST_LENGTH
          ? min(mSelectedProfileIndex,
                mReflowModel->getNumProfiles() - MAX_LIST_LENGTH + 1)
          : 0;

  uint8_t visibleListIndex = 0;
  for (uint8_t i = listScreenStartIndex;
       i < min(listScreenStartIndex + MAX_LIST_LENGTH,
               mReflowModel->getNumProfiles());
       i++) {
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
      mReflowModel->getReflowProfile(profileIndex)->getName();

  // Work out how big the text is
  int16_t txtx = PROFILE_LIST_X + PROFILE_LIST_CELL_BORDER, txty = PROFILE_LIST_Y + PROFILE_LIST_CELL_BORDER;
  int16_t txtbx, txtby;
  uint16_t txtw, itemTxtHeight;
  mScreen->setTextSize(LIST_TEXT_SIZE);
  mScreen->setTextWrap(false);
  mScreen->getTextBounds(listItemText, txtx, txty, &txtbx, &txtby, &txtw,
                         &itemTxtHeight);
  // Therefore work out how far down this list item goes
  txty = PROFILE_LIST_Y + PROFILE_LIST_CELL_BORDER + visibleListIndex * (itemTxtHeight + PROFILE_LIST_CELL_BORDER * 2);

  // If it's selected, highlight the background
  if (selected) {
    mScreen->getTextBounds(listItemText, txtx, txty, &txtbx, &txtby, &txtw,
                          &itemTxtHeight);
    mScreen->fillRect(txtx - PROFILE_LIST_X, txty - PROFILE_LIST_CELL_BORDER,
                      mScreen->width() - PROFILE_LIST_X - 5 -
                          LIST_TRIANGLE_ARROW_WIDTH - 5 - PROFILE_LIST_CELL_BORDER * 2,
                      itemTxtHeight + PROFILE_LIST_CELL_BORDER * 2, LIST_HIGHLIGHT_COLOUR);
  }
  
  // Print the name
  mScreen->setTextColor(PROFILE_LIST_ITEM_TEXT_COLOUR);
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
  mScreen->fillTriangle(
      mScreen->width() - 5 - LIST_TRIANGLE_ARROW_WIDTH / 2, //x0
      PROFILE_LIST_Y + PROFILE_LIST_HEIGHT - 5, //y0
      mScreen->width() - 5, //x1
      PROFILE_LIST_Y + PROFILE_LIST_HEIGHT - 5 - LIST_TRIANGLE_ARROW_WIDTH, //y1
      mScreen->width() - 5 - LIST_TRIANGLE_ARROW_WIDTH, //x2
      PROFILE_LIST_Y + PROFILE_LIST_HEIGHT - 5 - LIST_TRIANGLE_ARROW_WIDTH, LIST_ARROW_COLOUR); //y2
}