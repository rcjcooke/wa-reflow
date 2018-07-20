#ifndef __REFLOWPROFILE_H_INCLUDED__
#define __REFLOWPROFILE_H_INCLUDED__

#include <stdint.h>

enum class ReflowState : uint8_t {
  Preheat,
  Soak,
  Ramp,
  Reflow,
  Solidifying,
  Cooling
};

class ReflowZone {
public:
  ReflowZone(const ReflowState reflowState, const double maxTempGradient,
             const uint8_t targetTemp, const long durationMilliseconds)
      : mReflowState(reflowState), mMaxTempGradient(maxTempGradient),
        mTargetTemp(targetTemp), mDurationMilliseconds(durationMilliseconds) {}

  const ReflowState getReflowState() const {return mReflowState;}
  const double getMaxTempGradient() const {return mMaxTempGradient;}
  const uint8_t getTargetTemp() const {return mTargetTemp;}
  const long getDurationMilliseconds() const {return mDurationMilliseconds;}

private:
  const ReflowState mReflowState;
  const double mMaxTempGradient;
  const uint8_t mTargetTemp;
  const long mDurationMilliseconds;
};

class ReflowProfile {

public:

  ReflowProfile(const char* name, uint8_t numZones, const ReflowZone zones[]) : mName(name), mZones(zones), mNumZones(numZones) {}

  const char* getName() const {return mName;}
  const uint8_t getNumZones() const {return mNumZones;}
  const ReflowZone getZone(int zone) const {return mZones[zone];}
  
  long getTotalDuration() {
    if (mTotalDuration != 0) return mTotalDuration;
    for (int i=0; i<mNumZones; i++) {
      mTotalDuration+=mZones[i].getDurationMilliseconds();
    }
    return mTotalDuration;
  }

protected:
  const char* mName;
  const uint8_t mNumZones;
  const ReflowZone* mZones;
  long mTotalDuration = 0;

};

#endif // __REFLOWPROFILE_H_INCLUDED__