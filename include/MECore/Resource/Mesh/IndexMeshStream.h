//
// Created by dotryen on 12/18/25.
//

#pragma once

#include "MeshStreamTemplate.h"

namespace ME {
    class IndexMeshStream final : public MeshStreamTemplate<uint32_t> {
        public:
        explicit IndexMeshStream() : MeshStreamTemplate("INDEX") { }
        IndexMeshStream(const std::string& name) : MeshStreamTemplate(name, "INDEX") { }

        MeshStreamType GetStreamType() const override { return MeshStreamType::Index; }
        nvrhi::Format GetFormat() const override { return nvrhi::Format::R32_UINT; }
    };
}
