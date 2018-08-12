#include "ReflowController.hpp"
#include "SerialDebugger/SerialDebugger.hpp"

void ReflowController::init() {
  // Set the initial oven state
  mReflowModel->setOvenState(ReflowOvenState::UserSelecting);
}

void ReflowController::refresh() {
  if (mReflowModel->getOvenState() == ReflowOvenState::Reflowing) {
    unsigned long durationSinceStartMillis = millis() - mReflowModel->getRunningReflowStartTimeMillis();

    // Update remaining time
    unsigned long totalDurationSeconds = mReflowModel->getRunningReflowProfile()->getTotalDurationSeconds();
    unsigned long totalDurationMillis = totalDurationSeconds * 1000;
    unsigned long remainingTimeMillis = totalDurationMillis - durationSinceStartMillis;
    float remainingTimeSeconds = (float) remainingTimeMillis / (float) 1000.0;
    mReflowModel->setRunningTimeRemainingSeconds(remainingTimeSeconds);
    
    // Update current zone
    ReflowZone* zone = mReflowModel->getRunningReflowZone();
    uint16_t zoneStart = zone->getStartTimeSeconds();
    uint16_t zoneDuration = zone->getDurationSeconds();
    
    if (durationSinceStartMillis > (unsigned long) (zoneStart + zoneDuration) * 1000) {
      if (zone->getNextZone()) {
        mReflowModel->setRunningReflowZone(zone->getNextZone());
      } else {
        // Reached the end of the profile
        mReflowModel->toggleOvenState();
      }
    }

    uint16_t timeInZone = durationSinceStartMillis / 1000 - zone->getStartTimeSeconds();
    float runningTargetTemp = (float) (timeInZone / zone->getDurationSeconds()) * (zone->getTargetTemp() - zone->getStartTemp()) + zone->getStartTemp();

    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh: startTemp", zone->getStartTemp());
    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh: targetTemp", zone->getTargetTemp());
    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh: timeInZone", timeInZone);
    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh: durationSeconds", zone->getDurationSeconds());
    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh: runningTargetTemp", runningTargetTemp);
    
    // Update the current target temperature
    mReflowModel->setRunningTargetTemperature(runningTargetTemp);

  }
}

void ReflowController::startReflow() {
  if (mReflowModel->getOvenState() == ReflowOvenState::UserSelecting) {
    mReflowModel->setRunningReflowProfile(mReflowModel->getReflowProfile(mReflowModel->getSelectedProfileIndex()));
    mReflowModel->setRunningReflowZone(mReflowModel->getRunningReflowProfile()->getZone(0));
    mReflowModel->setRunningReflowStartTimeMillis(millis());
    // Set the start temperature of the first zone in the active profile - used for determining temperature profile
    mReflowModel->getRunningReflowZone()->setStartTemp(mReflowModel->getOvenTemp());

    if (DEBUG) SerialDebugger.updateValue("Running Profile", mReflowModel->getRunningReflowProfile()->getName());
    if (DEBUG) SerialDebugger.updateValue("Running Reflow Zone", ReflowZone::translateReflowState(mReflowModel->getRunningReflowZone()->getReflowState()));
    if (DEBUG) SerialDebugger.updateValue("Running Reflow Start Time", mReflowModel->getRunningReflowStartTimeMillis());
    if (DEBUG) SerialDebugger.updateValue("Running Reflow Start Temp", mReflowModel->getRunningReflowZone()->getStartTemp());

    mReflowModel->toggleOvenState();
  }
}

void ReflowController::abortReflow() {
  if (mReflowModel->getOvenState() == ReflowOvenState::Reflowing) {
    // TODO
  }
}
