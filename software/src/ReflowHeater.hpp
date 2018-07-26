#ifndef __REFLOWHEATER_H_INCLUDED__
#define __REFLOWHEATER_H_INCLUDED__

#include "ReflowModel.hpp"
#include <stdint.h>

class ReflowHeater {

public:
  ReflowHeater(ReflowModel *reflowModel, uint8_t lpPin, uint8_t hpPin)
      : mReflowModel(reflowModel), mLowPowerHeatingElementPin(lpPin),
        mHighPowerHeatingElementPin(hpPin){};

private:
  ReflowModel *mReflowModel;
  uint8_t mLowPowerHeatingElementPin;
  uint8_t mHighPowerHeatingElementPin;
};

#endif // __REFLOWHEATER_H_INCLUDED__