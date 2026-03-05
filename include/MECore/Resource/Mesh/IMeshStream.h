//
// Created by dotryen on 12/18/25.
//

#pragma once

namespace ME {
    enum class MeshStreamType { Vertex, Index, Other };
    
    class IMeshStream {
        public:
        virtual ~IMeshStream() = default;

        virtual const std::string& GetName() const = 0;
        virtual const std::string& GetSemantic() const = 0;
        virtual MeshStreamType GetStreamType() const = 0;
        virtual Diligent::VALUE_TYPE GetFormat() const = 0;

        virtual const void* GetData() const = 0;
        virtual size_t GetSize() const = 0;
        virtual size_t GetStride() const = 0;
        size_t GetCount() const { return GetSize() / GetStride(); }

        virtual bool IsDirty() const = 0;
        virtual void ClearDirty() = 0;
    };
}
