#include <Arduino.h>

#include "SR1230.hpp"
#include "Encoder.hpp"

Encoder mEncoder(2, 7);
unsigned long nextTime;
unsigned int mButtonPressesWaitingToAction;

/**************************
 * Interrupt function calls
 **************************/
void userTurn() {
  mEncoder.update();
}

void userSelectButtonPress() {
  mButtonPressesWaitingToAction++;
}

void SR1230::init() {
  attachInterrupt(digitalPinToInterrupt(mChannelAPin), userTurn, CHANGE);
  attachInterrupt(digitalPinToInterrupt(mSwitchPin), userSelectButtonPress, RISING);
}