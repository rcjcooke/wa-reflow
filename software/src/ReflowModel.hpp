#ifndef __REFLOWMODEL_H_INCLUDED__
#define __REFLOWMODEL_H_INCLUDED__

#include "reflowprofile/ReflowProfile.hpp"

enum class ReflowOvenState : uint8_t {
  UserSelecting,
  Reflowing
};

class ReflowModel {

public:

  // Initialise the oven in the user selection state
  ReflowModel() : mOvenState(ReflowOvenState::UserSelecting) {};

  // Get the current overall state of the oven
  ReflowOvenState getOvenState() const {return mOvenState;}

private:
  // The current state of the oven
  ReflowOvenState mOvenState;
  // The zone of the profile we're currently in
  uint8_t mProfileZone;
  
};

#endif // __REFLOWMODEL_H_INCLUDED__