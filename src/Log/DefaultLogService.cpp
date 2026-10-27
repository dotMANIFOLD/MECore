//
// Created by dotryen on 12/31/25.
//

#include "MECore/Log/DefaultLogService.h"

#include "MECore/MECore.h"
#include "spdlog/sinks/stdout_color_sinks-inl.h"

namespace ME {
    DefaultLogService::DefaultLogService() {
        mainLogger = spdlog::stdout_color_mt("ME_Main");
        gameLogger = spdlog::stdout_color_mt("ME_Game");
        spdlog::set_default_logger(mainLogger);
    }

    void DefaultLogService::RegisterScript(asIScriptEngine* engine) {
        auto result = engine->RegisterGlobalFunction("void Log_Info(const string&in)", asMETHOD(DefaultLogService, LogInfo), asCALL_THISCALL_ASGLOBAL, this);
        assert(result);
        result = engine->RegisterGlobalFunction("void Log_Warning(const string&in)", asMETHOD(DefaultLogService, LogWarning), asCALL_THISCALL_ASGLOBAL, this);
        assert(result);
        result = engine->RegisterGlobalFunction("void Log_Error(const string&in)", asMETHOD(DefaultLogService, LogError), asCALL_THISCALL_ASGLOBAL, this);
        assert(result);
    }

    void DefaultLogService::LogInfo(const std::string& message) {
        gameLogger->info(message);
    }
    
    void DefaultLogService::LogWarning(const std::string& message) {
        gameLogger->warn(message);
    }
    
    void DefaultLogService::LogError(const std::string& message) {
        gameLogger->error(message);
    }
}
