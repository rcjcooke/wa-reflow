#ifndef __SR1230_H_INCLUDED__
#define __SR1230_H_INCLUDED__

#include "Encoder.hpp"
#include <stdint.h>

class SR1230 {

public:
  SR1230(uint8_t pinA, uint8_t pinB, uint8_t pinS)
      : mChannelAPin(pinA), mChannelBPin(pinB), mSwitchPin(pinS),
        mEncoder(Encoder(pinA, pinB)){};

  // Initialise
  void init();

  const uint8_t getChannelAPin() const { return mChannelAPin; }
  const uint8_t getChannelBPin() const { return mChannelBPin; }
  const uint8_t getSwitchPin() const { return mSwitchPin; }
  long getPosition() { return mEncoder.getPosition(); }
  bool isButtonPressesWaitingToAction() {return mButtonPressesWaitingToAction;}

  void userTurn();
  void userSelectButtonPress();
  void buttonPressActioned();

protected:
  const uint8_t mChannelAPin;
  const uint8_t mChannelBPin;
  const uint8_t mSwitchPin;
  Encoder mEncoder;
  bool mButtonPressesWaitingToAction = false;
};

#endif // __SR1230_H_INCLUDED__