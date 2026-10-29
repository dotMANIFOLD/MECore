//
// Created by ryen on 3/5/25.
//

#include "MECore/Filesystem/NativeFileSystem.h"

#include <cassert>
#include <fstream>
#include <spdlog/spdlog.h>

#ifdef WIN32
#include <Shlwapi.h>
#else
extern "C" {
#include <glob.h>
}
#endif // _WIN32

#include "MECore/Filesystem/File.h"

namespace ME {
    // Magic function from nvidia that does cool things such as enumerating files on different systems
    static int EnumerateNativeFiles(const char* pattern, bool directories, EnumerateCallback callback) {
        #ifdef WIN32

        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA(pattern, &findData);

        if (hFind == INVALID_HANDLE_VALUE) {
            if (GetLastError() == ERROR_FILE_NOT_FOUND)
                return 0;

            return status::Failed;
        }

        int numEntries = 0;

        do {
            bool isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
            bool isDot = strcmp(findData.cFileName, ".") == 0;
            bool isDotDot = strcmp(findData.cFileName, "..") == 0;

            if ((isDirectory == directories) && !isDot && !isDotDot) {
                callback(findData.cFileName);
                ++numEntries;
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);

        return numEntries;

        #else // WIN32

        glob64_t glob_matches;
        int globResult = glob64(pattern, 0, nullptr, &glob_matches);

        if (globResult == 0) {
            int numEntries = 0;

            for (int i = 0; i < glob_matches.gl_pathc; ++i) {
                const char* globentry = (glob_matches.gl_pathv)[i];
                std::error_code ec, ec2;
                std::filesystem::directory_entry entry(globentry, ec);
                if (!ec) {
                    if (directories == entry.is_directory(ec2) && !ec2) {
                        callback(entry.path().filename().native());
                        ++numEntries;
                    }
                }
            }
            globfree64(&glob_matches);

            return numEntries;
        }

        if (globResult == GLOB_NOMATCH)
            return 0;

        return Status::Failed;

        #endif // WIN32
    }

    bool NativeFileSystem::FileExists(const std::filesystem::path& path) {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    bool NativeFileSystem::DirectoryExists(const std::filesystem::path& path) {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }

    std::shared_ptr<IFile> NativeFileSystem::ReadFile(const std::filesystem::path& path) {
        std::ifstream file(path, std::ios::binary);

        if (!file.is_open()) {
            return nullptr;
        }

        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        file.seekg(0, std::ios::beg);

        if (size > static_cast<uint64_t>(std::numeric_limits<std::streamsize>::max())) {
            // stupidly big ass file????
            assert(false);
            return nullptr;
        }

        char* data = static_cast<char*>(malloc(size));

        if (!data) {
            // out of memory
            assert(false);
            return nullptr;
        }

        file.read(data, size);

        if (!file.good()) {
            // read error
            assert(false);
            free(data);
            return nullptr;
        }

        return std::make_shared<File>(data, size);
    }

    bool NativeFileSystem::WriteFile(const std::filesystem::path& name, const void* data, size_t size) {
        std::ofstream file(name, std::ios::binary);

        if (!file.is_open()) {
            spdlog::warn("NativeFileSystem::WriteFile: Failed to open file {}", name.c_str());
            return false;
        }

        if (size > 0) {
            file.write(static_cast<const char*>(data), size);
        }

        if (!file.good()) {
            spdlog::warn("NativeFileSystem::WriteFile: Failed to write file {}", name.c_str());
            return false;
        }

        return true;
    }

    int NativeFileSystem::CreateDirectory(const std::filesystem::path& path) {
        auto result = std::filesystem::create_directory(path);
        return result ? Status::Ok : Status::Failed;
    }

    int NativeFileSystem::CreateDirectories(const std::filesystem::path& path) {
        auto result = std::filesystem::create_directories(path);
        return result ? Status::Ok : Status::Failed;
    }

    int NativeFileSystem::EnumerateFiles(const std::filesystem::path& path, const std::vector<std::string>& extensions, EnumerateCallback callback, bool allowDuplicates) {
        (void)allowDuplicates;

        if (extensions.empty()) {
            std::string pattern = (path / "*").generic_string();
            return EnumerateNativeFiles(pattern.c_str(), false, callback);
        }

        int numEntries = 0;
        for (const auto& ext : extensions) {
            std::string pattern = (path / ("*" + ext)).generic_string();
            int result = EnumerateNativeFiles(pattern.c_str(), false, callback);

            if (result < 0) return result;

            numEntries += result;
        }
        return numEntries;
    }

    int NativeFileSystem::EnumerateDirectories(const std::filesystem::path& path, EnumerateCallback callback, bool allowDuplicates) {
        (void)allowDuplicates;

        std::string pattern = (path / "*").generic_string();
        return EnumerateNativeFiles(pattern.c_str(), true, callback);
    }

    std::filesystem::path NativeFileSystem::GetRealPath(const std::filesystem::path& path) {
        return std::filesystem::absolute(path);
    }

}
