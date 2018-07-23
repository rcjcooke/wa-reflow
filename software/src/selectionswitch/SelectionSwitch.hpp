#ifndef __SELECTIONSWITCH_H_INCLUDED__
#define __SELECTIONSWITCH_H_INCLUDED__

#include <Arduino.h>
#include "SR1230.hpp"
#include "../Refreshable.hpp"
#include "../ReflowModel.hpp"

class SelectionSwitch : public Refreshable {

public:
  SelectionSwitch(ReflowModel* reflowModel, uint8_t pinA, uint8_t pinB, uint8_t pinS) : mReflowModel(reflowModel), mSR1230(SR1230(pinA, pinB, pinS)) {};

  // Call to initialise - WARNING: Attaches 2 interrupts
  void init();
  // Called whenever the screen needs to check for model updates
  void refresh();

private:

  ReflowModel* mReflowModel;
  SR1230 mSR1230;
  long int mEncoderPosition;

};

#endif // __SELECTIONSWITCH_H_INCLUDED__