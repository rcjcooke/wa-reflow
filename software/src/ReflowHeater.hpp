#ifndef __REFLOWHEATER_H_INCLUDED__
#define __REFLOWHEATER_H_INCLUDED__

#include "ReflowModel.hpp"
#include <stdint.h>

class ReflowHeater : public Refreshable {

public:
  ReflowHeater(ReflowModel *reflowModel, uint8_t lpPin, uint8_t hpPin)
      : mReflowModel(reflowModel), mLowPowerHeatingElementPin(lpPin),
        mHighPowerHeatingElementPin(hpPin){};

  // Initialise the heater
  void init();
  // Refresh - make sure the heater is trying to reach the required temperature
  void refresh();

private:

  static constexpr int16_t HIGHEST_THRESHOLD_TEMP = 10;
  static constexpr int16_t HIGH_THRESHOLD_TEMP = 5;
  static constexpr int16_t LOW_THRESHOLD_TEMP = 2;

  void turnOnHighPowerElement();
  void turnOffHighPowerElement();
  void turnOnLowPowerElement();
  void turnOffLowPowerElement();

  ReflowModel *mReflowModel;
  uint8_t mLowPowerHeatingElementPin;
  uint8_t mHighPowerHeatingElementPin;
};

#endif // __REFLOWHEATER_H_INCLUDED__