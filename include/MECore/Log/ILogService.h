//
// Created by dotryen on 12/20/25.
//

#pragma once

#include <string>
#include "MECore/Service/IService.h"

namespace ME {
    class ILogService : IService {
        public:
        virtual void LogInfo(const std::string& message) = 0;
        virtual void LogWarning(const std::string& message) = 0;
        virtual void LogError(const std::string& message) = 0;
    };
}
