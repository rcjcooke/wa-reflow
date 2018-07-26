#include "ReflowProgressScreen.hpp"

#include "../reflowprofile/SAC305.hpp" 

void ReflowProgressScreen::refresh() {
  // Check the temperature
  if (mReflowModel->getOvenTemp() != mLocalModel.getOvenTemp()) {
    // Only bother updating the screen if it's changed - avoids excessive "blinking"
    updateScreenTempText(mReflowModel->getOvenTemp());
    mLocalModel.setOvenTemp(mReflowModel->getOvenTemp());
  }
}

void ReflowProgressScreen::drawScreen() {
  drawAbortButton();
  drawProfileGraph(new SAC305_(), 20);
  refresh();
}

void ReflowProgressScreen::updateScreenTimeRemaining(uint16_t secondsRemaining) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (secondsRemaining != mPreviousSecondsRemaining) {
    char text[5] = "";
    itoa(secondsRemaining, text, 10);
    strcat(text, TIME_END_C_STRING);

    updateScreenText(mCurTimeText, text, TIME_TEXT_SIZE, BLACK, TIME_TEXT_COLOUR, TIME_TEXT_X_POS, TIME_TEXT_Y_POS);

    mPreviousSecondsRemaining = secondsRemaining;
    strcpy(mCurTimeText, text);
  }
}

void ReflowProgressScreen::updateScreenTempText(uint16_t temp) {
  char text[6] = "";
  dtostrf(temp, 3, 0, text);
  strcat(text, TEMP_END_C_STRING);

  updateScreenText(mCurTempText, text, TEMP_TEXT_SIZE, BLACK, TEMP_TEXT_COLOUR, TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);

  strcpy(mCurTempText, text);
}

void ReflowProgressScreen::updateScreenStateText(const char* state) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (state != mPreviousState) {
    updateScreenText(mPreviousState, state, STATE_TEXT_SIZE, BLACK, STATE_TEXT_COLOUR, STATE_TEXT_X_POS, STATE_TEXT_Y_POS);
    mPreviousState = state;
  }
}

void ReflowProgressScreen::drawAbortButton() {
  drawRoundRectangleButton(ABORT_BUTTON_TEXT, ABORT_BUTTON_TEXT_COLOUR, ABORT_BUTTON_FILL_COLOUR, 75, 95);
}

void ReflowProgressScreen::drawProfileGraph(ReflowProfile* profile, uint16_t startTemp) {
  
  // Draw axes
  mScreen->drawFastVLine(GRAPH_ORIGIN_X, GRAPH_ORIGIN_Y-GRAPH_HEIGHT, GRAPH_HEIGHT, GRAPH_AXIS_COLOUR);
  mScreen->drawFastHLine(GRAPH_ORIGIN_X, GRAPH_ORIGIN_Y, GRAPH_WIDTH, GRAPH_AXIS_COLOUR);

  // Determine scale
  float pixelsPerDegree = (float) GRAPH_HEIGHT / (float) profile->getMaxTemp();
  float pixelsPerSecond = (float) GRAPH_WIDTH / (float) profile->getTotalDuration();
  
  // Plot profile
  uint16_t zoneEndTime = 0;
  int16_t startX = GRAPH_ORIGIN_X;
  int16_t startY = GRAPH_ORIGIN_Y - startTemp * pixelsPerDegree;

  int16_t ptxtby = GRAPH_ORIGIN_Y, ptxtbex = GRAPH_ORIGIN_X;

  for (int i=0; i<profile->getNumZones(); i++) {
    ReflowZone zone = profile->getZone(i);
    zoneEndTime += zone.getDurationSeconds();
    uint8_t zoneEndTemp = zone.getTargetTemp();

    int16_t endY = GRAPH_ORIGIN_Y - zoneEndTemp * pixelsPerDegree;
    int16_t endX = GRAPH_ORIGIN_X + zoneEndTime * pixelsPerSecond;

    // Add the ticks
    mScreen->drawFastHLine(GRAPH_ORIGIN_X - TICK_LENGTH, endY , TICK_LENGTH, GRAPH_AXIS_COLOUR);
    mScreen->drawFastVLine(endX, GRAPH_ORIGIN_Y, TICK_LENGTH, GRAPH_AXIS_COLOUR);
    // Add the labels (unless they overlap the previous one)
    printYAxisLabel(endY, &ptxtby, zoneEndTemp);
    printXAxisLabel(endX, &ptxtbex, zoneEndTime);

    // Draw the line
    mScreen->drawLine(startX, startY, endX, endY, GRAPH_AXIS_COLOUR);

    startX = endX;
    startY = endY;
  }

}

void ReflowProgressScreen::printYAxisLabel(int16_t y, int16_t* ptxtby, uint16_t temp) {

  int16_t txtx, txty;
  int16_t txtbx, txtby;
  uint16_t txtw, txth;

  // Create the label
  char label[3];
  sprintf(label, "%03d", temp);

  // Add the labels (unless they overlap the previous one)
  mScreen->setTextSize(GRAPH_LABEL_TEXT_SIZE);
  mScreen->setTextColor(GRAPH_AXIS_COLOUR);
  mScreen->setTextWrap(false);

  // Set the x-cordinate
  txtx = GRAPH_ORIGIN_X - 20;
  // Get the bounds
  txty = y;
  mScreen->getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);
  // Centre vertically
  txty = y - txth/2;
  mScreen->getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);

  if ((int16_t)(txtby + txth) < *ptxtby) {
    mScreen->setCursor(txtx, txty);
    mScreen->print(label);
    *ptxtby = txtby;
  }

}

void ReflowProgressScreen::printXAxisLabel(int16_t x, int16_t* ptxtbex, uint16_t seconds) {

  int16_t txtx, txty;
  int16_t txtbx, txtby;
  uint16_t txtw, txth;

  // Create the label
  char label[4];
  sprintf(label, "%03d", seconds);
  
  // Add the labels (unless they overlap the previous one)
  mScreen->setTextSize(GRAPH_LABEL_TEXT_SIZE);
  mScreen->setTextColor(GRAPH_AXIS_COLOUR);
  mScreen->setTextWrap(false);

  // Set the x-cordinate
  txty = GRAPH_ORIGIN_Y + 4;
  // Get the bounds
  txtx = x;
  mScreen->getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);
  // Centre horizontally
  txtx = x - txtw/2;
  mScreen->getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);

  if (txtbx > *ptxtbex) {
    mScreen->setCursor(txtx, txty);
    mScreen->print(label);
    *ptxtbex = txtbx + txtw;
  }
}

