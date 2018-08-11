#ifndef __SELECTIONSWITCH_H_INCLUDED__
#define __SELECTIONSWITCH_H_INCLUDED__

#include "../ReflowController.hpp"
#include "../ReflowModel.hpp"
#include "../Refreshable.hpp"
#include "SR1230.hpp"
#include <Arduino.h>

typedef void (*voidFuncPtr)(void);

class SelectionSwitch : public Refreshable {

public:
  SelectionSwitch(ReflowModel *reflowModel, ReflowController *reflowController,
                  voidFuncPtr userTurnInterruptDispatch,
                  voidFuncPtr userSelectButtonPressInterruptDispatch,
                  uint8_t pinA, uint8_t pinB, uint8_t pinS)
      : mReflowModel(reflowModel), mReflowController(reflowController),
        mSR1230(SR1230(pinA, pinB, pinS)),
        mUserTurnInterruptDispatchFunctionPtr(userTurnInterruptDispatch),
        mUserSelectButtonPressInterruptDispatchFunctionPtr(
            userSelectButtonPressInterruptDispatch){};

  // Call to initialise - WARNING: Attaches 2 interrupts
  void init();
  // Called whenever the screen needs to check for model updates
  void refresh();
  // Method to be called by interrupt routine to update the switch state
  void userTurnInterrupt();
  // Method to be called by interrupt routine to update the switch state
  void userSelectButtonPressInterrupt();

private:
  ReflowModel *mReflowModel;
  ReflowController *mReflowController;
  SR1230 mSR1230;
  voidFuncPtr mUserTurnInterruptDispatchFunctionPtr;
  voidFuncPtr mUserSelectButtonPressInterruptDispatchFunctionPtr;

  long int mEncoderPreviousPosition;
};

#endif // __SELECTIONSWITCH_H_INCLUDED__