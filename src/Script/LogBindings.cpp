//
// Created by dotryen on 10/27/26.
//

#include <cassert>

#include "MECore/MECore.h"
#include "MECore/Script/Bindings.h"

namespace ME {
    void RegisterLog(asIScriptEngine* engine) {
        auto logger = MEInstance::Instance()->GetServiceProvider()->GetLogService();

        int r = engine->RegisterGlobalFunction("void Log_Info(const string&in)", asMETHOD(ILogService, LogInfo), asCALL_THISCALL_ASGLOBAL, logger);
        assert(r);
        r = engine->RegisterGlobalFunction("void Log_Warning(const string&in)", asMETHOD(ILogService, LogWarning), asCALL_THISCALL_ASGLOBAL, logger);
        assert(r);
        r = engine->RegisterGlobalFunction("void Log_Error(const string&in)", asMETHOD(ILogService, LogError), asCALL_THISCALL_ASGLOBAL, logger);
        assert(r);
    }
}
