//
// Created by dotryen on 12/19/25.
//

#pragma once
#include <memory>

#include "Mesh.h"

namespace ME {
    namespace PrimitiveMesh {
        std::shared_ptr<Mesh> Cube();
        std::shared_ptr<Mesh> Plane();
    }
}
