#include "SelectionSwitch.hpp"

/**************************
 * Class methods
 **************************/

void SelectionSwitch::init() {
  // Attach the interrupt methods
  attachInterrupt(digitalPinToInterrupt(mSR1230.getChannelAPin()), mUserTurnInterruptDispatchFunctionPtr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(mSR1230.getSwitchPin()), mUserSelectButtonPressInterruptDispatchFunctionPtr, RISING);
  // Record the current position for referencing changes
  mEncoderPreviousPosition = mSR1230.getPosition();
}

void SelectionSwitch::userTurnInterrupt() {
  mSR1230.userTurn();
}

void SelectionSwitch::userSelectButtonPressInterrupt() {
  mSR1230.userSelectButtonPress();
}

void SelectionSwitch::refresh() {
  // Get it once only incase it changes while this is executing
  long int positionNow = mSR1230.getPosition();

  // Check to see if the user has done anything
  if (positionNow != mEncoderPreviousPosition) {
    // User has turned the knob - update the model
    mReflowModel->moveUserSelection(mEncoderPreviousPosition - positionNow);
    mEncoderPreviousPosition = positionNow;
  }
  if (mSR1230.isButtonPressesWaitingToAction()) {
    if (mReflowModel->getOvenState() == ReflowOvenState::UserSelecting) {
      mReflowController->startReflow();
    } else {
      mReflowController->abortReflow();
    }
    mSR1230.buttonPressActioned();
  }
}
