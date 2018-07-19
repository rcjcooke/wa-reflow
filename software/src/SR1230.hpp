#ifndef __SR1230_H_INCLUDED__
#define __SR1230_H_INCLUDED__

#include <stdint.h>

class SR1230 {

public:
  SR1230(uint8_t pinA, uint8_t pinB, uint8_t pinS) : mChannelAPin(pinA), mChannelBPin(pinA), mSwitchPin(pinA) {};

  // Call this after creation. Warning: This will attach 2 interrupts
  void init();

protected:

private:

  const uint8_t mChannelAPin;
  const uint8_t mChannelBPin;
  const uint8_t mSwitchPin;

};

#endif // __SR1230_H_INCLUDED__