//
// Created by dotryen on 3/2/26.
//

#include <GraphicsUtilities.h>

#include "MECore/Resource/Shader/PropertyHelper.h"

#include "MapHelper.hpp"
#include "MECore/MECore.h"
#include "MECore/Resource/Shader/Shader.h"

namespace ME {
    void PropertyHelper::InitBlock(const PropertySet& set) {
        blockDirty = false;
        block = nullptr;
        blockBuffer = nullptr;
        blockSize = set.blockSize;
        
        auto device = MEInstance::Instance()->GetServiceProvider()->GetRenderInterface()->GetDevice();
        if (set.blockSize > 0) {
            block = std::unique_ptr<char>(new char[blockSize]);
            auto ptr = blockBuffer.get();
            CreateUniformBuffer(device, blockSize, "ShaderBlockBuffer", &ptr);
            blockBuffer = std::shared_ptr<IBuffer>(ptr);
            blockDirty = true;
        }
    }

    void PropertyHelper::InitBindings(const PropertySet& set) {
        // bindings.clear();
        // bindingSet = nullptr;
        // 
        // bindings.reserve(set.bindings.size() + 1);
        // if (block) {
        //     bindings.push_back(nvrhi::BindingSetItem::ConstantBuffer(0, blockBuffer));
        // }
        // for (const auto& [key, prop] : set.bindings) {
        //     bindings.emplace_back();
        // }
        // bindingsDirty = true;
    }

    void PropertyHelper::SetBlockProperty(const PropertySet& set, const std::string& name, const void* data, int dataSize) {
        if (!set.block.contains(name)) return;
        const auto& property = set.block.at(name);
        if (dataSize != property.length) return;
        auto ptr = block.get() + property.offset;
        memcpy(ptr, data, dataSize);
        
        blockDirty = true;
    }

    void PropertyHelper::UpdateBlock(IDeviceContext* ctx) {
        if (!blockBuffer) return;
        if (!blockDirty) return;
        
        void* map;
        ctx->MapBuffer(blockBuffer.get(), MAP_WRITE, MAP_FLAG_DISCARD, map);
        memcpy(map, block.get(), blockSize);
        ctx->UnmapBuffer(blockBuffer.get(), MAP_WRITE);
    }

    // void PropertyHelper::UpdateBindingSet(nvrhi::BindingLayoutHandle layout) {
    //     if (!bindingsDirty) return;
    //     
    //     auto device = MEInstance::Instance()->GetServiceProvider()->GetRenderService()->GetInterface()->GetDevice();
    //     auto desc = nvrhi::BindingSetDesc();
    //     desc.bindings = bindings;
    //     
    //     bindingSet = device->createBindingSet(desc, layout);
    // }

    void PropertyHelper::CopyTo(PropertyHelper& other) {
        if (other.block && other.blockSize == blockSize) {
            memcpy(other.block.get(), block.get(), blockSize);
            other.blockDirty = true;
        }
        // other.bindings = bindings;
        other.bindingsDirty = true;
    }
}
