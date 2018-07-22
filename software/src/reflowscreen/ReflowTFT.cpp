#include "ReflowTFT.hpp"

ReflowOvenState localOvenState;

void ReflowTFT::init() {
  mScreen.initR(INITR_144GREENTAB);
  mScreen.setRotation(ROTATE_0_DEGREES);
  mScreen.fillScreen(BLACK);
  // Tried fonts to make it look nicer but had real problems with degree symbol - so now it's just default
  mScreen.setFont();
}

void ReflowTFT::refresh() {
  // Check the overall state first as this affects the whole screen
  if (mReflowModel->getOvenState() != localOvenState) {
    switch(mReflowModel->getOvenState()) {
      case ReflowOvenState::UserSelecting:
        mReflowSelectionScreen.drawScreen();
        localOvenState = ReflowOvenState::UserSelecting;
        break;
      case ReflowOvenState::Reflowing:
        mReflowProgressScreen.drawScreen();
        localOvenState = ReflowOvenState::Reflowing;
        break;
    }
  }
}