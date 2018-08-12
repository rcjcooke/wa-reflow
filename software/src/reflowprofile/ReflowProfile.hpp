#ifndef __REFLOWPROFILE_H_INCLUDED__
#define __REFLOWPROFILE_H_INCLUDED__

#include <Arduino.h>
#include "ReflowZone.hpp"

class ReflowProfile {

public:

  ReflowProfile(char* name, const uint8_t numZones, ReflowZone* zonesArray) : mName(name), mNumZones(numZones), mZonesArray(zonesArray) {
    populateCalcs();
  }

  // Annoyingly this has to be non-const to avoid Adafruit_gfx library getTextBounds issue
  char* getName() const {return mName;}
  const uint8_t getNumZones() const {return mNumZones;}
  ReflowZone* getZone(int zone) const {return &(mZonesArray[zone]);}
  uint16_t getTotalDurationSeconds() const {return mTotalDurationSeconds;}
  uint16_t getMaxTemp() const {return mMaxTemp;}  

  int getZoneIndexByDurationSinceProfileStart(unsigned long durationSinceStartMillis);

  String toString();
  
protected:
  // Annoyingly this has to be non-const to avoid Adafruit_gfx library getTextBounds issue
  char* mName;
  const uint8_t mNumZones;
  ReflowZone* mZonesArray;
  uint16_t mTotalDurationSeconds;
  int16_t mMaxTemp;

private:

  void populateCalcs();
};

#endif // __REFLOWPROFILE_H_INCLUDED__