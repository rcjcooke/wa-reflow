#include "ReflowTFT.hpp"

ReflowOvenState localOvenState = ReflowOvenState::UserSelecting;

void ReflowTFT::init() {
  mScreen.initR(INITR_144GREENTAB);
  mScreen.setRotation(ROTATE_0_DEGREES);
  mScreen.fillScreen(BLACK);
  // Tried fonts to make it look nicer but had real problems with degree symbol - so now it's just default
  mScreen.setFont();
  // Draw the initial screen
  drawScreen();
}

void ReflowTFT::refresh() {
  // Check the overall state first as this affects the whole screen
  if (mReflowModel->getOvenState() != localOvenState) {
    // Clear the current screen
    mScreen.fillScreen(BLACK);
    // Draw the new one
    drawScreen();
    localOvenState = mReflowModel->getOvenState();
  }

  // Refresh the screen
  mCurrentScreen->refresh();
}

void ReflowTFT::drawScreen() {
  switch(mReflowModel->getOvenState()) {
    case ReflowOvenState::UserSelecting:
      mReflowSelectionScreen.drawScreen();
      mCurrentScreen = &mReflowSelectionScreen;
      break;
    case ReflowOvenState::Reflowing:
      mReflowProgressScreen.drawScreen();
      mCurrentScreen = &mReflowProgressScreen;
      break;
  }
}