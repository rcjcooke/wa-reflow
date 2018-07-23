#ifndef __SAC305_H_INCLUDED__
#define __SAC305_H_INCLUDED__

#include "ReflowProfile.hpp"

const ReflowZone zones[] = {{ReflowState::Preheat, 1.5, 150, 90},
                            {ReflowState::Soak, 1.5, 175, 90},
                            {ReflowState::Ramp, 1.5, 217, 30},
                            {ReflowState::Reflow, 1.5, 249, 30},
                            {ReflowState::Solidifying, 1.5, 217, 30}};

class SAC305_ : public ReflowProfile {

public:
  SAC305_() : ReflowProfile("SAC305", (uint8_t)5, zones){};
};

const SAC305_ SAC305 = SAC305_();

#endif // __SAC305_H_INCLUDED__