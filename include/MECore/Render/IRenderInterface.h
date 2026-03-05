//
// Created by ryen on 2/21/25.
//

#pragma once

#include <memory>
#include <RenderDevice.h>

#include "MECore/Service/IService.h"
#include "Window.h"

namespace ME {
    class IRenderInterface : public IService {
        public:
        virtual Diligent::IRenderDevice* GetDevice() const = 0;
        virtual Diligent::IDeviceContext* GetImmediateContext() const = 0;
        virtual std::shared_ptr<Window> GetMainWindow() const = 0;

        virtual bool Initialize() = 0;

        virtual std::shared_ptr<Window> CreateWindow(const WindowParameters& params) = 0;
        virtual void SetMainWindow(const std::shared_ptr<Window> window) = 0;
    };
}
