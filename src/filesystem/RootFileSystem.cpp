//
// Created by ryen on 3/5/25.
//

#include <spdlog/spdlog.h>

#include "MECore/Filesystem/RootFileSystem.h"

namespace ME {
    void RootFileSystem::Mount(const std::filesystem::path& path, std::shared_ptr<IFileSystem> fs) {
        if (FindMountPoint(path, nullptr, nullptr)) {
            spdlog::error("Cannot mount a filesystem at {}. Another filesystem is already loaded there.", path.c_str());
            return;
        }

        mounted.push_back(std::make_pair(path.lexically_normal().generic_string(), fs));
    }

    bool RootFileSystem::Unmount(const std::filesystem::path& path) {
        std::string spath = path.lexically_normal().generic_string();

        for (size_t i = 0; i < mounted.size(); i++) {
            if (mounted[i].first == spath) {
                mounted.erase(mounted.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool RootFileSystem::FileExists(const std::filesystem::path& path) {
        std::filesystem::path relativePath;
        IFileSystem* fs = nullptr;

        if (FindMountPoint(path, &relativePath, &fs)) {
            return fs->FileExists(relativePath);
        }
        return false;
    }

    bool RootFileSystem::FolderExists(const std::filesystem::path& path) {
        std::filesystem::path relativePath;
        IFileSystem* fs = nullptr;

        if (FindMountPoint(path, &relativePath, &fs)) {
            return fs->FolderExists(relativePath);
        }
        return false;
    }

    std::shared_ptr<IFile> RootFileSystem::ReadFile(const std::filesystem::path& path) {
        std::filesystem::path relativePath;
        IFileSystem* fs = nullptr;

        if (FindMountPoint(path, &relativePath, &fs)) {
            return fs->ReadFile(relativePath);
        }
        return nullptr;
    }

    bool RootFileSystem::WriteFile(const std::filesystem::path& path, const void* data, size_t size) {
        std::filesystem::path relativePath;
        IFileSystem* fs = nullptr;

        if (FindMountPoint(path, &relativePath, &fs)) {
            return fs->WriteFile(relativePath, data, size);
        }
        return false;
    }

    int RootFileSystem::EnumerateFiles(const std::filesystem::path& path, const std::vector<std::string>& extensions, EnumerateCallback callback, bool allowDuplicates) {
        std::filesystem::path relativePath;
        IFileSystem* fs = nullptr;

        if (FindMountPoint(path, &relativePath, &fs)) {
            return fs->EnumerateFiles(relativePath, extensions, callback, allowDuplicates);
        }
        return Status::NotFound;
    }

    int RootFileSystem::EnumerateFolders(const std::filesystem::path& path, EnumerateCallback callback, bool allowDuplicates) {
        std::filesystem::path relativePath;
        IFileSystem* fs = nullptr;

        if (FindMountPoint(path, &relativePath, &fs)) {
            return fs->EnumerateFolders(relativePath, callback, allowDuplicates);
        }
        return false;
    }

    std::filesystem::path RootFileSystem::GetRealPath(const std::filesystem::path& path) {
        std::filesystem::path relativePath;
        IFileSystem* fs = nullptr;

        if (FindMountPoint(path, &relativePath, &fs)) {
            return fs->GetRealPath(relativePath);
        }
        return "";
    }

    bool RootFileSystem::FindMountPoint(const std::filesystem::path& path, std::filesystem::path* relativePath, IFileSystem** fs) {
        std::string spath = path.lexically_normal().generic_string();
        for (auto it : mounted) {
            if (spath.find(it.first, 0) == 0 && ((spath.length() == it.first.length()) || (spath[it.first.length()] == '/'))) {
                if (relativePath) {
                    std::string relative = (spath.length() == it.first.length()) ? "" : spath.substr(it.first.size() + 1);
                    *relativePath = relative;
                }
                if (fs) {
                    *fs = it.second.get();
                }
                return true;
            }
        }
        return false;
    }



}
