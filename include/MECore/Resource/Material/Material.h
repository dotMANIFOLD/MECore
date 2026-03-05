//
// Created by ryen on 2/26/25.
//

#pragma once

#include <memory>

#include "MECore/Resource/Resource.h"
#include "MECore/Resource/Shader/Shader.h"

using namespace Diligent;

namespace ME {
    struct MaterialDrawSettings {
        CULL_MODE culling = CULL_MODE_BACK;
        COLOR_MASK colorMask = COLOR_MASK_ALL;
    };

    struct MaterialBlendSettings {
        // TODO: add blending settings
    };

    struct MaterialDepthSettings {
        bool testEnabled;
        bool writeEnabled;
        COMPARISON_FUNCTION comparison;

        static constexpr MaterialDepthSettings Opaque() {
            return MaterialDepthSettings{ true, true, COMPARISON_FUNC_LESS_EQUAL };
        }
        static constexpr MaterialDepthSettings Transparent() {
            return MaterialDepthSettings{ true, false, COMPARISON_FUNC_LESS_EQUAL };
        }
    };

    struct MaterialStencilSettings {
        bool enabled = false;
        StencilOpDesc front;
        StencilOpDesc back;
    };

    class MaterialStageBase {
        protected:
        Shader::Pointer shader;
        PropertyHelper propertyHelper;
        
        public:
        MaterialStageBase() = default;
        
        Shader::Pointer GetShader() const { return shader; }
        std::shared_ptr<IBuffer> GetBlockBuffer() const { return propertyHelper.GetBlockBuffer(); }
        // nvrhi::BindingSetHandle GetBindingSet() const { return propertyHelper.GetBindingSet(); }
        
        bool IsBlockDirty() const { return propertyHelper.IsBlockDirty(); }
        bool IsBindingsDirty() const { return propertyHelper.IsBindingsDirty(); }
        
        void SetBlockProperty(const std::string& name, const void* data, const int dataSize) { propertyHelper.SetBlockProperty(shader->GetProperties(), name, data, dataSize); }
        
        void UpdateBlockBuffer(IDeviceContext* cmd) { propertyHelper.UpdateBlock(cmd); shader->UpdateBlockBuffer(cmd); }
        // void UpdateBindings() { propertyHelper.UpdateBindingSet(shader->GetResources().instBindingLayout); shader->UpdateBindingSet(); }
    };
    
    template<SHADER_TYPE type>
    class MaterialStage : public MaterialStageBase {
        public:
        MaterialStage() = default;
        
        void SetShader(Shader::Pointer shader);
        
        MaterialStage Clone();
    };
    
    class Material : public Resource, public std::enable_shared_from_this<Material> {
        private:
        MaterialStage<SHADER_TYPE_VERTEX> vertex;
        MaterialStage<SHADER_TYPE_PIXEL> pixel;

        MaterialDrawSettings drawSettings;
        MaterialDepthSettings depthSettings;
        MaterialStencilSettings stencilSettings;
        bool settingsDirty;

        public:
        using Pointer = std::shared_ptr<Material>;
        using WeakPointer = std::weak_ptr<Material>;
        
        Material() = default;

        static constexpr Material Opaque() {
            auto material = Material();
            material.depthSettings = MaterialDepthSettings::Opaque();
            return material;
        }

        static constexpr Material Transparent() {
            auto material = Material();
            material.depthSettings = MaterialDepthSettings::Transparent();
            return material;
        }

        MaterialStage<SHADER_TYPE_VERTEX>& GetVertexStage() { return vertex; }
        const MaterialStage<SHADER_TYPE_VERTEX>& GetVertexStage() const { return vertex; }
        MaterialStage<SHADER_TYPE_PIXEL>& GetPixelStage() { return pixel; }
        const MaterialStage<SHADER_TYPE_PIXEL>& GetPixelStage() const { return pixel; }

        const MaterialDrawSettings& GetDrawSettings() const { return drawSettings; }
        const MaterialDepthSettings& GetDepthSettings() const { return depthSettings; }
        const MaterialStencilSettings& GetStencilSettings() const { return stencilSettings; }

        // std::vector<nvrhi::BindingSetHandle> GetAllBindingSets();
        
        bool IsValid() const override { return vertex.GetShader().use_count() > 0 && pixel.GetShader().use_count() > 0; }
        bool IsSettingsDirty() const { return settingsDirty; }
        void ClearDirtySettings() { settingsDirty = false; }

        void SetDrawSettings(const MaterialDrawSettings& drawSettings) {
            this->drawSettings = drawSettings;
            settingsDirty = true;
        }
        void SetDepthSettings(const MaterialDepthSettings& depthSettings) {
            this->depthSettings = depthSettings;
            settingsDirty = true;
        }
        void SetStencilSettings(const MaterialStencilSettings& stencilSettings) {
            this->stencilSettings = stencilSettings;
            settingsDirty = true;
        }
        
        void UpdateBlocks(IDeviceContext* cmd);
        void UpdateBindings();
        
        Pointer Clone();
    };
}

#include "MaterialImpl.inl"
