#include "ReflowZone.hpp"

String ReflowZone::toString() {
  String base = String("ReflowZone[");
  base+="mReflowState=" + String(ReflowZone::translateReflowState(mReflowState)) + ",";
  base+="mMaxTempGradient=" + String(mMaxTempGradient) + ",";
  base+="mTargetTemp=" + String(mTargetTemp) + ",";
  base+="mDurationSeconds=" + String(mDurationSeconds);
  base+="]";
  return base;
}