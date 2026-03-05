//
// Created by ryen on 2/26/25.
//

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <Shader.h>
#include <InputLayout.h>
#include <RefCntAutoPtr.hpp>
#include <PipelineState.h>

#include "MECore/Resource/Resource.h"
#include "PropertyHelper.h"

using namespace Diligent;

namespace ME {
    struct BlockProperty {
        unsigned int length;
        unsigned int offset;
    };
    
    struct BindingProperty {
        int index;
        SHADER_RESOURCE_TYPE type;
        uint32_t slot;
    };

    struct PropertySet {
        std::unordered_map<std::string, BlockProperty> block;
        unsigned int blockSize;
        std::unordered_map<std::string, BindingProperty> bindings;
    };
    
    struct ShaderResources {
        RefCntAutoPtr<IShader> shader;
        std::vector<LayoutElement> inputLayout;
        std::vector<ShaderResourceVariableDesc> bindings;
    };
    
    class Shader : public Resource, std::enable_shared_from_this<Shader> {
        private:
        SHADER_TYPE type;
        ShaderResources resources;
        PropertySet properties;
        PropertySet globalProperties;

        PropertyHelper globalPropertiesHelper;
        
        public:
        using Pointer = std::shared_ptr<Shader>;
        
        Shader() = default;
        Shader(const SHADER_TYPE type, const ShaderResources& resources, const PropertySet& properties, const PropertySet& globalProperties) {
            this->type = type;
            this->resources = resources;
            this->properties = properties;
            this->globalProperties = globalProperties;
            
            globalPropertiesHelper.InitBlock(globalProperties);
            globalPropertiesHelper.InitBindings(globalProperties);
        }

        SHADER_TYPE GetType() const { return type; }
        const ShaderResources& GetResources() const { return resources; }
        const PropertySet& GetProperties() const { return properties; }
        const PropertySet& GetGlobalProperties() const { return globalProperties; } 
        
        std::shared_ptr<IBuffer> GetGlobalBlockBuffer() const { return globalPropertiesHelper.GetBlockBuffer(); }
        // nvrhi::BindingSetHandle GetGlobalBindingSet() const { return globalPropertiesHelper.GetBindingSet(); }
        
        void SetGlobalBlockProperty(const std::string& name, const void* data, int dataSize);
        
        void UpdateBlockBuffer(IDeviceContext* ctx) { globalPropertiesHelper.UpdateBlock(ctx); }
        // void UpdateBindingSet() { globalPropertiesHelper.UpdateBindingSet(resources.globalBindingLayout); }
    };
}
