//
// Created by dotryen on 3/4/26.
//

#include "MECore/Render/Window.h"

namespace ME {
    Window::Window(const WindowParameters& params, SDL_Window* window, ISwapChain* swapChain) {
        title = params.title;
        width = params.width;
        height = params.height;
        borderless = params.borderless;
        resizeable = params.resizeable;
        transparent = params.transparent;
        
        this->window = window;
        this->swapChain = swapChain;
    }

    void Window::SetTitle(const std::string& name) {
        this->title = name;
        SDL_SetWindowTitle(window, name.c_str());
    }

    void Window::SetSize(int width, int height) {
        this->width = width;
        this->height = height;
        SDL_SetWindowSize(window, width, height);
        swapChain->Resize(width, height);
    }

    void Window::SetFullscreen(bool fullscreen) {
        this->fullscreen = fullscreen;
        SDL_SetWindowFullscreen(window, fullscreen);
    }

    void Window::SetBorderless(bool borderless) {
        this->borderless = borderless;
        SDL_SetWindowBordered(window, borderless);
    }

    void Window::SetResizeable(bool resizeable) {
        // TODO: reimplement resizing
        // this->resizeable = resizeable;
        // SDL_SetWindowResizable(window, resizeable);
    }

    void Window::Present() {
        swapChain->Present();
    }

    void Window::Destroy() {
        // TODO: do something i forgot
    }
}
