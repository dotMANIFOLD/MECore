//
// Created by ryen on 3/1/25.
//

#include <spdlog/spdlog.h>

#include "MECore/Resource/Material/Material.h"

#include "MECore/MECore.h"
#include "MECore/Render/IRenderInterface.h"

namespace ME {
    std::vector<nvrhi::BindingSetHandle> Material::GetAllBindingSets() {
        std::vector<nvrhi::BindingSetHandle> sets;
        
        #define ADD_SET(stage) \
            if (auto set = stage.GetBindingSet(); set != nullptr) { sets.push_back(set); } \
            if (auto set = stage.GetShader()->GetGlobalBindingSet(); set != nullptr) { sets.push_back(set); } \
        
        ADD_SET(vertex)
        ADD_SET(pixel)
        
        #undef ADD_SET
        return sets;
    }

    void Material::UpdateResources() {
        if (vertex.IsBindingsDirty()) vertex.UpdateBindings();
        if (pixel.IsBindingsDirty()) pixel.UpdateBindings();
    }

    void Material::UpdateBuffers(nvrhi::ICommandList* cmd) {
        if (vertex.IsBlockDirty()) vertex.UpdateBlockBuffer(cmd);
        if (pixel.IsBlockDirty()) vertex.UpdateBlockBuffer(cmd);
    }
    
    Material::Pointer Material::Clone() {
        auto clone = std::make_shared<Material>();
        clone->vertex = vertex.Clone();
        clone->pixel = pixel.Clone();
        clone->drawSettings = drawSettings;
        clone->depthSettings = depthSettings;
        clone->stencilSettings = stencilSettings;
        return clone;
    }
}
