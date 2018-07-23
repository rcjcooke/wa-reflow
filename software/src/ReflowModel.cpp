#include "ReflowModel.hpp"

#include "reflowprofile/SAC305.hpp"

void ReflowModel::initialiseModel() {
  // Set the initial oven state
  mOvenState = ReflowOvenState::UserSelecting;
  // Populate the profiles list
  ReflowProfile* profiles[] = {new SAC305_()};
  mNumProfiles = 1;
  mReflowProfiles = profiles;
}