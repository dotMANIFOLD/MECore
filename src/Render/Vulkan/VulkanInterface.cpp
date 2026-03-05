//
// Created by dotryen on 3/3/26.
//

#include "MECore/Render/Vulkan/VulkanInterface.h"

#include "EngineFactoryVk.h"
#include "GraphicsTypes.h"
#include "MECore/Render/Window.h"
#include "spdlog/spdlog.h"

using namespace Diligent;

namespace ME {
    bool VulkanInterface::Initialize() {
        EngineVkCreateInfo engineCreateInfo;
        
        // DEVICE FEATURES
        engineCreateInfo.Features = DeviceFeatures(DEVICE_FEATURE_STATE_OPTIONAL);
        
        // VULKAN SPECIFIC
        engineCreateInfo.FeaturesVk.DynamicRendering = DEVICE_FEATURE_STATE_OPTIONAL;
        
        auto* vkFactory = GetEngineFactoryVk();
        vkFactory->CreateDeviceAndContextsVk(engineCreateInfo, &device, &immediateContext);
        return true;
    }

    std::shared_ptr<Window> VulkanInterface::CreateWindow(const WindowParameters& params) {
        auto flags = SDL_WINDOW_VULKAN;
        if (params.borderless) flags |= SDL_WINDOW_BORDERLESS;
        // TODO: reimplement resizing
        // if (params->resizeable) flags |= SDL_WINDOW_RESIZABLE;
        if (params.transparent) flags |= SDL_WINDOW_TRANSPARENT;
        auto sdlWindow = SDL_CreateWindow(params.title.c_str(), params.width, params.height, flags);
        if (!sdlWindow) {
            spdlog::error("VulkanInterface: Failed to create window.");
            return nullptr;
        }
        
        LinuxNativeWindow diligentWindow;
        
        auto driver = SDL_GetCurrentVideoDriver();
        auto properties = SDL_GetWindowProperties(sdlWindow);
        
        const char* waylandDriver = "wayland";
        const char* xDriver = "x11";
        
        if (memcmp(driver, waylandDriver, strlen(waylandDriver)) == 0) {
            diligentWindow.pDisplay = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
            diligentWindow.pWaylandSurface = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
        } else if (memcmp(driver, xDriver, strlen(xDriver)) == 0) {
            diligentWindow.pDisplay = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
            diligentWindow.WindowId = SDL_GetNumberProperty(properties, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, -1);
        } else {
            spdlog::critical("VulkanInterface: Unknown video driver '{}'", driver);
            assert(false);
        }
        
        auto* factory = GetEngineFactoryVk();
        
        SwapChainDesc swapChainDesc;
        swapChainDesc.Width = params.width;
        swapChainDesc.Height = params.height;
        ISwapChain* swapChain;
        factory->CreateSwapChainVk(device, immediateContext, swapChainDesc, diligentWindow, &swapChain);
        
        auto window = std::make_shared<Window>(params, sdlWindow, swapChain);
        windows.push_back(window);
        return window;
    }
}
