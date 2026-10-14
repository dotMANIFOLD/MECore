//
// Created by ryen on 2/24/25.
//

#pragma once

#include <SDL3/SDL.h>

#include "IRenderTarget.h"
#include "MECore/Common/IDestructible.h"

using namespace Diligent;

namespace ME {
    struct WindowParameters {
        std::string title;
        int width;
        int height;
        bool fullscreen;
        bool borderless;
        bool resizeable;
        bool transparent;
    };

    class Window : public IRenderTarget, public IDestructible {
        protected:
        WindowParameters details;
        SDL_Window* window;
        ISwapChain* swapChain;
        
        public:
        Window(const WindowParameters& params, SDL_Window* window, ISwapChain* swapChain);
        ~Window() override;
        
        const std::string& GetTitle() const { return details.title; }
        void GetSize(int* width, int* height) const override {
            *width = this->details.width;
            *height = this->details.height;
        }
        bool GetFullscreen() const { return details.fullscreen; }
        bool GetBorderless() const { return details.borderless; }
        bool GetResizeable() const { return details.resizeable; }
        bool GetTransparent() const { return details.transparent; }
        
        SDL_Window* GetWindow() const { return window; }
        ISwapChain* GetSwapChain() const override { return swapChain; }
        bool CanRender() const override { return true; }
        bool IsValid() const override { return window && swapChain; }
        
        void SetTitle(const std::string& name);
        void SetSize(int width, int height);
        void SetFullscreen(bool fullscreen);
        void SetBorderless(bool borderless);
        void SetResizeable(bool resizeable);

        void Present();
        void Destroy() override;
    };
}
