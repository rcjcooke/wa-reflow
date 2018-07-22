#include "ReflowTFT.hpp"
#include "SerialDebugger.hpp"

#include "reflowprofile/SAC305.hpp" 

char curTempText[6] = "";
char curTimeText[5] = "";
uint16_t previousSecondsRemaining = 0;
const char* previousState;

ReflowModel localModel = ReflowModel();

void ReflowTFT::init() {
  mScreen.initR(INITR_144GREENTAB);
  mScreen.setRotation(ROTATE_0_DEGREES);
  mScreen.fillScreen(BLACK);
  mScreen.setTextSize(2);
  // Tried fonts to make it look nicer but had real problems with degree symbol
  mScreen.setFont();
}

void ReflowTFT::refresh() {
  // Check the overall state first as this affects the whole screen
  if (mReflowModel->getOvenState() != localModel.getOvenState()) {
    switch(mReflowModel->getOvenState()) {
      case ReflowOvenState::UserSelecting:
        drawReflowSelectScreen();
        localModel.setOvenState(ReflowOvenState::UserSelecting);
        break;
      case ReflowOvenState::Reflowing:
        drawProgressScreen();
        localModel.setOvenState(ReflowOvenState::Reflowing);
        break;
    }
  }
  // Check the temperature
  if (mReflowModel->getOvenTemp() != localModel.getOvenTemp()) {
    // Only bother updating the screen if it's changed - avoids excessive "blinking"
    updateScreenTempText(mReflowModel->getOvenTemp());
    localModel.setOvenText(mReflowModel->getOvenTemp());
  }
}

void ReflowTFT::drawReflowSelectScreen() {
  drawStartButton();
  drawProfileGraph(new SAC305(), 20);
}

void ReflowTFT::drawProgressScreen() {
  drawAbortButton();
  drawProfileGraph(new SAC305(), 20);
}

void ReflowTFT::updateScreenTimeRemaining(uint16_t secondsRemaining) {
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

void ReflowTFT::updateScreenTempText(uint16_t temp) {
  char text[6] = "";
  dtostrf(temp, 3, 0, text);
  strcat(text, TEMP_END_C_STRING);

  updateScreenText(curTempText, text, TEMP_TEXT_SIZE, BLACK, TEMP_TEXT_COLOUR, TEMP_TEXT_X_POS, TEMP_TEXT_Y_POS);

  strcpy(curTempText, text);
}

void ReflowTFT::updateScreenStateText(const char* state) {
  // Only bother updating the screen if it's changed - avoids excessive "blinking"
  if (state != previousState) {
    updateScreenText(previousState, state, STATE_TEXT_SIZE, BLACK, STATE_TEXT_COLOUR, STATE_TEXT_X_POS, STATE_TEXT_Y_POS);
    previousState = state;
  }
}

void ReflowTFT::updateScreenText(const char* prevText, const char* text, uint8_t size, uint16_t bgColour, uint16_t txtColour, int16_t x, int16_t y) {
  mScreen.setTextSize(size);        
  
  // Clear off existing
  mScreen.setTextColor(bgColour);
  mScreen.setCursor(x, y);
  mScreen.print(prevText);

  // Update text
  mScreen.setTextColor(txtColour);    
  mScreen.setCursor(x, y);
  mScreen.print(text);
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
  mScreen.setTextSize(BUTTON_TEXT_SIZE);
  mScreen.getTextBounds(buttonText, txtx, txty, &txtbx, &txtby, &txtw, &txth);

  // fill
  mScreen.fillRoundRect(x, y, txtw+20, txth+20, 4, buttonColour);
  // border
  mScreen.drawRoundRect(x, y, txtw+20, txth+20, 4, BUTTON_BORDER_COLOUR);
  // text
  mScreen.setCursor(txtx, txty);
  mScreen.setTextColor(textColour);
  mScreen.print(buttonText);

}

void ReflowTFT::drawProfileGraph(ReflowProfile* profile, uint16_t startTemp) {
  
  // Draw axes
  mScreen.drawFastVLine(GRAPH_ORIGIN_X, GRAPH_ORIGIN_Y-GRAPH_HEIGHT, GRAPH_HEIGHT, GRAPH_AXIS_COLOUR);
  mScreen.drawFastHLine(GRAPH_ORIGIN_X, GRAPH_ORIGIN_Y, GRAPH_WIDTH, GRAPH_AXIS_COLOUR);

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

    SerialDebugger.updateValue("zoneEndTemp" + String(i), zoneEndTemp);
    SerialDebugger.updateValue("zoneEndTime" + String(i), zoneEndTime);
    
    int16_t endY = GRAPH_ORIGIN_Y - zoneEndTemp * pixelsPerDegree;
    int16_t endX = GRAPH_ORIGIN_X + zoneEndTime * pixelsPerSecond;

    // Add the ticks
    mScreen.drawFastHLine(GRAPH_ORIGIN_X - TICK_LENGTH, endY , TICK_LENGTH, GRAPH_AXIS_COLOUR);
    mScreen.drawFastVLine(endX, GRAPH_ORIGIN_Y, TICK_LENGTH, GRAPH_AXIS_COLOUR);
    // Add the labels (unless they overlap the previous one)
    printYAxisLabel(endY, &ptxtby, zoneEndTemp);
    printXAxisLabel(endX, &ptxtbex, zoneEndTime);

    // Draw the line
    mScreen.drawLine(startX, startY, endX, endY, GRAPH_AXIS_COLOUR);

    startX = endX;
    startY = endY;
  }

}

void ReflowTFT::printYAxisLabel(int16_t y, int16_t* ptxtby, uint16_t temp) {

  int16_t txtx, txty;
  int16_t txtbx, txtby;
  uint16_t txtw, txth;

  // Create the label
  char label[3];
  sprintf(label, "%03d", temp);

  // Add the labels (unless they overlap the previous one)
  mScreen.setTextSize(GRAPH_LABEL_TEXT_SIZE);
  mScreen.setTextColor(GRAPH_AXIS_COLOUR);
  mScreen.setTextWrap(false);

  // Set the x-cordinate
  txtx = GRAPH_ORIGIN_X - 20;
  // Get the bounds
  txty = y;
  mScreen.getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);
  // Centre vertically
  txty = y - txth/2;
  mScreen.getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);

  if ((int16_t)(txtby + txth) < *ptxtby) {
    mScreen.setCursor(txtx, txty);
    mScreen.print(label);
    *ptxtby = txtby;
  }

}

void ReflowTFT::printXAxisLabel(int16_t x, int16_t* ptxtbex, uint16_t seconds) {

  int16_t txtx, txty;
  int16_t txtbx, txtby;
  uint16_t txtw, txth;

  // Create the label
  char label[3];
  sprintf(label, "%03d", seconds);
  
  // Add the labels (unless they overlap the previous one)
  mScreen.setTextSize(GRAPH_LABEL_TEXT_SIZE);
  mScreen.setTextColor(GRAPH_AXIS_COLOUR);
  mScreen.setTextWrap(false);

  // Set the x-cordinate
  txty = GRAPH_ORIGIN_Y + 4;
  // Get the bounds
  txtx = x;
  mScreen.getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);
  // Centre horizontally
  txtx = x - txtw/2;
  mScreen.getTextBounds(label, txtx, txty, &txtbx, &txtby, &txtw, &txth);

  if (txtbx > *ptxtbex) {
    mScreen.setCursor(txtx, txty);
    mScreen.print(label);
    *ptxtbex = txtbx + txtw;
  }
}