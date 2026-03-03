//
// Created by dotryen on 12/18/25.
//

#pragma once
#include "IMeshStream.h"

namespace ME {
    class RawMeshStream final : public IMeshStream {
        private:
        std::string name;
        std::string semantic;
        MeshStreamType streamType;
        nvrhi::Format format;

        public:
        void* data;
        size_t size;
        bool isDirty;

        RawMeshStream(const std::string& name, const std::string& semantic, MeshStreamType type, nvrhi::Format format) {
            this->name = name;
            this->semantic = semantic;
            this->streamType = type;
            this->format = format;

            data = nullptr;
            size = 0;
            isDirty = false;
        }
        RawMeshStream(const std::string& semantic, MeshStreamType type, nvrhi::Format format) : RawMeshStream(semantic, semantic, type, format) { }

        const std::string& GetName() const override { return name; }
        const std::string& GetSemantic() const override { return semantic; }
        MeshStreamType GetStreamType() const override { return streamType; }
        nvrhi::Format GetFormat() const override { return format; }

        const void* GetData() const override { return data; }
        size_t GetSize() const override { return size; }
        size_t GetStride() const override { auto info = nvrhi::getFormatInfo(format); return info.blockSize * info.bytesPerBlock; }

        bool IsDirty() const override { return isDirty; }
        void ClearDirty() override { isDirty = false; }
    };
}
