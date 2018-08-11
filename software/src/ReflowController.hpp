#ifndef __REFLOWTIMER_H_INCLUDED__
#define __REFLOWTIMER_H_INCLUDED__

#include "Refreshable.hpp"
#include "ReflowModel.hpp"

/**
 * Controls the Reflow process
 */
class ReflowController : public Refreshable {

public:

  ReflowController(ReflowModel* reflowModel) : mReflowModel(reflowModel) {};

  // Initialises the controller and model
  void init();
  // Checks the time and controls the process based on it
  void refresh();
  // Start a reflow
  void startReflow();
  // Abort a reflow once it's started
  void abortReflow();

private:

  // Determines what the temperature should be at this point in an active profile
  int16_t determineTargetProfileTempNow();

  // Pointer to the model
  ReflowModel* mReflowModel;

};

#endif