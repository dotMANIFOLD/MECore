//
// Created by ryen on 2/21/25.
//

#include <spdlog/spdlog.h>

#include "MECore/Render/IRenderInterface.h"

namespace ME {
    DefaultMessageCallback& DefaultMessageCallback::GetInstance() {
        static DefaultMessageCallback instance;
        return instance;
    }

    void DefaultMessageCallback::message(nvrhi::MessageSeverity severity, const char* messageText) {
        spdlog::level::level_enum level = spdlog::level::info;
        switch (severity) {
            case nvrhi::MessageSeverity::Info: {
                level = spdlog::level::info;
                break;
            }
            case nvrhi::MessageSeverity::Warning: {
                level = spdlog::level::warn;
                break;
            }
            case nvrhi::MessageSeverity::Error: {
                level = spdlog::level::err;
                break;
            }
            case nvrhi::MessageSeverity::Fatal: {
                level = spdlog::level::critical;
                break;
            }
        }
        spdlog::log(level, messageText);
    }
}
