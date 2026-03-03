//
// Created by dotryen on 3/2/26.
//

#include "MECore/Resource/Shader/PropertyHelper.h"

#include "MECore/MECore.h"
#include "MECore/Resource/Shader/Shader.h"

namespace ME {
    void PropertyHelper::InitBlock(const PropertySet& set) {
        blockDirty = false;
        block = nullptr;
        blockBuffer = nullptr;
        blockSize = set.blockSize;
        
        auto device = MEInstance::Instance()->GetServiceProvider()->GetRenderService()->GetInterface()->GetDevice();
        if (set.blockSize > 0) {
            auto desc = nvrhi::BufferDesc()
                .setByteSize(set.blockSize)
                .setIsConstantBuffer(true)
                .setInitialState(nvrhi::ResourceStates::ConstantBuffer)
                .setKeepInitialState(true);
            
            block = std::unique_ptr<char>(new char[desc.byteSize]);
            blockBuffer = device->createBuffer(desc);
            blockDirty = true;
        }
    }

    void PropertyHelper::InitBindings(const PropertySet& set) {
        bindings.clear();
        bindingSet = nullptr;
        
        bindings.reserve(set.bindings.size() + 1);
        if (block) {
            bindings.push_back(nvrhi::BindingSetItem::ConstantBuffer(0, blockBuffer));
        }
        for (const auto& [key, prop] : set.bindings) {
            bindings.emplace_back();
        }
        bindingsDirty = true;
    }

    void PropertyHelper::SetBlockProperty(const PropertySet& set, const std::string& name, const void* data, int dataSize) {
        if (!set.block.contains(name)) return;
        const auto& property = set.block.at(name);
        if (dataSize != property.length) return;
        auto ptr = block.get() + property.offset;
        memcpy(ptr, data, dataSize);
        
        blockDirty = true;
    }

    void PropertyHelper::UpdateBlock(nvrhi::ICommandList* cmd) {
        if (!blockBuffer) return;
        if (!blockDirty) return;
        
        cmd->writeBuffer(blockBuffer, block.get(), blockSize);
    }

    void PropertyHelper::UpdateBindingSet(nvrhi::BindingLayoutHandle layout) {
        if (!bindingsDirty) return;
        
        auto device = MEInstance::Instance()->GetServiceProvider()->GetRenderService()->GetInterface()->GetDevice();
        auto desc = nvrhi::BindingSetDesc();
        desc.bindings = bindings;
        
        bindingSet = device->createBindingSet(desc, layout);
    }

    void PropertyHelper::CopyTo(PropertyHelper& other) {
        if (other.block && other.blockSize == blockSize) {
            memcpy(other.block.get(), block.get(), blockSize);
            other.blockDirty = true;
        }
        other.bindings = bindings;
        other.bindingsDirty = true;
    }
}
