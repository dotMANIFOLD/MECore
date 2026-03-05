//
// Created by dotryen on 12/18/25.
//

#pragma once

#include "MeshStreamTemplate.h"

namespace ME {
    class IndexMeshStream final : public MeshStreamTemplate<uint32_t> {
        public:
        IndexMeshStream() : MeshStreamTemplate("INDEX") { }
        explicit IndexMeshStream(const std::string& name) : MeshStreamTemplate(name, "INDEX") { }

        MeshStreamType GetStreamType() const override { return MeshStreamType::Index; }
        VALUE_TYPE GetFormat() const override { return VT_UINT32; }
    };
}
