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
  // Set the current overall state of the oven
  void setOvenState(ReflowOvenState newState) {mOvenState = newState;}
  // Set the oven temperature
  void setOvenTemp(int16_t temp) {mOvenTemp = temp;}
  // Get the number of profiles this oven knows about
  const uint8_t getNumProfiles() const {return mNumProfiles;}
  // Get the full set of profiles this oven supports
  ReflowProfile* getReflowProfile(int profileIndex) const {return mReflowProfiles[profileIndex];}

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
  ReflowProfile* mReflowProfiles[10] = {(SAC305_*) new SAC305_(), new ReflowProfile("Test2", 0, nullptr), new ReflowProfile("Test3", 0, nullptr), new ReflowProfile("Test4", 0, nullptr), new ReflowProfile("Test5", 0, nullptr), new ReflowProfile("Test6", 0, nullptr), new ReflowProfile("Test7", 0, nullptr), new ReflowProfile("Test8", 0, nullptr), new ReflowProfile("Test9", 0, nullptr), new ReflowProfile("Test10", 0, nullptr)};
  // The number of reflow profiles in memory
  const uint8_t mNumProfiles = 10;

  // The zone of the profile we're currently in
  uint8_t mProfileZone;

};

#endif // __REFLOWMODEL_H_INCLUDED__