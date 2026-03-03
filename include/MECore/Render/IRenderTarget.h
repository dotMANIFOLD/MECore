//
// Created by ryen on 2/25/25.
//

#pragma once

#include <nvrhi/nvrhi.h>

#include "IViewport.h"

namespace ME {
    class IRenderTarget : public IViewport {
        public:
        virtual bool CanRender() const = 0;
        virtual nvrhi::FramebufferHandle GetFramebuffer() const = 0;
    };
}
