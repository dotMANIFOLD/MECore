//
// Created by ryen on 2/19/25.
//

#pragma once

#include <memory>

#include "../Mesh/Mesh.h"
#include "../Material/Material.h"

namespace ME {
    /// A model can have multiple meshes, default materials, and a skeleton.
    class Model {
        public:
        using Pointer = std::shared_ptr<Model>;
        
        std::vector<Mesh::Pointer> meshes;
        // For now, materials should match the size of meshes. This will be changed later.
        std::vector<std::shared_ptr<Material>> materials;

        Model() = default;
    };
}
