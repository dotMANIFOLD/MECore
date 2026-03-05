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
        bool borderless;
        bool resizeable;
        bool transparent;
    };

    class Window : public IRenderTarget, public IDestructible {
        private:
        std::string title;
        int width, height;
        bool fullscreen;
        bool borderless;
        bool resizeable;
        bool transparent;
        
        SDL_Window* window;
        ISwapChain* swapChain;
        
        public:
        Window(const WindowParameters& params, SDL_Window* window, ISwapChain* swapChain);
        
        const std::string& GetTitle() const { return title; }
        void GetSize(int* width, int* height) const override {
            *width = this->width;
            *height = this->height;
        }
        bool GetFullscreen() const { return fullscreen; }
        bool GetBorderless() const { return borderless; }
        bool GetResizeable() const { return resizeable; }
        bool GetTransparent() const { return transparent; }
        
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
