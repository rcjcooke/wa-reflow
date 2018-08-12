#ifndef __SAC305_H_INCLUDED__
#define __SAC305_H_INCLUDED__

#include "ReflowProfile.hpp"

static ReflowZone zones[] = {{ReflowState::Preheat, 1.5, 150, 90},
                      {ReflowState::Soak, 1.5, 175, 90},
                      {ReflowState::Ramp, 1.5, 217, 30},
                      {ReflowState::Reflow, 1.5, 249, 30},
                      {ReflowState::Cooling, 1.5, 217, 30}};

class SAC305_ : public ReflowProfile {

  char NAME[7] = {'S', 'A', 'C', '3', '0', '5', '\0'};

public:
  SAC305_() : ReflowProfile(NAME, (uint8_t)5, zones){};
};

extern SAC305_ SAC305;

#endif // __SAC305_H_INCLUDED__