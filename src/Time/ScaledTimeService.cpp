//
// Created by ryen on 12/23/24.
//

#include <SDL3/SDL_timer.h>

#include "MECore/Time/ScaledTimeService.h"

namespace ME {
    ScaledTimeService::ScaledTimeService() {
        elapsed = 0;
        delta = 0;
        scale = 1;
        id = 0;
        lastChecked = 0;
        Reset();
    }

    void ScaledTimeService::Update() {
        auto now = SDL_GetTicksNS();
        auto tmp = now - lastChecked;
        delta = SDL_NS_TO_SECONDS(static_cast<double>(tmp)) * scale;
        elapsed += delta;
        lastChecked = now;
        id++;
    }

    void ScaledTimeService::Reset() {
        elapsed = 0;
        delta = 0;
        scale = 1;
        lastChecked = SDL_GetTicksNS();
    }
}
