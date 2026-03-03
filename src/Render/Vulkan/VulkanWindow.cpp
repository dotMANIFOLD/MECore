//
// Created by ryen on 2/24/25.
//

#include <spdlog/spdlog.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "MECore/Render/Vulkan/VulkanWindow.h"

#include "nvrhi/utils.h"

namespace ME {
    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    VulkanWindow::VulkanWindow(WindowParameters* params, VulkanInterface* interface, SDL_Window* window, VkSurfaceKHR vkSurface) {
        title = params->title;
        width = params->width;
        height = params->height;
        borderless = params->borderless;
        resizeable = params->resizeable;
        transparent = params->transparent;

        this->interface = interface;
        this->window = window;
        this->surface = vkSurface;
        swapchainIndex = 0;
        acquireSemaphoreIndex = 0;
        presentSemaphoreIndex = 0;
    }

    VulkanWindow::~VulkanWindow() {

    }

    bool VulkanWindow::IsValid() const {
        return surface;
    }

    bool VulkanWindow::CreateSwapchain() {
        if (!CreateSwapchainInternal()) return false;

        presentSemaphores.reserve(MAX_FRAMES_IN_FLIGHT + 1);
        acquireSemaphores.reserve(MAX_FRAMES_IN_FLIGHT + 1);
        for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT + 1; i++) {
            presentSemaphores.push_back(interface->GetVkDevice().createSemaphore(vk::SemaphoreCreateInfo()));
            acquireSemaphores.push_back(interface->GetVkDevice().createSemaphore(vk::SemaphoreCreateInfo()));
        }

