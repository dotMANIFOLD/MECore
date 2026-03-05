//
// Created by ryen on 2/25/25.
//

#pragma once

#include <SwapChain.h>

#include "IViewport.h"

namespace ME {
    class IRenderTarget : public IViewport {
        public:
        virtual bool CanRender() const = 0; // do we still need this function?
        virtual Diligent::ISwapChain* GetSwapChain() const = 0;
    };
}
