#ifndef __REFLOWPROFILE_H_INCLUDED__
#define __REFLOWPROFILE_H_INCLUDED__

#include <Arduino.h>

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
  ReflowZone(const ReflowState reflowState, const float maxTempGradient,
             const uint16_t targetTemp, const uint16_t durationSeconds)
      : mReflowState(reflowState), mMaxTempGradient(maxTempGradient),
        mTargetTemp(targetTemp), mDurationSeconds(durationSeconds) {}

  const ReflowState getReflowState() const {return mReflowState;}
  const float getMaxTempGradient() const {return mMaxTempGradient;}
  const uint16_t getTargetTemp() const {return mTargetTemp;}
  const uint16_t getDurationSeconds() const {return mDurationSeconds;}

private:
  const ReflowState mReflowState;
  const float mMaxTempGradient;
  const uint16_t mTargetTemp;
  const uint16_t mDurationSeconds;
};

class ReflowProfile {

public:

  ReflowProfile(char* name, uint8_t numZones, const ReflowZone zones[]) : mName(name), mNumZones(numZones), mZones(zones) {}

  // Annoyingly this has to be non-const to avoid Adafruit_gfx library getTextBounds issue
  char* getName() const {return mName;}
  const uint8_t getNumZones() const {return mNumZones;}
  const ReflowZone getZone(int zone) const {return mZones[zone];}
  
  uint16_t getTotalDuration() {
    if (mTotalDuration != 0) return mTotalDuration;
    populateCalcs();
    return mTotalDuration;
  }

  uint16_t getMaxTemp() {
    if (mMaxTemp != 0) return mMaxTemp;
    populateCalcs();
    return mMaxTemp;
  }  

protected:
  // Annoyingly this has to be non-const to avoid Adafruit_gfx library getTextBounds issue
  char* mName;
  const uint8_t mNumZones;
  const ReflowZone* mZones;
  uint16_t mTotalDuration = 0;
  uint16_t mMaxTemp = 0;

private:

  void populateCalcs() {
    for (int i=0; i<mNumZones; i++) {
      mTotalDuration+=mZones[i].getDurationSeconds();
      mMaxTemp = max(mMaxTemp, mZones[i].getTargetTemp() );
    }
  }
};

#endif // __REFLOWPROFILE_H_INCLUDED__