        return true;
    }

    void VulkanWindow::DestroySwapchain() {
        DestroySwapchainInternal();

        auto device = interface->GetVkDevice();
        for (auto& semaphore : presentSemaphores) {
            if (semaphore) {
                device.destroySemaphore(semaphore);
                semaphore = vk::Semaphore();
            }
        }
        for (auto& semaphore : acquireSemaphores) {
            if (semaphore) {
                device.destroySemaphore(semaphore);
                semaphore = vk::Semaphore();
            }
        }
    }

    void VulkanWindow::RefreshSwapchain() {
        if (vkSwapchain) {
            beforeRefresh();
            DestroySwapchainInternal();
            CreateSwapchainInternal();
            afterRefresh();
        }
    }

    void VulkanWindow::Destroy() {
        DestroySwapchain();

        if (surface) {
            SDL_Vulkan_DestroySurface(interface->GetVkInstance(), surface, nullptr);
            surface = nullptr;
        }
    }

    bool VulkanWindow::BeginFrame() {
        const auto& semaphore = acquireSemaphores[acquireSemaphoreIndex];

        vk::Result result;

        const int maxAttempts = 3;
        for (int attempt = 0; attempt < maxAttempts; ++attempt) {
            // This line is causing some issues for some reason?
            // result = vkDevice.acquireNextImageKHR(vkSwapchain, std::numeric_limits<uint64_t>::max(), semaphore, vk::Fence(), swapchainIndex).result;
            result = vk::Result(vkAcquireNextImageKHR(interface->GetVkDevice(), vkSwapchain, UINT64_MAX, semaphore, VkFence(), &swapchainIndex));
            if (result == vk::Result::eErrorOutOfDateKHR && attempt < maxAttempts) {
                auto caps = interface->GetVkPhysicalDevice().getSurfaceCapabilitiesKHR(surface);
                width = caps.currentExtent.width;
                height = caps.currentExtent.height;

                spdlog::warn("Vulkan swapchain out of date! Refreshing...");
                RefreshSwapchain();
            } else {
                break;
            }
        }

        acquireSemaphoreIndex = (acquireSemaphoreIndex + 1) % acquireSemaphores.size();

        if (static_cast<int>(result) >= 0) {
            interface->GetNvDevice()->queueWaitForSemaphore(nvrhi::CommandQueue::Graphics, semaphore, 0);
            return true;
        }
        spdlog::error("VulkanWindow: Failed to begin frame. {}", nvrhi::vulkan::resultToString(static_cast<VkResult>(result)));
        return false;
    }

    bool VulkanWindow::Present() {
        const auto& semaphore = presentSemaphores[presentSemaphoreIndex];
        auto nvDevice = interface->GetNvDevice();

        nvDevice->queueSignalSemaphore(nvrhi::CommandQueue::Graphics, semaphore, 0);

        // This captures the semaphore apparently
        nvDevice->executeCommandLists(nullptr, 0);

        vk::PresentInfoKHR info = vk::PresentInfoKHR()
            .setWaitSemaphoreCount(1)
            .setPWaitSemaphores(&semaphore)
            .setSwapchainCount(1)
            .setPSwapchains(&vkSwapchain)
            .setPImageIndices(&swapchainIndex);

        const vk::Result result = interface->QueuePresent(&info);
        if (static_cast<int>(result) < 0) {
            spdlog::error("VulkanWindow: Failed to present to frame. {}", nvrhi::vulkan::resultToString(static_cast<VkResult>(result)));
            return false;
        }

        presentSemaphoreIndex = (presentSemaphoreIndex + 1) % presentSemaphores.size();

        while (framesInFlight.size() >= MAX_FRAMES_IN_FLIGHT) {
            auto query = framesInFlight.front();
            framesInFlight.pop();

            nvDevice->waitEventQuery(query);
            queryPool.push_back(query);
        }

        nvrhi::EventQueryHandle query;
        if (!queryPool.empty()) {
            query = queryPool.back();
            queryPool.pop_back();
        } else {
            query = nvDevice->createEventQuery();
        }

        nvDevice->resetEventQuery(query);
        nvDevice->setEventQuery(query, nvrhi::CommandQueue::Graphics);
        framesInFlight.push(query);
        return true;
    }

    bool VulkanWindow::CreateSwapchainInternal() {
        auto vkPhysicalDevice = interface->GetVkPhysicalDevice();
        auto capabilities = vkPhysicalDevice.getSurfaceCapabilitiesKHR(surface);
        
        auto nvFormat = nvrhi::Format::BGRA8_UNORM; // TODO: make this adjustable
        auto vkFormat = vk::Format(nvrhi::vulkan::convertFormat(nvFormat));
        auto colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

        vk::Extent2D extent = { (uint32_t)width, (uint32_t)height };
        
        std::vector<uint32_t> queues{};
        queues.reserve(2);
        queues.push_back(interface->GetGraphicsQueueFamily());
        int nextFamily = interface->GetPresentQueueFamily();
        if (std::ranges::find(queues, nextFamily) == queues.end()) {
            queues.push_back(nextFamily);
        }
        
        const bool enableSharing = queues.size() > 1;

        auto desc = vk::SwapchainCreateInfoKHR()
            .setSurface(surface)
            .setMinImageCount(capabilities.minImageCount)
            .setImageFormat(vkFormat)
            .setImageColorSpace(colorSpace)
            .setImageExtent(extent)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
            .setImageSharingMode(enableSharing ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive)
            .setFlags(vk::SwapchainCreateFlagBitsKHR::eMutableFormat) // TODO: deepnds on mutable swapchain, add check.
            .setQueueFamilyIndexCount(enableSharing ? queues.size() : 0)
            .setPQueueFamilyIndices(enableSharing ? queues.data() : nullptr)
            .setPreTransform(vk::SurfaceTransformFlagBitsKHR::eIdentity)
            .setCompositeAlpha(transparent ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied : vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setPresentMode(vk::PresentModeKHR::eImmediate) // TODO: Immediate means no vsync, FIFO means vsync. Allow this to be set
            .setClipped(true)
            .setOldSwapchain(nullptr);

        std::vector<vk::Format> imageFormats = { vkFormat };
        switch(vkFormat) {
            case vk::Format::eR8G8B8A8Unorm:
                imageFormats.push_back(vk::Format::eR8G8B8A8Srgb);
            break;
            case vk::Format::eR8G8B8A8Srgb:
                imageFormats.push_back(vk::Format::eR8G8B8A8Unorm);
            break;
            case vk::Format::eB8G8R8A8Unorm:
                imageFormats.push_back(vk::Format::eB8G8R8A8Srgb);
            break;
            case vk::Format::eB8G8R8A8Srgb:
                imageFormats.push_back(vk::Format::eB8G8R8A8Unorm);
            break;
            default:
                break;
        }

        auto imageFormatListCreateInfo = vk::ImageFormatListCreateInfo()
            .setViewFormats(imageFormats);

        // TODO: depends on mutable swapchain. add check.
        desc.pNext = &imageFormatListCreateInfo;

        const vk::Result result = interface->GetVkDevice().createSwapchainKHR(&desc, nullptr, &vkSwapchain);
        if (result != vk::Result::eSuccess) {
            spdlog::error("Failed to create swapchain! Error: {}", nvrhi::vulkan::resultToString(VkResult(result)));
            return false;
        }

        auto images = interface->GetVkDevice().getSwapchainImagesKHR(vkSwapchain);
        for (auto image : images) {
            SwapChainImage sci;
            sci.image = image;

            nvrhi::TextureDesc textureDesc{};
            textureDesc.width = extent.width;
            textureDesc.height = extent.height;
            textureDesc.format = nvFormat;
            textureDesc.debugName = "Swapchain Image";
            textureDesc.initialState = nvrhi::ResourceStates::Present;
            textureDesc.keepInitialState = true;
            textureDesc.isRenderTarget = true;

            sci.rhiHandle = interface->GetDevice()->createHandleForNativeTexture(nvrhi::ObjectTypes::VK_Image, nvrhi::Object(sci.image), textureDesc);
            swapChainImages.push_back(sci);
        }

        swapchainIndex = 0;
        return true;
    }

    void VulkanWindow::DestroySwapchainInternal() {
        if (auto device = interface->GetVkDevice(); device) {
            device.waitIdle();

            if (vkSwapchain) {
                device.destroySwapchainKHR(vkSwapchain);
                vkSwapchain = nullptr;
            }
        }

        swapChainImages.clear();
    }
}
