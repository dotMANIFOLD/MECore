//
// Created by ryen on 2/25/25.
//

#include <spdlog/spdlog.h>

#include "MECore/Render/RenderWindow.h"

#include "MECore/MECore.h"
#include "MECore/Render/IRenderInterface.h"

namespace ME {
    RenderWindow::RenderWindow(Window* window) {
        this->window = window;
        this->window->beforeRefresh = [this](){ DestroyFramebuffers(); };
        this->window->afterRefresh = [this](){ CreateFramebuffers(); };
        CreateFramebuffers();
    }

    RenderWindow::~RenderWindow() {
        if (window) {
            window->beforeRefresh = nullptr;
            window->afterRefresh = nullptr;
        }
    }

    void RenderWindow::CreateFramebuffers() {
        auto service = MEInstance::Instance()->GetServiceProvider()->GetRenderService();
        assert(service);
        
        int width, height;
        window->GetSize(&width, &height);

        auto nvDevice = service->GetInterface()->GetDevice();
        auto depthDesc = nvrhi::TextureDesc()
            .setWidth(width)
            .setHeight(height)
            .setFormat(nvrhi::Format::D32S8)
            .setIsRenderTarget(true)
            .setInitialState(nvrhi::ResourceStates::DepthWrite)
            .setKeepInitialState(true);
        depthBuffer = nvDevice->createTexture(depthDesc);

        auto count = window->GetSwapchainCount();
        framebuffers.resize(count);
        for (int i = 0; i < count ; i++) {
            auto frameBufferDesc = nvrhi::FramebufferDesc()
                .addColorAttachment(window->GetSwapchainTexture(i))
                .setDepthAttachment(depthBuffer);
            framebuffers[i] = nvDevice->createFramebuffer(frameBufferDesc);
        }
    }

    void RenderWindow::DestroyFramebuffers() {
        framebuffers.clear();
        depthBuffer = nullptr;
    }
}
