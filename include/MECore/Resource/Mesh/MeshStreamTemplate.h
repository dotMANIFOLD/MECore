//
// Created by dotryen on 12/18/25.
//

#pragma once
#include "IMeshStream.h"

namespace ME {
    template<typename T>
    class MeshStreamTemplate : public IMeshStream {
        private:
        std::string name;
        std::string semantic;

        public:
        std::vector<T> data;
        bool isDirty;

        MeshStreamTemplate(const std::string& semantic) {
            this->name = semantic;
            this->semantic = semantic;
            isDirty = false;
        }
        MeshStreamTemplate(const std::string& name, const std::string& semantic) {
            this->name = name;
            this->semantic = semantic;
            isDirty = false;
        }

        const std::string& GetName() const override { return name; }
        const std::string& GetSemantic() const override { return semantic; }

        const void* GetData() const override { return data.data(); }
        size_t GetSize() const override { return data.size() * GetStride(); }
        size_t GetStride() const override { return sizeof(T); }

        bool IsDirty() const override { return isDirty; }
        void ClearDirty() override { isDirty = false; }
    };
}
