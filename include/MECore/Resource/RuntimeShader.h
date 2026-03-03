//
// Created by ryen on 2/26/25.
//

#pragma once

#include <nvrhi/nvrhi.h>

namespace ME {
    class Shader {
        private:
        nvrhi::ShaderHandle shader;
        nvrhi::BindingLayoutHandle bindingLayout;
        nvrhi::InputLayoutHandle inputLayout;

        public:
        nvrhi::ShaderType type;
        // only needed for vertex shaders
        std::vector<nvrhi::VertexAttributeDesc> vertexAttributes;
        nvrhi::BindingLayoutDesc bindings;
        // doesnt need to persist, only needed for creation
        std::vector<uint8_t> bytecode;

        Shader() = default;

        nvrhi::ShaderHandle GetGPUShader() const { return shader; }
        nvrhi::BindingLayoutHandle GetBindingLayout() const { return bindingLayout; }
        nvrhi::InputLayoutHandle GetInputLayout() const { return inputLayout; }

        bool CreateGPUShader();
    };
}
