//
// Created by dotryen on 3/2/26.
//

#pragma once

#include <memory>
#include <Buffer.h>
#include <DeviceContext.h>
#include <unordered_map>

using namespace Diligent;

namespace ME {
    class Shader;
    struct PropertySet;
    
    class PropertyHelper {
        private:
        bool blockDirty;
        std::unique_ptr<char> block = nullptr;
        std::shared_ptr<IBuffer> blockBuffer = nullptr;
        unsigned int blockSize;
        
        bool bindingsDirty;
        // std::vector<nvrhi::BindingSetItem> bindings;
        // nvrhi::BindingSetHandle bindingSet;
        // TODO: bring back bindings
        
        public:
        PropertyHelper() = default;
        
        bool IsBlockDirty() const { return blockDirty; }
        bool IsBindingsDirty() const { return bindingsDirty; }
        
        std::shared_ptr<IBuffer> GetBlockBuffer() const { return blockBuffer; }
        // nvrhi::BindingSetHandle GetBindingSet() const { return bindingSet; }
        
        void InitBlock(const PropertySet& set);
        void InitBindings(const PropertySet& set);
        
        void SetBlockProperty(const PropertySet& set, const std::string& name, const void* data, int dataSize);
        // void SetBinding(const PropertySet& set, const std::string& name, nvrhi::ResourceHandle resource);
        
        void UpdateBlock(IDeviceContext* ctx);
        // void UpdateBindingSet(nvrhi::BindingLayoutHandle layout);
        
        void CopyTo(PropertyHelper& other);
    };
}
