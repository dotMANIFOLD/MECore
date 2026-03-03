//
// Created by ryen on 2/21/25.
//

#include <SDL3/SDL_vulkan.h>
#include <nvrhi/validation.h>
#include <spdlog/spdlog.h>

#include "MECore/MECore.h"
#include "MECore/Render/Vulkan/VulkanInterface.h"
#include "MECore/Render/Vulkan/VulkanWindow.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace ME {
    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    static std::vector<const char *> StringSetToVector(const std::unordered_set<std::string>& set) {
        std::vector<const char *> ret;
        for(const auto& s : set)
        {
            ret.push_back(s.c_str());
        }

        return ret;
    }

    template <typename T>
    static std::vector<T> SetToVector(const std::unordered_set<T>& set) {
        std::vector<T> ret;
        for(const auto& s : set)
        {
            ret.push_back(s);
        }

        return ret;
    }

    bool VulkanInterface::CreateInstance() {
        SDL_Vulkan_LoadLibrary(nullptr);

        auto vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr();
        if (vkGetInstanceProcAddr == nullptr) {
            spdlog::critical("Couldnt get procAddr: {0}", SDL_GetError());
            assert(false);
        }
        
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        Uint32 sdlReqCount = 0;
        auto* sdlRequirements = SDL_Vulkan_GetInstanceExtensions(&sdlReqCount);

        for (int i = 0; i < sdlReqCount; i++) {
            enabledExtensions.instance.insert(std::string(sdlRequirements[i]));
        }

        // Check required extensions
        auto requiredExtensions = enabledExtensions.instance;
        for (const auto& extension : vk::enumerateInstanceExtensionProperties()) {
            const std::string& name = extension.extensionName;
            if (optionalExtensions.instance.contains(name)) {
                enabledExtensions.instance.insert(name);
            }
            requiredExtensions.erase(name);
        }

        if (!requiredExtensions.empty()) {
            spdlog::error("Failed to create Vulkan Instance. Required extension(s) missing:");
            for (const auto& extension : requiredExtensions) {
                spdlog::error("- {}", extension);
            }
            return false;
        }

        spdlog::info("Enabled Vulkan instance extensions:");
        for (const auto& extension : enabledExtensions.instance) {
            spdlog::info("- {}", extension);
        }

        // Check required layers
        auto requiredLayers = enabledExtensions.layers;
        for (const auto& layer : vk::enumerateInstanceLayerProperties()) {
            const std::string name = layer.layerName;
            if (optionalExtensions.layers.contains(name)) {
                enabledExtensions.layers.insert(name);
            }
            requiredLayers.erase(name);
        }

        if (!requiredLayers.empty()) {
            spdlog::error("Failed to create Vulkan Instance. Required layer(s) missing:");
            for (const auto& layer : requiredLayers) {
                spdlog::error("- {}", layer);
            }
            return false;
        }

        spdlog::info("Enabled Vulkan layers: ");
        for (const auto& layer : enabledExtensions.layers) {
            spdlog::info("- {}", layer);
        }

        auto instanceExtVec = StringSetToVector(enabledExtensions.instance);
        auto instanceLayerVec = StringSetToVector(enabledExtensions.layers);

        // Create app info
        // TODO: add version checks
        vk::ApplicationInfo appInfo{};
        appInfo.pApplicationName = MEInstance::Instance()->GetAppData().name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: make this configurable
        appInfo.pEngineName = "MANIFOLDEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        vk::InstanceCreateInfo createInfo{};
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = instanceLayerVec.size();
        createInfo.ppEnabledLayerNames = instanceLayerVec.data();
        createInfo.enabledExtensionCount = instanceExtVec.size();
        createInfo.ppEnabledExtensionNames = instanceExtVec.data();

        vk::Result instResult = vk::createInstance(&createInfo, nullptr, &vkInstance);
        if (instResult != vk::Result::eSuccess) {
            spdlog::error("Failed to create Vulkan instance. Code: {}", nvrhi::vulkan::resultToString(VkResult(instResult)));
            return false;
        }

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkInstance);

        return true;
    }

    bool VulkanInterface::CreateDevice() {
        assert(PickPhysicalDevice());
        assert(FindQueueFamilies(vkPhysicalDevice));
        assert(CreateVulkanDevice());

        auto vecInstanceExt = StringSetToVector(enabledExtensions.instance);
        auto vecLayers = StringSetToVector(enabledExtensions.layers);
        auto vecDeviceExt = StringSetToVector(enabledExtensions.device);

        nvrhi::vulkan::DeviceDesc deviceDesc{};
        deviceDesc.errorCB = &DefaultMessageCallback::GetInstance();
        deviceDesc.instance = vkInstance;
        deviceDesc.physicalDevice = vkPhysicalDevice;
        deviceDesc.device = vkDevice;

        deviceDesc.graphicsQueue = graphicsQueue;
        deviceDesc.graphicsQueueIndex = graphicsQueueFamily;
        deviceDesc.computeQueue = computeQueue;
        deviceDesc.computeQueueIndex = computeQueueFamily;
        deviceDesc.transferQueue = transferQueue;
        deviceDesc.transferQueueIndex = transferQueueFamily;

        deviceDesc.instanceExtensions = vecInstanceExt.data();
        deviceDesc.numInstanceExtensions = vecInstanceExt.size();
        deviceDesc.deviceExtensions = vecDeviceExt.data();
        deviceDesc.numDeviceExtensions = vecDeviceExt.size();
        deviceDesc.bufferDeviceAddressSupported = true;

        nvDevice = nvrhi::vulkan::createDevice(deviceDesc);
        nvValidationLayer = nvrhi::validation::createValidationLayer(nvDevice);

        return true;
    }

    void VulkanInterface::DestroyDevice() {
        nvDevice = nullptr;
        nvValidationLayer = nullptr;

        if (vkDevice) {
            vkDevice.destroy();
            vkDevice = nullptr;
        }
        if (vkInstance) {
            vkInstance.destroy();
            vkInstance = nullptr;
        }
    }

    Window* VulkanInterface::CreateWindow(WindowParameters* params) {
        auto flags = SDL_WINDOW_VULKAN;
        if (params->borderless) flags |= SDL_WINDOW_BORDERLESS;
        if (params->resizeable) flags |= SDL_WINDOW_RESIZABLE;
        if (params->transparent) flags |= SDL_WINDOW_TRANSPARENT;
        auto window = SDL_CreateWindow(params->title.c_str(), params->width, params->height, flags);
        if (!window) {
            spdlog::error("Failed to create Vulkan window.");
            return nullptr;
        }

        VkSurfaceKHR surface;
        if (!SDL_Vulkan_CreateSurface(window, vkInstance, nullptr, &surface)) {
            SDL_DestroyWindow(window);
            spdlog::error("Failed to create Vulkan surface.");
            return nullptr;
        }

        auto windowWrapper = new VulkanWindow(params, this, window, surface);
        windows.insert(windowWrapper);
        return windowWrapper;
    }

    vk::Result VulkanInterface::QueuePresent(vk::PresentInfoKHR* info) {
        return presentQueue.presentKHR(info);
    }

    bool VulkanInterface::PickPhysicalDevice() {
        auto devices = vkInstance.enumeratePhysicalDevices();

        std::vector<vk::PhysicalDevice> discreteGPUs;
        std::vector<vk::PhysicalDevice> otherGPUs;

        // TODO: add more physical device checks
        for (auto device : devices) {
            vk::PhysicalDeviceProperties prop = device.getProperties();

            if (prop.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                discreteGPUs.push_back(device);
            } else {
                otherGPUs.push_back(device);
            }
        }

        if (!discreteGPUs.empty()) {
            vkPhysicalDevice = discreteGPUs[0];
            return true;
        }

        if (!otherGPUs.empty()) {
            vkPhysicalDevice = otherGPUs[0];
            return true;
        }

        return false;
    }

    bool VulkanInterface::FindQueueFamilies(vk::PhysicalDevice physicalDevice) {
        auto props = physicalDevice.getQueueFamilyProperties();

        // Look for dedicated queues
        for(int i = 0; i < props.size(); i++) {
            const auto& queueFamily = props[i];

            if (graphicsQueueFamily == -1) {
                if (queueFamily.queueCount > 0 &&
                    (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)) {
                    graphicsQueueFamily = i;
                }
            }

            if (computeQueueFamily == -1) {
                if (queueFamily.queueCount > 0 &&
                    (queueFamily.queueFlags & vk::QueueFlagBits::eCompute) &&
                    !(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)) {
                    computeQueueFamily = i;
                }
            }

            if (transferQueueFamily == -1) {
                if (queueFamily.queueCount > 0 &&
                    (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) &&
                    !(queueFamily.queueFlags & vk::QueueFlagBits::eCompute) &&
                    !(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)) {
                    transferQueueFamily = i;
                }
            }

            if (presentQueueFamily == -1) {
                if (queueFamily.queueCount > 0 &&
                    SDL_Vulkan_GetPresentationSupport(vkInstance, physicalDevice, i)) {
                    presentQueueFamily = i;
                }
            }
        }

        // Fallback for no dedicated transfer queues
        if (transferQueueFamily == -1) {
            for (int i = 0; i < props.size(); i++) {
                const auto& queueFamily = props[i];
                if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer)) {
                    transferQueueFamily = i;
                    break;
                }
            }
        }

        // no parameters yet
        if (graphicsQueueFamily == -1 ||
            (presentQueueFamily == -1) ||
            (computeQueueFamily == -1) ||
            (transferQueueFamily == -1)) {
            return false;
        }

        return true;
    }

    bool VulkanInterface::CreateVulkanDevice() {
        auto deviceExtensions = vkPhysicalDevice.enumerateDeviceExtensionProperties();
        for (const auto& extension : deviceExtensions) {
            const std::string name = extension.extensionName;
            if (optionalExtensions.device.contains(name)) {
                // This check happens when headless
                // if (name == VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME) continue;
                enabledExtensions.device.insert(name);
            }

            // TODO: enable ray tracing extensions here
        }

        // TODO: check if headless here
        enabledExtensions.device.insert(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        spdlog::info("Enabled Vulkan device extensions:");
        for (const auto& extension : enabledExtensions.device) {
            spdlog::info("- {}", extension);

            // TODO: check for device features (raytracing and stuff)
        }

        #define APPEND_EXTENSION(condition, desc) if (condition) { (desc).pNext = pNext; pNext = &(desc); }
        void* pNext = nullptr;

        vk::PhysicalDeviceFeatures2 physicalDeviceFeatures2;
        auto bufferDeviceAddressFeatures = vk::PhysicalDeviceBufferDeviceAddressFeatures();

        APPEND_EXTENSION(true, bufferDeviceAddressFeatures);

        physicalDeviceFeatures2.pNext = pNext;
        vkPhysicalDevice.getFeatures2(&physicalDeviceFeatures2);

        // TODO: graphics should be the only default. check for compute queue, transfer queue, and headless mode in the future
        std::unordered_set<uint32_t> uniqueQueueFamilies = { graphicsQueueFamily, computeQueueFamily, transferQueueFamily, presentQueueFamily };

        float priority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queueDesc;
        queueDesc.reserve(uniqueQueueFamilies.size());
        for (int family : uniqueQueueFamilies) {
            queueDesc.push_back(vk::DeviceQueueCreateInfo()
                .setQueueFamilyIndex(family)
                .setQueueCount(1)
                .setPQueuePriorities(&priority));
        }

        // TODO: in donut there is a giant features check here, i only have vk1.3 here

        auto vk13Features = vk::PhysicalDeviceVulkan13Features()
            .setDynamicRendering(true)
            .setSynchronization2(true)
            .setMaintenance4(true);

        pNext = nullptr;
        // TODO: check if api is at least 1.3
        APPEND_EXTENSION(true,  vk13Features);

        #undef APPEND_EXTENSION

        // VULKAN 1.0 - 1.2 FEATURES

        auto deviceFeatures = vk::PhysicalDeviceFeatures()
            .setShaderImageGatherExtended(true)
            .setSamplerAnisotropy(true)
            .setTessellationShader(true)
            .setTextureCompressionBC(true)
            .setGeometryShader(true)
            .setImageCubeArray(true)
            .setShaderInt16(true)
            .setFillModeNonSolid(true)
            .setFragmentStoresAndAtomics(true)
            .setDualSrcBlend(true)
            .setVertexPipelineStoresAndAtomics(true)
            .setShaderInt64(true)
            .setShaderStorageImageWriteWithoutFormat(true)
            .setShaderStorageImageReadWithoutFormat(true);

        auto vk11Features = vk::PhysicalDeviceVulkan11Features()
            .setStorageBuffer16BitAccess(true)
            .setPNext(pNext);

        auto vk12Features = vk::PhysicalDeviceVulkan12Features()
            .setDescriptorIndexing(true)
            .setRuntimeDescriptorArray(true)
            .setDescriptorBindingPartiallyBound(true)
            .setDescriptorBindingVariableDescriptorCount(true)
            .setTimelineSemaphore(true)
            .setShaderSampledImageArrayNonUniformIndexing(true)
            .setBufferDeviceAddress(true) // TODO: keep track of buffer device address
            .setShaderSubgroupExtendedTypes(true)
            .setScalarBlockLayout(true)
            .setPNext(&vk11Features);

        auto layerVec = StringSetToVector(enabledExtensions.layers);
        auto deviceVec = StringSetToVector(enabledExtensions.device);

        auto deviceDesc = vk::DeviceCreateInfo()
            .setPQueueCreateInfos(queueDesc.data())
            .setQueueCreateInfoCount(queueDesc.size())
            .setPEnabledFeatures(&deviceFeatures)
            .setEnabledExtensionCount(deviceVec.size())
            .setPpEnabledExtensionNames(deviceVec.data())
            .setEnabledLayerCount(layerVec.size())
            .setPpEnabledLayerNames(layerVec.data())
            .setPNext(&vk12Features);

        // Some weird info callback would happen here in donut

        vk::Result result = vkPhysicalDevice.createDevice(&deviceDesc, nullptr, &vkDevice);
        if (result != vk::Result::eSuccess) {
            spdlog::error("Failed to create logical device! Error Code: {}", nvrhi::vulkan::resultToString(VkResult(result)));
            return false;
        }

        vkDevice.getQueue(graphicsQueueFamily, 0, &graphicsQueue);
        vkDevice.getQueue(computeQueueFamily, 0, &computeQueue);
        vkDevice.getQueue(transferQueueFamily, 0, &transferQueue);
        vkDevice.getQueue(presentQueueFamily, 0, &presentQueue);

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkDevice);

        // something about bufferDeviceAddress

        spdlog::info("Vulkan device created");
        return true;
    }
}
