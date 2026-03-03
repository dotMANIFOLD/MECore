//
// Created by dotryen on 1/1/26.
//

#include "MECore/Render/DefaultRenderService.h"

namespace ME {
    DefaultRenderService::DefaultRenderService() {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
    }

    DefaultRenderService::~DefaultRenderService() {
        interface.DestroyDevice();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}
