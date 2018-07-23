#include "ReflowModel.hpp"

void ReflowModel::initialiseModel() {
  // Set the initial oven state
  mOvenState = ReflowOvenState::UserSelecting;
}