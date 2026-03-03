//
// Created by ryen on 3/5/25.
//

#pragma once

#include "IFileSystem.h"

namespace ME {
    class File : public IFile {
        private:
        void* data;
        size_t size;

        public:
        File(void* data, size_t size) {
            this->data = data;
            this->size = size;
        }
        ~File() override {
            if (data) {
                free(data);
                data = nullptr;
            }
            size = 0;
        }

        const void* Data() const override { return data; }
        size_t Size() const override { return size; }
    };
}
