#ifndef __REFLOWPROFILE_H_INCLUDED__
#define __REFLOWPROFILE_H_INCLUDED__

#include <Arduino.h>
#include "../SerialDebugger/SerialDebugger.hpp"

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
  uint16_t getStartTimeSeconds() const {return mStartTimeSeconds;}
  int16_t getStartTemp() const {return mStartTemp;}
  float getTargetTempGradient() const {return mTargetTempGradient;}
  ReflowZone* getNextZone() const {return mNextZone;}

  void setStartTimeSeconds(uint16_t startTime) {mStartTimeSeconds = startTime;}
  void setStartTemp(int16_t startTemp) {mStartTemp = startTemp;}
  void setTargetTempGradient(float targetTempGradient) {mTargetTempGradient = targetTempGradient;}
  void setNextZone(ReflowZone* nextZone) {mNextZone = nextZone;}

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
  uint16_t mStartTimeSeconds = 0;
  int16_t mStartTemp = 0;
  float mTargetTempGradient = 0; // degrees per second
  ReflowZone* mNextZone = NULL;

};

class ReflowProfile {

public:

  ReflowProfile(char* name, uint8_t numZones, ReflowZone zones[]) : mName(name), mNumZones(numZones), mZones(zones) {
    populateCalcs();
  }

  // Annoyingly this has to be non-const to avoid Adafruit_gfx library getTextBounds issue
  char* getName() const {return mName;}
  const uint8_t getNumZones() const {return mNumZones;}
  ReflowZone* getZone(int zone) const {return &mZones[zone];}
  uint16_t getTotalDurationSeconds() const {
    if (DEBUG) SerialDebugger.updateValue("mTotalDurationSeconds (getter)", mTotalDurationSeconds);
    if (DEBUG) SerialDebugger.updateValue("getter this", (void *) this);
    return mTotalDurationSeconds;
  }
  uint16_t getMaxTemp() const {return mMaxTemp;}  

  int getZoneIndexByDurationSinceProfileStart(unsigned long durationSinceStartMillis) {
    unsigned long zoneStartMillis = 0;
    for (int i=0; i<mNumZones; i++) {
      ReflowZone z = mZones[i];
      unsigned long zoneDurationMillis = z.getDurationSeconds() * 1000;
      if (zoneDurationMillis + zoneStartMillis >= durationSinceStartMillis) {
        return i;
      }
      zoneStartMillis += zoneDurationMillis;
    }
    return mNumZones;
  }

protected:
  // Annoyingly this has to be non-const to avoid Adafruit_gfx library getTextBounds issue
  char* mName;
  const uint8_t mNumZones;
  ReflowZone* mZones;
  uint16_t mTotalDurationSeconds;
  int16_t mMaxTemp;

private:

  void populateCalcs() {
    uint16_t zoneStart = 0;
    int16_t zoneStartTemp = 0;
    for (int i=0; i<mNumZones; i++) {
      ReflowZone z = mZones[i];
      uint16_t zoneDuration = z.getDurationSeconds();
      if (DEBUG) SerialDebugger.updateValue("zoneDuration", zoneDuration);
      mTotalDurationSeconds += zoneDuration;
      if (DEBUG) SerialDebugger.updateValue("mTotalDurationSeconds", mTotalDurationSeconds);
      if (DEBUG) SerialDebugger.updateValue("populateCalcs this", (void *) this);

      mMaxTemp = max(mMaxTemp, z.getTargetTemp());
      z.setStartTimeSeconds(zoneStart);
      z.setStartTemp(zoneStartTemp);
      z.setTargetTempGradient((z.getTargetTemp() - zoneStartTemp)/zoneDuration);
      if (i+1 < mNumZones) z.setNextZone(&mZones[i]);
      zoneStart += zoneDuration;
      zoneStartTemp = z.getTargetTemp();
    }
  }
};

#endif // __REFLOWPROFILE_H_INCLUDED__