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
    
    // Update the current target temperature
    ReflowZone* zone = mReflowModel->getRunningReflowZone();
    uint16_t zoneStart = mReflowModel->getRunningReflowZoneStartTimeSeconds();
    uint16_t zoneDuration = zone->getDurationSeconds();
    uint16_t zoneStartTemp = mReflowModel->getRunningReflowZoneStartTemp();
    uint16_t zoneTargetTemp = zone->getTargetTemp();

    uint16_t timeInZone = durationSinceStartMillis / 1000 - zoneStart;
    float zoneTimeRatio = (float) timeInZone / (float) zoneDuration;
    float runningTargetTemp = zoneTimeRatio * (zoneTargetTemp - zoneStartTemp) + zoneStartTemp;
    mReflowModel->setRunningTargetTemperature(runningTargetTemp);

    // Update current zone
    if (durationSinceStartMillis > (unsigned long) (zoneStart + zoneDuration) * 1000) {
      int zoneIndex = mReflowModel->getRunningReflowZoneIndex();
      if (zoneIndex+1 < mReflowModel->getRunningReflowProfile()->getNumZones()) {
        mReflowModel->setRunningReflowZoneStartTimeSeconds(mReflowModel->getRunningReflowZoneStartTimeSeconds() + zoneDuration);
        mReflowModel->setRunningReflowZoneStartTemp(zoneTargetTemp);
        mReflowModel->setRunningReflowZoneIndex(zoneIndex+1);
      } else {
        // Reached the end of the profile
        mReflowModel->toggleOvenState();
      }
    }

    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh profile", mReflowModel->getRunningReflowProfile()->toString());
    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh zone", ReflowZone::translateReflowState(zone->getReflowState()));
    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh numZones", mReflowModel->getRunningReflowProfile()->getNumZones());
    if (DEBUG) SerialDebugger.updateValue("ReflowController::refresh runningTargetTemp", runningTargetTemp);

  }
}

void ReflowController::startReflow() {
  if (mReflowModel->getOvenState() == ReflowOvenState::UserSelecting) {
    mReflowModel->setRunningReflowProfile(mReflowModel->getReflowProfile(mReflowModel->getSelectedProfileIndex()));
    mReflowModel->setRunningReflowZoneIndex(0);
    mReflowModel->setRunningReflowStartTimeMillis(millis());
    mReflowModel->setRunningReflowZoneStartTemp(mReflowModel->getOvenTemp());
    mReflowModel->setRunningReflowZoneStartTimeSeconds(0);

    if (DEBUG) SerialDebugger.updateValue("Running Profile", mReflowModel->getRunningReflowProfile()->getName());
    if (DEBUG) SerialDebugger.updateValue("Running Reflow Zone", ReflowZone::translateReflowState(mReflowModel->getRunningReflowZone()->getReflowState()));
    if (DEBUG) SerialDebugger.updateValue("Running Reflow Start Time", mReflowModel->getRunningReflowStartTimeMillis());

    mReflowModel->toggleOvenState();
  }
}

void ReflowController::abortReflow() {
  if (mReflowModel->getOvenState() == ReflowOvenState::Reflowing) {
    // TODO
  }
}
