//
// Created by dotryen on 12/18/25.
//

#pragma once
#include "MeshStreamTemplate.h"
#include "MECore/Math/Types.h"
#include "nvrhi/nvrhi.h"

namespace ME {
    class PositionMeshStream final : public MeshStreamTemplate<PackedVector3> {
        public:
        PositionMeshStream() : MeshStreamTemplate("POSITION") { }
        PositionMeshStream(const std::string& name) : MeshStreamTemplate(name, "POSITION") { }

        MeshStreamType GetStreamType() const override { return MeshStreamType::Vertex; }
        nvrhi::Format GetFormat() const override { return nvrhi::Format::RGB32_FLOAT; }
    };
}
