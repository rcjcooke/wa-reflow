#include "ReflowProfile.hpp"

int ReflowProfile::getZoneIndexByDurationSinceProfileStart(unsigned long durationSinceStartMillis) {
  unsigned long zoneStartMillis = 0;
  for (int i=0; i<mNumZones; i++) {
    ReflowZone z = mZonesArray[i];
    unsigned long zoneDurationMillis = z.getDurationSeconds() * 1000;
    if (zoneDurationMillis + zoneStartMillis >= durationSinceStartMillis) {
      return i;
    }
    zoneStartMillis += zoneDurationMillis;
  }
  return mNumZones;
}

void ReflowProfile::populateCalcs() {
  for (int i=0; i<mNumZones; i++) {
    ReflowZone z = mZonesArray[i];
    uint16_t zoneDuration = z.getDurationSeconds();
    mTotalDurationSeconds += zoneDuration;
    mMaxTemp = max(mMaxTemp, z.getTargetTemp());
  }
}

String ReflowProfile::toString() {
  String base = String("ReflowProfile[");
  base+="mName=" + String(mName) + ",";
  base+="mNumZones=" + String(mNumZones) + ",";
  base+="mZonesArray=[";
  for (int i=0; i<mNumZones; i++) {
    if (i!=0) base+=",";
    base+=mZonesArray[i].toString();
  }
  base+="],";
  base+="mTotalDurationSeconds=" + String(mTotalDurationSeconds) + ",";
  base+="mMaxTemp=" + String(mMaxTemp);
  base+="]";
  return base;
}