//
// Created by dotryen on 3/2/26.
//

#pragma once

#include "MECore/MECore.h"

namespace ME {
    template<nvrhi::ShaderType type>
    void MaterialStage<type>::SetShader(Shader::Pointer shader) {
        if (shader->GetType() != type) {
            spdlog::error("Shader type isnt matching");
            return;
        }
        
        this->shader = shader;
        propertyHelper.InitBlock(shader->GetProperties());
        propertyHelper.InitBindings(shader->GetProperties());
    }
    
    template<nvrhi::ShaderType type>
    MaterialStage<type> MaterialStage<type>::Clone() {
        MaterialStage clone;
        clone.SetShader(shader);
        propertyHelper.CopyTo(clone.propertyHelper);
        return std::move(clone);
    }
}
