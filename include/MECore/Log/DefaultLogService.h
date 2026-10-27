//
// Created by dotryen on 12/31/25.
//

#pragma once
#include <spdlog/spdlog.h>
#include <angelscript.h>

#include "ILogService.h"

namespace ME {
    class DefaultLogService : public ILogService {
        private:
        std::shared_ptr<spdlog::logger> mainLogger;
        std::shared_ptr<spdlog::logger> gameLogger;
        
        public:
        DefaultLogService();
        
        void RegisterScript(asIScriptEngine* engine);
        
        void LogInfo(const std::string& message) override;
        void LogWarning(const std::string& message) override;
        void LogError(const std::string& message) override;
    };
}
