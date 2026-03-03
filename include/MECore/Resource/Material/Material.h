//
// Created by ryen on 2/26/25.
//

#pragma once

#include <memory>
#include <nvrhi/nvrhi.h>

#include "MECore/Resource/Resource.h"
#include "MECore/Resource/Shader/Shader.h"

namespace ME {
    enum class ColorMask : uint8_t {
        None = 0,
        Red = 1,
        Green = 2,
        Blue = 4,
        Alpha = 8,
        All = 0xF
    };

    struct MaterialDrawSettings {
        nvrhi::RasterCullMode culling = nvrhi::RasterCullMode::Back;
        ColorMask colorMask = ColorMask::All;
    };

    struct MaterialBlendSettings {
        // TODO: add blending settings
    };

    struct MaterialDepthSettings {
        bool testEnabled;
        bool writeEnabled;
        nvrhi::ComparisonFunc comparison;

        static constexpr MaterialDepthSettings Opaque() {
            return MaterialDepthSettings{ true, true, nvrhi::ComparisonFunc::LessOrEqual };
        }
        static constexpr MaterialDepthSettings Transparent() {
            return MaterialDepthSettings{ true, false, nvrhi::ComparisonFunc::LessOrEqual };
        }
    };

    struct MaterialStencilSettings {
        bool enabled = false;
        nvrhi::DepthStencilState::StencilOpDesc front;
        nvrhi::DepthStencilState::StencilOpDesc back;
    };

    class MaterialStageBase {
        protected:
        Shader::Pointer shader;
        PropertyHelper propertyHelper;
        
        public:
        MaterialStageBase() = default;
        
        Shader::Pointer GetShader() const { return shader; }
        nvrhi::BufferHandle GetBlockBuffer() const { return propertyHelper.GetBlockBuffer(); }
        nvrhi::BindingSetHandle GetBindingSet() const { return propertyHelper.GetBindingSet(); }
        
        bool IsBlockDirty() const { return propertyHelper.IsBlockDirty(); }
        bool IsBindingsDirty() const { return propertyHelper.IsBindingsDirty(); }
        
        void SetBlockProperty(const std::string& name, const void* data, const int dataSize) { propertyHelper.SetBlockProperty(shader->GetProperties(), name, data, dataSize); }
        
        void UpdateBlockBuffer(nvrhi::ICommandList* cmd) { propertyHelper.UpdateBlock(cmd); shader->UpdateBlockBuffer(cmd); }
        void UpdateBindings() { propertyHelper.UpdateBindingSet(shader->GetResources().instBindingLayout); shader->UpdateBindingSet(); }
    };
    
    template<nvrhi::ShaderType type>
    class MaterialStage : public MaterialStageBase {
        public:
        MaterialStage() = default;
        
        void SetShader(Shader::Pointer shader);
        
        MaterialStage Clone();
    };
    
    class Material : public Resource, public std::enable_shared_from_this<Material> {
        private:
        MaterialStage<nvrhi::ShaderType::Vertex> vertex;
        MaterialStage<nvrhi::ShaderType::Pixel> pixel;

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

        MaterialStage<nvrhi::ShaderType::Vertex>& GetVertexStage() { return vertex; }
        const MaterialStage<nvrhi::ShaderType::Vertex>& GetVertexStage() const { return vertex; }
        MaterialStage<nvrhi::ShaderType::Pixel>& GetPixelStage() { return pixel; }
        const MaterialStage<nvrhi::ShaderType::Pixel>& GetPixelStage() const { return pixel; }

        const MaterialDrawSettings& GetDrawSettings() const { return drawSettings; }
        const MaterialDepthSettings& GetDepthSettings() const { return depthSettings; }
        const MaterialStencilSettings& GetStencilSettings() const { return stencilSettings; }

        std::vector<nvrhi::BindingSetHandle> GetAllBindingSets();
        
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
        
        void UpdateResources();
        void UpdateBuffers(nvrhi::ICommandList* cmd);
        
        Pointer Clone();
    };
}

#include "MaterialImpl.inl"
