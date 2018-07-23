#include "ReflowModel.hpp"

void ReflowModel::initialiseModel() {
  // Set the initial oven state
  mOvenState = ReflowOvenState::UserSelecting;
}

void ReflowModel::moveUserSelection(long int numberMoved) {
  // Note: While the code below might look clunky, it is safe from interrupts
  if (mSelectedProfileIndex + numberMoved > mNumProfiles) {
    mSelectedProfileIndex = mNumProfiles;
  } else {
    if (mSelectedProfileIndex + numberMoved < 0) {
      mSelectedProfileIndex = 0;
    } else {
      mSelectedProfileIndex += numberMoved;
    }
  }
}

void ReflowModel::toggleOvenState() {
  switch (mOvenState) {
    case ReflowOvenState::UserSelecting:
      mOvenState = ReflowOvenState::Reflowing;
      break;
    case ReflowOvenState::Reflowing:
      mOvenState = ReflowOvenState::UserSelecting;
      break;
  }
}