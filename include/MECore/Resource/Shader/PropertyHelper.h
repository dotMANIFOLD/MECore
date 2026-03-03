//
// Created by dotryen on 3/2/26.
//

#pragma once

#include <memory>
#include <nvrhi/nvrhi.h>

namespace ME {
    class Shader;
    struct PropertySet;
    
    class PropertyHelper {
        private:
        bool blockDirty;
        std::unique_ptr<char> block;
        nvrhi::BufferHandle blockBuffer;
        unsigned int blockSize;
        
        bool bindingsDirty;
        std::vector<nvrhi::BindingSetItem> bindings;
        nvrhi::BindingSetHandle bindingSet;
        
        public:
        PropertyHelper() = default;
        
        bool IsBlockDirty() const { return blockDirty; }
        bool IsBindingsDirty() const { return bindingsDirty; }
        
        nvrhi::BufferHandle GetBlockBuffer() const { return blockBuffer; }
        nvrhi::BindingSetHandle GetBindingSet() const { return bindingSet; }
        
        void InitBlock(const PropertySet& set);
        void InitBindings(const PropertySet& set);
        
        void SetBlockProperty(const PropertySet& set, const std::string& name, const void* data, int dataSize);
        // void SetBinding(const PropertySet& set, const std::string& name, nvrhi::ResourceHandle resource);
        
        void UpdateBlock(nvrhi::ICommandList* cmd);
        void UpdateBindingSet(nvrhi::BindingLayoutHandle layout);
        
        void CopyTo(PropertyHelper& other);
    };
}
