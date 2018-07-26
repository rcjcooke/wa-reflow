#include "ReflowHeater.hpp"

void ReflowHeater::init() {

}

void ReflowHeater::refresh() {
  if (mReflowModel->getOvenState() == ReflowOvenState::Reflowing) {
    // Find out what the temperature is now
    int16_t currentTemp = mReflowModel->getOvenTemp();
    // Find out what the temperature SHOULD be now
    int16_t targetTemp = mReflowModel->determineTargetProfileTempNow();

    if (targetTemp - currentTemp > HIGHEST_THRESHOLD_TEMP) {
      turnOnHighPowerElement();
      turnOnLowPowerElement();
    } else if (targetTemp - currentTemp > HIGH_THRESHOLD_TEMP) {
      turnOnHighPowerElement();
      turnOffLowPowerElement();
    } else if (targetTemp - currentTemp > LOW_THRESHOLD_TEMP) {
      turnOffHighPowerElement();
      turnOnLowPowerElement();
    }

  } else {
    // Make sure everything is off as we're not currently Reflowing
    turnOffHighPowerElement();
    turnOffLowPowerElement();
  }
}

void ReflowHeater::turnOnHighPowerElement() {
  digitalWrite(mHighPowerHeatingElementPin, HIGH);
}

void ReflowHeater::turnOffHighPowerElement() {
  digitalWrite(mHighPowerHeatingElementPin, LOW);
}

void ReflowHeater::turnOnLowPowerElement() {
  digitalWrite(mLowPowerHeatingElementPin, HIGH);
}

void ReflowHeater::turnOffLowPowerElement() {
  digitalWrite(mLowPowerHeatingElementPin, LOW);
}
