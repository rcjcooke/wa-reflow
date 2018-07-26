#include "ReflowModel.hpp"

void ReflowModel::initialiseModel() {
  // Set the initial oven state
  mOvenState = ReflowOvenState::UserSelecting;
}

void ReflowModel::moveUserSelection(long int numberMoved) {
  if (mOvenState == ReflowOvenState::UserSelecting) {
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
}

void ReflowModel::toggleOvenState() {
  switch (mOvenState) {
    case ReflowOvenState::UserSelecting:
      mOvenState = ReflowOvenState::Reflowing;
      break;
    case ReflowOvenState::Reflowing:
      mOvenState = ReflowOvenState::UserSelecting;
      mReflowStartTimeMillis = millis();
      // Set the start temperature of the first zone in the active profile
      mReflowProfiles[mSelectedProfileIndex]->getZone(0)->setStartTemp(mOvenTemp);
      break;
  }
}

int16_t ReflowModel::determineTargetProfileTempNow() {
  if (mOvenState == ReflowOvenState::Reflowing) {
    long durationSinceStart = millis() - mReflowStartTimeMillis;
    int zoneIndex = mReflowProfiles[mSelectedProfileIndex]->getZoneIndexByDurationSinceProfileStart(durationSinceStart);
    ReflowZone* reflowZone = mReflowProfiles[mSelectedProfileIndex]->getZone(zoneIndex);
    return durationSinceStart / 1000 * reflowZone->getTargetTempGradient();
  } else {
    return 0;
  }
}