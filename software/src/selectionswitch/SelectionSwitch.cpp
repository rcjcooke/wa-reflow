#include "SelectionSwitch.hpp"

#include <Arduino.h>
#include "Encoder.hpp"

// NOTE: DEFINED GLOBALLY NOT IN CLASS SO THAT INTERRUPT HAS ACCESS
Encoder* gEncoder;
bool gButtonPressesWaitingToAction = false;

/**************************
 * Interrupt function calls
 **************************/
// NOTE: THESE HAVE TO BE GLOBAL, NOT CLASS METHODS

void userTurn() {
  gEncoder->update();
}

void userSelectButtonPress() {
  gButtonPressesWaitingToAction = true;
}

/**************************
 * Class methods
 **************************/

void SelectionSwitch::init() {
  // Set up the encoder instance
  gEncoder = new Encoder(mSR1230.getChannelAPin(), mSR1230.getChannelBPin());
  // Set up the switch pin - the others are handled in Encoder
  pinMode(mSR1230.getSwitchPin(), INPUT);
  digitalWrite(mSR1230.getSwitchPin(), LOW);
  // Attach the interrupt methods
  attachInterrupt(digitalPinToInterrupt(mSR1230.getChannelAPin()), userTurn, CHANGE);
  attachInterrupt(digitalPinToInterrupt(mSR1230.getSwitchPin()), userSelectButtonPress, RISING);
  // Record the current position for referencing changes
  mEncoderPosition = gEncoder->getPosition();
}

void SelectionSwitch::refresh() {
  // Get it once only incase it changes while this is executing
  long int positionNow = gEncoder->getPosition();

  // Check to see if the user has done anything
  if (positionNow != mEncoderPosition) {
    // User has turned the knob - update the model
    mReflowModel->moveUserSelection(mEncoderPosition - positionNow);
    mEncoderPosition = positionNow;
  }
  if (gButtonPressesWaitingToAction) {
    mReflowModel->toggleOvenState();
    gButtonPressesWaitingToAction = false;
  }
}
