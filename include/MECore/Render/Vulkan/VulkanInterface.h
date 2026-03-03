//
// Created by ryen on 2/21/25.
//

// A lot of this code is derived/copied from NVIDIA's Donut.
// Donut is licensed under the MIT License.

#pragma once

#include <unordered_set>
#include <queue>

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif
#include <vulkan/vulkan.hpp>
#include <nvrhi/vulkan.h>

#include "../IRenderInterface.h"

namespace ME {
    class VulkanWindow;
    class VulkanInterface : public IRenderInterface {
        protected:
        struct VulkanExtensionSet {
            std::unordered_set<std::string> instance;
            std::unordered_set<std::string> layers;
            std::unordered_set<std::string> device;
        };

        VulkanExtensionSet enabledExtensions = {
            {
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME
            },
            {
                "VK_LAYER_KHRONOS_validation"
            },
            {
                
            }
        };

        VulkanExtensionSet optionalExtensions = {
            {
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME,
            },
            { },
            {
                VK_EXT_DEBUG_MARKER_EXTENSION_NAME,
                VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
                VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
                VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
                VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
                VK_EXT_MESH_SHADER_EXTENSION_NAME,
                VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
            }
        };

        // Vulkan Device
        vk::Instance vkInstance;
        vk::PhysicalDevice vkPhysicalDevice;
        uint32_t graphicsQueueFamily = -1;
        uint32_t computeQueueFamily = -1;
        uint32_t transferQueueFamily = -1;
        uint32_t presentQueueFamily = -1;

        vk::Device vkDevice;
        vk::Queue graphicsQueue;
        vk::Queue computeQueue;
        vk::Queue transferQueue;
        vk::Queue presentQueue;

        // Windows
        std::unordered_set<VulkanWindow*> windows;

        // NVRHI
        nvrhi::vulkan::DeviceHandle nvDevice;
        nvrhi::DeviceHandle nvValidationLayer;

        bool PickPhysicalDevice();
        bool FindQueueFamilies(vk::PhysicalDevice physicalDevice);
        bool CreateVulkanDevice();

        public:
        VulkanInterface() = default;
        ~VulkanInterface() override = default;

        nvrhi::IDevice* GetDevice() override {
            if (nvValidationLayer) return nvValidationLayer;
            return nvDevice;
        }
        nvrhi::vulkan::DeviceHandle GetNvDevice() { return nvDevice; }
        vk::Instance GetVkInstance() { return vkInstance; }
        vk::PhysicalDevice GetVkPhysicalDevice() { return vkPhysicalDevice; }
        vk::Device GetVkDevice() { return vkDevice; }

        int GetGraphicsQueueFamily() const { return graphicsQueueFamily; }
        int GetPresentQueueFamily() const { return presentQueueFamily; }

        bool CreateInstance() override;
        bool CreateDevice() override;
        void DestroyDevice() override;

        Window* CreateWindow(WindowParameters* params) override;

        vk::Result QueuePresent(vk::PresentInfoKHR* info);
    };
}
