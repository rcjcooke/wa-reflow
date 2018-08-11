#include "SR1230.hpp"

void SR1230::init() {
  // Set up the switch pin - the others are handled in Encoder
  pinMode(mSwitchPin, INPUT);
  digitalWrite(mSwitchPin, LOW);
}

void SR1230::userTurn() {
  mEncoder.update();
}

void SR1230::userSelectButtonPress() {
  mButtonPressesWaitingToAction = true;
}

void SR1230::buttonPressActioned() {
  mButtonPressesWaitingToAction = false;
}
