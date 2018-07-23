#ifndef __SR1230_H_INCLUDED__
#define __SR1230_H_INCLUDED__

#include <stdint.h>

class SR1230 {

public:
  SR1230(uint8_t pinA, uint8_t pinB, uint8_t pinS) : mChannelAPin(pinA), mChannelBPin(pinA), mSwitchPin(pinA) {};

  const uint8_t getChannelAPin() const {return mChannelAPin;}
  const uint8_t getChannelBPin() const {return mChannelBPin;}
  const uint8_t getSwitchPin() const {return mSwitchPin;}

private:

  const uint8_t mChannelAPin;
  const uint8_t mChannelBPin;
  const uint8_t mSwitchPin;

};

#endif // __SR1230_H_INCLUDED__