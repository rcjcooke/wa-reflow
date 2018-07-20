#ifndef __SAC305_H_INCLUDED__
#define __SAC305_H_INCLUDED__

#include "ReflowProfile.hpp"

class SAC305 : ReflowProfile {

  SAC305()
      : ReflowProfile(
            "SAC305", (uint8_t)5,
            (ReflowZone[]){{ReflowState::Preheat, 1.5, 150, 90000},
                           {ReflowState::Soak, 1.5, 175, 90000},
                           {ReflowState::Ramp, 1.5, 217, 30000},
                           {ReflowState::Reflow, 1.5, 249, 30000},
                           {ReflowState::Solidifying, 1.5, 217, 30000}}){};
};

#endif // __SAC305_H_INCLUDED__