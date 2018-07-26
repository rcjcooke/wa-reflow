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

  void setStartTimeSeconds(uint16_t startTime) {mStartTimeSeconds = startTime;}
  void setStartTemp(int16_t startTemp) {mStartTemp = startTemp;}
  void setTargetTempGradient(float targetTempGradient) {mTargetTempGradient = targetTempGradient;}

private:
  const ReflowState mReflowState;
  const float mMaxTempGradient;
  const int16_t mTargetTemp;
  const uint16_t mDurationSeconds;
  uint16_t mStartTimeSeconds = 0;
  int16_t mStartTemp = 0;
  float mTargetTempGradient = 0; // degrees per second

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
  const uint16_t getTotalDuration() const {return mTotalDuration;}
  const uint16_t getMaxTemp() const {return mMaxTemp;}  

  int getZoneIndexByDurationSinceProfileStart(long durationSinceStartMillis) {
    long zoneStartMillis = 0;
    for (int i=0; i<mNumZones; i++) {
      ReflowZone z = mZones[i];
      long zoneDurationMillis = z.getDurationSeconds() * 1000;
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
  uint16_t mTotalDuration = 0;
  int16_t mMaxTemp = 0;

private:

  void populateCalcs() {
    uint16_t zoneStart = 0;
    int16_t zoneStartTemp = 0;
    for (int i=0; i<mNumZones; i++) {
      ReflowZone z = mZones[i];
      uint16_t zoneDuration = z.getDurationSeconds();
      mTotalDuration += zoneDuration;
      mMaxTemp = max(mMaxTemp, z.getTargetTemp());
      z.setStartTimeSeconds(zoneStart);
      z.setStartTemp(zoneStartTemp);
      z.setTargetTempGradient((z.getTargetTemp() - zoneStartTemp)/zoneDuration);
      zoneStart += zoneDuration;
      zoneStartTemp = z.getTargetTemp();
    }
  }
};

#endif // __REFLOWPROFILE_H_INCLUDED__