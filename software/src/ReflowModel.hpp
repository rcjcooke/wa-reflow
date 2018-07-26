#ifndef __REFLOWMODEL_H_INCLUDED__
#define __REFLOWMODEL_H_INCLUDED__

#include "reflowprofile/ReflowProfile.hpp"
#include "reflowprofile/SAC305.hpp"

enum class ReflowOvenState : uint8_t {
  UserSelecting,
  Reflowing
};

class ReflowModel {

public:

  // Initialise the oven in the user selection state
  ReflowModel() {
    initialiseModel();
  };

  // Populates the model
  void initialiseModel();

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
  long getReflowStartTimeMillis() const {return mReflowStartTimeMillis;}

  // Determines what the temperature should be at this point in an active profile
  int16_t determineTargetProfileTempNow();
  // Determine how long is left to go on the current profile reflow
  int16_t determineTimeRemaining();

  // Set the current overall state of the oven
  void setOvenState(ReflowOvenState newState) {mOvenState = newState;}
  // Switches the oven to the other state 
  void toggleOvenState();
  // Set the oven temperature
  void setOvenTemp(int16_t temp) {mOvenTemp = temp;}
  // Update the user's selection - sign indicates direction
  void moveUserSelection(long int numberMoved);

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

  // The current state of the oven
  ReflowOvenState mOvenState;

  // The oven temperature
  int16_t mOvenTemp;
  // The list of reflow profiles
  // ReflowProfile* mReflowProfiles[10] = {(SAC305_*) new SAC305_(), new ReflowProfile("Test2", 0, nullptr), new ReflowProfile("Test3", 0, nullptr), new ReflowProfile("Test4", 0, nullptr), new ReflowProfile("Test5", 0, nullptr), new ReflowProfile("Test6", 0, nullptr), new ReflowProfile("Test7", 0, nullptr), new ReflowProfile("Test8", 0, nullptr), new ReflowProfile("Test9", 0, nullptr), new ReflowProfile("Test10", 0, nullptr)};
  ReflowProfile* mReflowProfiles[10] = {(SAC305_*) new SAC305_()};
  // The number of reflow profiles in memory
  const uint8_t mNumProfiles = 10;
  // The user's selected profile index
  uint8_t mSelectedProfileIndex = 0;
  // The system start time of the current reflow
  long mReflowStartTimeMillis = 0;
  // The zone of the profile we're currently in
  uint8_t mProfileZone;

};

#endif // __REFLOWMODEL_H_INCLUDED__