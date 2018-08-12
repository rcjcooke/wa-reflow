#ifndef __REFLOWZONE_H_INCLUDED__
#define __REFLOWZONE_H_INCLUDED__

#include <stdint.h>
#include <WString.h>

enum class ReflowState : uint8_t {
  Preheat,
  Soak,
  Ramp,
  Reflow,
  Cooling,
  Done
};

class ReflowZone {
public:
  ReflowZone(const ReflowState reflowState, const float maxTempGradient,
             const int16_t targetTemp, const uint16_t durationSeconds)
      : mReflowState(reflowState), mMaxTempGradient(maxTempGradient),
        mTargetTemp(targetTemp), mDurationSeconds(durationSeconds) {}

  const ReflowState getReflowState() const {return mReflowState;}
  const float getMaxTempGradient() const {return mMaxTempGradient;}
  const int16_t getTargetTemp() const {return mTargetTemp;}
  const uint16_t getDurationSeconds() const {return mDurationSeconds;}

  String toString();

  static const char* translateReflowState(ReflowState reflowState) {
    switch(reflowState) {
      case ReflowState::Preheat: return "Preheat";
      case ReflowState::Soak: return "Soak";
      case ReflowState::Ramp: return "Ramp";
      case ReflowState::Reflow: return "Reflow";
      case ReflowState::Cooling: return "Cooling";
      case ReflowState::Done: return "Done";
      default: return "Unknown";
    }
  }

private:
  const ReflowState mReflowState;
  const float mMaxTempGradient;
  const int16_t mTargetTemp;
  const uint16_t mDurationSeconds;

};

#endif // __REFLOWZONE_H_INCLUDED__