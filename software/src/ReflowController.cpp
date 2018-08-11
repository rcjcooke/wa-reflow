#include "ReflowController.hpp"
#include "SerialDebugger/SerialDebugger.hpp"

void ReflowController::init() {
  // Set the initial oven state
  mReflowModel->setOvenState(ReflowOvenState::UserSelecting);
}

void ReflowController::refresh() {
  if (mReflowModel->getOvenState() == ReflowOvenState::Reflowing) {
    unsigned long durationSinceStartMillis = millis() - mReflowModel->getRunningReflowStartTimeMillis();
    if (DEBUG) SerialDebugger.updateValue("durationSinceStartMillis", durationSinceStartMillis);
    if (DEBUG) SerialDebugger.updateValue("Running Profile (refresh)", mReflowModel->getRunningReflowProfile()->getName());
    if (DEBUG) SerialDebugger.updateValue("Running Reflow Zone (refresh)", ReflowZone::translateReflowState(mReflowModel->getRunningReflowZone()->getReflowState()));

    // Update remaining time
    unsigned long totalDurationSeconds = mReflowModel->getRunningReflowProfile()->getTotalDurationSeconds();
    unsigned long totalDurationMillis = totalDurationSeconds * 1000;
    unsigned long remainingTimeMillis = totalDurationMillis - durationSinceStartMillis;
    float remainingTimeSeconds = (float) remainingTimeMillis / (float) 1000.0;
    if (DEBUG) SerialDebugger.updateValue("totalDurationSeconds", totalDurationSeconds);
    if (DEBUG) SerialDebugger.updateValue("totalDurationMillis", totalDurationMillis);
    if (DEBUG) SerialDebugger.updateValue("remainingTimeMillis", remainingTimeMillis);
    if (DEBUG) SerialDebugger.updateValue("remainingTimeSeconds", remainingTimeSeconds);
    mReflowModel->setRunningTimeRemainingSeconds(remainingTimeSeconds);
    
    // Update current zone
    ReflowZone* zone = mReflowModel->getRunningReflowZone();
    uint16_t zoneStart = zone->getStartTimeSeconds();
    uint16_t zoneDuration = zone->getDurationSeconds();
    if ((zoneStart + zoneDuration) * 1000 > durationSinceStartMillis) {
      if (zone->getNextZone()) {
        mReflowModel->setRunningReflowZone(zone->getNextZone());
      } else {
        // Reached the end of the profile
        mReflowModel->toggleOvenState();
      }
    }

    // Update the current target temperature
    mReflowModel->setRunningTargetTemperature(((float) (durationSinceStartMillis / 1000) * zone->getTargetTempGradient()));

  }
}

void ReflowController::startReflow() {
  if (mReflowModel->getOvenState() == ReflowOvenState::UserSelecting) {
    mReflowModel->setRunningReflowStartTimeMillis(millis());
    mReflowModel->setRunningReflowProfile(mReflowModel->getReflowProfile(mReflowModel->getSelectedProfileIndex()));
    mReflowModel->setRunningReflowZone(mReflowModel->getRunningReflowProfile()->getZone(0));
    // Set the start temperature of the first zone in the active profile - used for determining temperature profile
    mReflowModel->getRunningReflowZone()->setStartTemp(mReflowModel->getOvenTemp());

    if (DEBUG) SerialDebugger.updateValue("Running Profile", mReflowModel->getRunningReflowProfile()->getName());
    if (DEBUG) SerialDebugger.updateValue("Running Reflow Zone", ReflowZone::translateReflowState(mReflowModel->getRunningReflowZone()->getReflowState()));

    mReflowModel->toggleOvenState();
  }
}

void ReflowController::abortReflow() {
  if (mReflowModel->getOvenState() == ReflowOvenState::Reflowing) {
    // TODO
  }
}
