//
// Created by ryen on 2/24/25.
//

#pragma once

#include <queue>
#include <nvrhi/nvrhi.h>
#include <nvrhi/vulkan.h>
#include <vulkan/vulkan.hpp>

#include "../Window.h"
#include "VulkanInterface.h"

namespace ME {
    class VulkanWindow : public Window {
        protected:
        struct SwapChainImage {
            vk::Image image;
            nvrhi::TextureHandle rhiHandle;
        };

        VulkanInterface* interface;

        std::string title;
        int width, height;
        bool fullscreen;
        bool borderless;
        bool resizeable;
        bool transparent;

        SDL_Window* window;
        VkSurfaceKHR surface;

        vk::SwapchainKHR vkSwapchain;
        std::vector<SwapChainImage> swapChainImages;
        uint32_t swapchainIndex;

        std::vector<vk::Semaphore> presentSemaphores;
        std::vector<vk::Semaphore> acquireSemaphores;
        uint32_t presentSemaphoreIndex;
        uint32_t acquireSemaphoreIndex;

        std::queue<nvrhi::EventQueryHandle> framesInFlight;
        std::vector<nvrhi::EventQueryHandle> queryPool;

        bool CreateSwapchainInternal();
        void DestroySwapchainInternal();

        public:
        VulkanWindow(WindowParameters* params, VulkanInterface* interface, SDL_Window* window, VkSurfaceKHR vkSurface);
        ~VulkanWindow();

        bool IsValid() const override;

        SDL_Window* GetWindow() const override { return window; }
        const std::string& GetTitle() const override { return title; }
        void GetSize(int* width, int* height) const override {
            *width = this->width;
            *height = this->height;
        }
        bool GetFullscreen() const override { return fullscreen; }
        bool GetBorderless() const override { return borderless; }
        bool GetResizeable() const override { return resizeable; }
        bool GetTransparent() const override { return transparent; }

        Uint32 GetSwapchainCount() override {
            return swapChainImages.size();
        }
        Uint32 GetCurrentSwapchainIndex() override {
            return swapchainIndex;
        }
        nvrhi::ITexture* GetSwapchainTexture(Uint32 index) override {
            if (index < swapChainImages.size()) {
                return swapChainImages[index].rhiHandle;
            }
            return nullptr;
        }
        nvrhi::ITexture* GetCurrentSwapchainTexture() override {
            return swapChainImages[swapchainIndex].rhiHandle;
        }

        void SetTitle(const std::string& title) override {
            this->title = title;
            SDL_SetWindowTitle(window, this->title.c_str());
        }
        void SetSize(int width, int height) override {
            this->width = width;
            this->height = height;
            SDL_SetWindowSize(window, width, height);
            RefreshSwapchain();
        }
        void SetFullscreen(bool fullscreen) override {
            this->fullscreen = fullscreen;
            SDL_SetWindowFullscreen(window, fullscreen);
        }
        void SetBorderless(bool borderless) override {
            this->borderless = borderless;
            SDL_SetWindowBordered(window, !borderless);
        }
        void SetResizeable(bool resizeable) override {
            this->resizeable = resizeable;
            SDL_SetWindowResizable(window, resizeable);
        }

        bool CreateSwapchain() override;
        void DestroySwapchain() override;
        void RefreshSwapchain() override;
        void Destroy() override;

        bool BeginFrame() override;
        bool Present() override;
    };
}
