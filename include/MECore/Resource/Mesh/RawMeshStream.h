//
// Created by dotryen on 12/18/25.
//

#pragma once

#include <GraphicsAccessories.hpp>

#include "IMeshStream.h"

namespace ME {
    class RawMeshStream final : public IMeshStream {
        private:
        std::string name;
        std::string semantic;
        MeshStreamType streamType;
        VALUE_TYPE format;

        public:
        void* data;
        size_t size;
        int components;
        bool isDirty;

        RawMeshStream(const std::string& name, const std::string& semantic, MeshStreamType type, VALUE_TYPE format) {
            this->name = name;
            this->semantic = semantic;
            this->streamType = type;
            this->format = format;

            data = nullptr;
            size = 0;
            components = 1;
            isDirty = false;
        }
        RawMeshStream(const std::string& semantic, MeshStreamType type, VALUE_TYPE format) : RawMeshStream(semantic, semantic, type, format) { }

        const std::string& GetName() const override { return name; }
        const std::string& GetSemantic() const override { return semantic; }
        MeshStreamType GetStreamType() const override { return streamType; }
        VALUE_TYPE GetFormat() const override { return format; }

        const void* GetData() const override { return data; }
        size_t GetSize() const override { return size; }
        size_t GetStride() const override { return GetValueSize(format); }

        bool IsDirty() const override { return isDirty; }
        void ClearDirty() override { isDirty = false; }
    };
}
