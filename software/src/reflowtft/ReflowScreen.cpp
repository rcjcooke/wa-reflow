#include "ReflowScreen.hpp"

constexpr char ReflowScreen::TEMP_END_C_STRING[];

void ReflowScreen::updateScreenText(const char* prevText, const char* text, uint8_t size, uint16_t bgColour, uint16_t txtColour, int16_t x, int16_t y) {
  mScreen->setTextSize(size);        
  
  // Clear off existing
  mScreen->setTextColor(bgColour);
  mScreen->setCursor(x, y);
  mScreen->print(prevText);

  // Update text
  mScreen->setTextColor(txtColour);    
  mScreen->setCursor(x, y);
  mScreen->print(text);
}

void ReflowScreen::drawRoundRectangleButton(char* buttonText, uint16_t textColour, uint16_t buttonColour, int16_t x, int16_t y ) {
  
  // Work out how big the text is
  int16_t txtx = x+10, txty = y+10;
  int16_t txtbx, txtby;
  uint16_t txtw, txth;
  mScreen->setTextSize(BUTTON_TEXT_SIZE);
  mScreen->getTextBounds(buttonText, txtx, txty, &txtbx, &txtby, &txtw, &txth);

  // fill
  mScreen->fillRoundRect(x, y, txtw+20, txth+20, 4, buttonColour);
  // border
  mScreen->drawRoundRect(x, y, txtw+20, txth+20, 4, BUTTON_BORDER_COLOUR);
  // text
  mScreen->setCursor(txtx, txty);
  mScreen->setTextColor(textColour);
  mScreen->print(buttonText);

}