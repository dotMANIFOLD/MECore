//
// Created by dotryen on 3/4/26.
//

#include "MECore/Render/Window.h"

namespace ME {
    Window::Window(const WindowParameters& params, SDL_Window* window, ISwapChain* swapChain) {
        this->details = params;
        this->window = window;
        this->swapChain = swapChain;
    }

    Window::~Window() {
        if (Window::IsValid()) {
            Destroy();
        }
    }

    void Window::SetTitle(const std::string& name) {
        this->details.title = name;
        SDL_SetWindowTitle(window, name.c_str());
    }

    void Window::SetSize(int width, int height) {
        this->details.width = width;
        this->details.height = height;
        SDL_SetWindowSize(window, width, height);
        swapChain->Resize(width, height);
    }

    void Window::SetFullscreen(bool fullscreen) {
        this->details.fullscreen = fullscreen;
        SDL_SetWindowFullscreen(window, fullscreen);
    }

    void Window::SetBorderless(bool borderless) {
        this->details.borderless = borderless;
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
        swapChain->Release();
        SDL_DestroyWindow(window);
        swapChain = nullptr;
        window = nullptr;
    }
}
