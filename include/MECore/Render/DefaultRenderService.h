//
// Created by dotryen on 1/1/26.
//

#pragma once
#include "IRenderService.h"
#include "Vulkan/VulkanInterface.h"

namespace ME {
    class DefaultRenderService : public IRenderService {
        private:
        VulkanInterface interface;
        Window* mainWindow;
        IRenderTarget* mainRenderTarget;
        
        public:
        DefaultRenderService();
        ~DefaultRenderService() override;
        
        IRenderInterface* GetInterface() override { return &interface; }
        
        Window* GetMainWindow() override { return mainWindow; }
        IRenderTarget* GetMainRenderTarget() override { return mainRenderTarget; }
        IRenderPipeline* GetPipeline() override { return nullptr; }
        
        void SetMainWindow(Window* window) override { mainWindow = window; }
        void SetMainRenderTarget(IRenderTarget* target) override { mainRenderTarget = target; }
    };
}
