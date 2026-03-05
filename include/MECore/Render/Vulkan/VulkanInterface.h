//
// Created by dotryen on 3/3/26.
//

#pragma once

#include <memory>
#include <RenderDevice.h>
#include <vector>

#include "MECore/Render/IRenderInterface.h"
#include "MECore/Render/Window.h"

using namespace Diligent;

namespace ME {
    class VulkanInterface : public IRenderInterface {
        private:
        IRenderDevice* device;
        IDeviceContext* immediateContext;
        
        std::vector<std::shared_ptr<Window>> windows;
        std::weak_ptr<Window> mainWindow;
        
        public:
        VulkanInterface() = default;
        
        IRenderDevice* GetDevice() const override { return device; }
        IDeviceContext* GetImmediateContext() const override { return immediateContext; }
        
        const std::vector<std::shared_ptr<Window>>& GetWindows() const { return windows; }
        std::shared_ptr<Window> GetMainWindow() const override { return mainWindow.lock(); }
        
        bool Initialize() override;

        std::shared_ptr<Window> CreateWindow(const WindowParameters& params) override;
        void SetMainWindow(const std::shared_ptr<Window> window) override { mainWindow = window; }
    };
}
