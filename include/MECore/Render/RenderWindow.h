//
// Created by ryen on 2/25/25.
//

#pragma once

#include "IRenderTarget.h"
#include "Window.h"

namespace ME {
    class RenderWindow : public IRenderTarget {
        private:
        Window* window;
        std::vector<nvrhi::FramebufferHandle> framebuffers;
        nvrhi::TextureHandle depthBuffer;

        void CreateFramebuffers();
        void DestroyFramebuffers();

        public:
        RenderWindow() = default;
        explicit RenderWindow(Window* window);
        ~RenderWindow() override;

        void GetSize(int* width, int* height) const override {
            window->GetSize(width, height);
        }
        bool CanRender() const override {
            return window && !framebuffers.empty();
        }
        nvrhi::FramebufferHandle GetFramebuffer() const override {
            if (!window) return nullptr;
            return framebuffers[window->GetCurrentSwapchainIndex()];
        }
        Window* GetWindow() const {
            return window;
        }
    };
}
