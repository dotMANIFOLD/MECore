//
// Created by dotryen on 12/18/25.
//

#pragma once
#include "MeshStreamTemplate.h"
#include "MECore/Math/Types.h"

namespace ME {
    class PositionMeshStream final : public MeshStreamTemplate<PackedVector3> {
        public:
        PositionMeshStream() : MeshStreamTemplate("POSITION") { }
        explicit PositionMeshStream(const std::string& name) : MeshStreamTemplate(name, "POSITION") { }

        MeshStreamType GetStreamType() const override { return MeshStreamType::Vertex; }
        VALUE_TYPE GetFormat() const override { return VT_FLOAT32; }
    };
}
