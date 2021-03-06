#ifndef __REFLOWMODEL_H_INCLUDED__
#define __REFLOWMODEL_H_INCLUDED__

#include "reflowprofile/ReflowProfile.hpp"
#include "reflowprofile/SAC305.hpp"
#include "SerialDebugger/SerialDebugger.hpp"
    
enum class ReflowOvenState : uint8_t {
  UserSelecting,
  Reflowing
};

class ReflowModel {

public:

  ReflowModel() {}

  // Get the current overall state of the oven
  ReflowOvenState getOvenState() const {return mOvenState;}
  // Get the oven temperature
  int16_t getOvenTemp() const {return mOvenTemp;}
  // Get the number of profiles this oven knows about
  const uint8_t getNumProfiles() const {return mNumProfiles;}
  // Get a specific reflow profile that this oven supports
  ReflowProfile* getReflowProfile(int profileIndex) const {return mReflowProfiles[profileIndex];}
  // Get the user's currently selected profile
  uint8_t getSelectedProfileIndex() const {return mSelectedProfileIndex;}
  // Get the time that the last reflow starting running
  unsigned long getRunningReflowStartTimeMillis() const {return mRunningReflowStartTimeMillis;}
  // Get the user selected reflow profile
  ReflowProfile* getRunningReflowProfile() const {return mRunningReflowProfile;}
  // Gets the index of the profile zone that's being executed
  int getRunningReflowZoneIndex() {return mRunningReflowZoneIdx;}
  // Get the reflow zone currently being executed
  ReflowZone* getRunningReflowZone() const {return mRunningReflowProfile->getZone(mRunningReflowZoneIdx);}
  // Returns the number of seconds left in the current profile
  uint16_t getRunningTimeRemainingSeconds() const {return mRunningTimeRemainingSeconds;}
  // Gets the start time in seconds from the beginning of the profile of the current reflow zone
  uint16_t getRunningReflowZoneStartTimeSeconds() const {return mRunningReflowZoneStartTimeSeconds;}
  // The starting temperature of the active reflow zone
  int16_t getRunningReflowZoneStartTemp() const {return mRunningReflowZoneStartTemp;}
  // The temperature target now
  int16_t getRunningTargetTemperature() const {return mRunningTargetTemperature;}

  // Set the current overall state of the oven
  void setOvenState(ReflowOvenState newState) {mOvenState = newState;}
  // Switches the oven to the other state 
  void toggleOvenState();
  // Set the oven temperature
  void setOvenTemp(int16_t temp) {mOvenTemp = temp;}
  // Update the user's selection - sign indicates direction
  void moveUserSelection(long int numberMoved);
  // Sets the start time for the current reflow
  void setRunningReflowStartTimeMillis(unsigned long startTime) {mRunningReflowStartTimeMillis = startTime;}
  // Sets the profile that's being executed
  void setRunningReflowProfile(ReflowProfile* reflowProfile) {mRunningReflowProfile = reflowProfile;}
  // Sets the profile zone that's being executed
  void setRunningReflowZoneIndex(int index) {mRunningReflowZoneIdx = index;}
  // Set the number of seconds left in the current profile
  void setRunningTimeRemainingSeconds(uint16_t timeRemaining) {mRunningTimeRemainingSeconds = timeRemaining;}
  // Sets the start time in seconds from the beginning of the profile of the current reflow zone
  void setRunningReflowZoneStartTimeSeconds(uint16_t startTime) {mRunningReflowZoneStartTimeSeconds = startTime;}
  // The starting temperature of the active reflow zone
  void setRunningReflowZoneStartTemp(int16_t startTemp) {mRunningReflowZoneStartTemp = startTemp;}
  // The temperature target now
  void setRunningTargetTemperature(int16_t tempTarget) {mRunningTargetTemperature = tempTarget;}
   
  // Helper method: Translates an oven state into a human readable string
  static const char* translateOvenState(ReflowOvenState ovenState) {
    switch (ovenState) {
      case ReflowOvenState::UserSelecting:
        return USER_SELECTING_STRING;
      case ReflowOvenState::Reflowing:
        return REFLOWING_STRING;
    }
    return "Undefined";
  }

private:

  // Translation constants
  static constexpr const char* USER_SELECTING_STRING = "UserSelecting";
  static constexpr const char* REFLOWING_STRING = "Reflowing";

  // The number of reflow profiles in memory
  static constexpr uint8_t mNumProfiles = 1;
  // The list of reflow profiles
  ReflowProfile* mReflowProfiles[mNumProfiles] = {&SAC305};

  // The current state of the oven
  ReflowOvenState mOvenState;
  // The oven temperature
  int16_t mOvenTemp;
  // The user's selected profile index
  uint8_t mSelectedProfileIndex = 0;

  // The system start time of the current reflow
  unsigned long mRunningReflowStartTimeMillis = 0;
  // The reflow profile we're executing
  ReflowProfile* mRunningReflowProfile = NULL;
  // The index of the zone of the profile we're currently in
  int mRunningReflowZoneIdx = 0;
  // The target temperature at the current time
  int16_t mRunningTargetTemperature = 0;
  // The starting temperature of the active reflow zone
  int16_t mRunningReflowZoneStartTemp = 0;
  // Time remaining in the current profile
  uint16_t mRunningTimeRemainingSeconds = 0;
  // The start time in seconds from the beginning of the profile of the current reflow zone
  uint16_t mRunningReflowZoneStartTimeSeconds = 0;

};

#endif // __REFLOWMODEL_H_INCLUDED__