//
// Created by ryen on 3/5/25.
//

// Based off of donut's implementation

#pragma once

#include <memory>
#include <string>
#include <functional>
#include <filesystem>
#include <vector>

#include "MECore/Service/IService.h"

namespace ME {
    enum Status {
        Ok = 0,
        Failed = -1,
        NotFound = -2,
        NotImplemented = -3
    };

    using EnumerateCallback = const std::function<void(std::string_view)>&;

    inline std::function<void(std::string_view)> EnumerateToVector(std::vector<std::string>& vec) {
        return [&vec](std::string_view name) { vec.push_back(std::string(name)); };
    }

    class IFile {
        public:
        virtual ~IFile() = default;

        virtual const void* Data() const = 0;
        virtual size_t Size() const = 0;
    };

    class IFileSystem : IService {
        public:
        virtual bool FileExists(const std::filesystem::path& path) = 0;
        virtual bool FolderExists(const std::filesystem::path& path) = 0;

        virtual std::shared_ptr<IFile> ReadFile(const std::filesystem::path& path) = 0;
        virtual bool WriteFile(const std::filesystem::path& path, const void* data, size_t size) = 0;

        virtual int EnumerateFiles(const std::filesystem::path& path, const std::vector<std::string>& extensions, EnumerateCallback callback, bool allowDuplicates = false) = 0;
        virtual int EnumerateFolders(const std::filesystem::path& path, EnumerateCallback callback, bool allowDuplicates = false) = 0;

		virtual std::filesystem::path GetRealPath(const std::filesystem::path& path) = 0;
    };
}
