#include "SelectionSwitch.hpp"

#include <Arduino.h>
#include "Encoder.hpp"

// NOTE: DEFINED GLOBALLY NOT IN CLASS SO THAT INTERRUPT HAS ACCESS
Encoder gEncoder(2, 7);
bool gButtonPressesWaitingToAction = false;

/**************************
 * Interrupt function calls
 **************************/
// NOTE: THESE HAVE TO BE GLOBAL, NOT CLASS METHODS

void userTurn() {
  gEncoder.update();
}

void userSelectButtonPress() {
  gButtonPressesWaitingToAction = true;
}

/**************************
 * Class methods
 **************************/

void SelectionSwitch::refresh() {
  // Get it once only incase it changes while this is executing
  long int positionNow = gEncoder.getPosition();
  // Check to see if the user has done anything
  if (positionNow != mEncoderPosition) {
    // User has turned the knob - update the model
    mReflowModel->moveUserSelection(mEncoderPosition - positionNow);
  }
  if (gButtonPressesWaitingToAction) {
    mReflowModel->toggleOvenState();
  }
}

void SelectionSwitch::init() {
  attachInterrupt(digitalPinToInterrupt(mSR1230.getChannelAPin()), userTurn, CHANGE);
  attachInterrupt(digitalPinToInterrupt(mSR1230.getSwitchPin()), userSelectButtonPress, RISING);
  mEncoderPosition = gEncoder.getPosition();
}