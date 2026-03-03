//
// Created by ryen on 3/5/25.
//

#pragma once

#include "IFileSystem.h"

namespace ME {
    class NativeFileSystem : public IFileSystem {
        public:
        bool FileExists(const std::filesystem::path& path) override;
        bool FolderExists(const std::filesystem::path& path) override;

        std::shared_ptr<IFile> ReadFile(const std::filesystem::path& path) override;
        bool WriteFile(const std::filesystem::path& path, const void* data, size_t size) override;

        int EnumerateFiles(const std::filesystem::path& path, const std::vector<std::string>& extensions, EnumerateCallback callback, bool allowDuplicates) override;
        int EnumerateFolders(const std::filesystem::path& path, EnumerateCallback callback, bool allowDuplicates) override;

        std::filesystem::path GetRealPath(const std::filesystem::path& path) override;
    };
}
