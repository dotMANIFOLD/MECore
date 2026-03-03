//
// Created by dotryen on 12/18/25.
//

#pragma once
#include "IRenderInterface.h"
#include "IRenderTarget.h"
#include "Window.h"
#include "MECore/Service/IService.h"
#include "Pipeline/IRenderPipeline.h"

namespace ME {
    class IRenderService : IService {
        public:
        virtual IRenderInterface* GetInterface() = 0;
        
        virtual Window* GetMainWindow() = 0;
        virtual IRenderTarget* GetMainRenderTarget() = 0;
        
        virtual IRenderPipeline* GetPipeline() = 0;
        
        virtual void SetMainWindow(Window* window) = 0;
        virtual void SetMainRenderTarget(IRenderTarget* target) = 0;
    };
}
