//
// Created by ryen on 2/26/25.
//

#pragma once

#include <memory>
#include <unordered_map>
#include <nvrhi/nvrhi.h>

#include "MECore/Resource/Resource.h"
#include "PropertyHelper.h"

namespace ME {
    struct BlockProperty {
        unsigned int length;
        unsigned int offset;
    };
    
    struct BindingProperty {
        int index;
        nvrhi::ResourceType type;
        uint32_t slot;
    };

    struct PropertySet {
        std::unordered_map<std::string, BlockProperty> block;
        unsigned int blockSize;
        std::unordered_map<std::string, BindingProperty> bindings;
    };
    
    struct ShaderResources {
        nvrhi::ShaderHandle shader;
        nvrhi::InputLayoutHandle inputLayout;
        nvrhi::BindingLayoutHandle instBindingLayout;
        nvrhi::BindingLayoutHandle globalBindingLayout;
    };
    
    class Shader : public Resource, std::enable_shared_from_this<Shader> {
        private:
        nvrhi::ShaderType type;
        ShaderResources resources;
        PropertySet properties;
        PropertySet globalProperties;

        PropertyHelper globalPropertiesHelper;
        
        public:
        using Pointer = std::shared_ptr<Shader>;
        
        Shader() = default;
        Shader(const nvrhi::ShaderType type, const ShaderResources& resources, const PropertySet& properties, const PropertySet& globalProperties) {
            this->type = type;
            this->resources = resources;
            this->properties = properties;
            this->globalProperties = globalProperties;
            
            globalPropertiesHelper.InitBlock(globalProperties);
            globalPropertiesHelper.InitBindings(globalProperties);
        }

        nvrhi::ShaderType GetType() const { return type; }
        const ShaderResources& GetResources() const { return resources; }
        const PropertySet& GetProperties() const { return properties; }
        const PropertySet& GetGlobalProperties() const { return globalProperties; } 
        
        nvrhi::BufferHandle GetGlobalBlockBuffer() const { return globalPropertiesHelper.GetBlockBuffer(); }
        nvrhi::BindingSetHandle GetGlobalBindingSet() const { return globalPropertiesHelper.GetBindingSet(); }
        
        void SetGlobalBlockProperty(const std::string& name, const void* data, int dataSize);
        
        void UpdateBlockBuffer(nvrhi::ICommandList* cmd) { globalPropertiesHelper.UpdateBlock(cmd); }
        void UpdateBindingSet() { globalPropertiesHelper.UpdateBindingSet(resources.globalBindingLayout); }
    };
}
